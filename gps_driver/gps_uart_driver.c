#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/time.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/kfifo.h>
#include <linux/mutex.h>
#include <linux/kthread.h>
#include <linux/semaphore.h>
#include <linux/string.h>
#include <linux/errno.h>

#include "gps_headers.h"

#define CLASS_NAME "GPS"
#define DEVICE_NAME "GPS"
#define MODULE_NAME "NEO-Blub";
#define FIFO_SIZE 4096

//static struct gpio_device* device  = NULL;
static struct task_struct* kernel_thread = NULL;
static DEFINE_MUTEX(buffer_lock);
static struct device *charDevice = NULL;
// IRQ_Number
static unsigned int irq_falling_endge_Number;

static uint8_t opened = 0; // flag indicates if the device is opened
static int major_number = 0;
static struct class *driver_class = NULL;
static uint8_t interrupt_flag = 0;
static uint8_t kthread_should_run = 1;
static DECLARE_WAIT_QUEUE_HEAD(RCV_RQ);
static DECLARE_KFIFO(recv_fifo,uint8_t,FIFO_SIZE); // declare kernel fifo 

static struct timespec irq_timestamp;

static void rcv_data(void){
	//Interrupt will come on the setting the bus to low so we wait one bit time before write tinto the buffer
	uint8_t data = 0, i = 0;// init variables with 0 
	struct timespec kthreadtime;
	// after the interrrup we are at the beginning of the signal
	// now wie put our sample point in the middle of the sampling time 
	//
	// udlay is more atomic as sleep functions so we used it here 
	getnstimeofday(&kthreadtime);
	udelay(BIT_TIME_US*0.6);
	for(i = 0; i < 8; i++ ){
		udelay(BIT_TIME_US);
		data |= (gpio_get_value(GPS_RX_PIN) << i);
	}
	mutex_lock(&buffer_lock);
	kfifo_put(&recv_fifo,data);
	mutex_unlock(&buffer_lock);

}

static int sendFunction(void *data) {
	INIT_KFIFO(recv_fifo); // init the kfifo
	while(kthread_should_run){
		// wait for interrupt
		wait_event_interruptible(RCV_RQ,interrupt_flag == 1);
		
		// double ckeck is we get woke up by the removing the module
		if(interrupt_flag && kthread_should_run){
			//printk_ratelimited(KERN_DEBUG "Start RX procedure");
			rcv_data();// make rcev of data
		}
		interrupt_flag = 0; // clear the flag and wait for anoter interrpt
	}
	// TODO: read the docs if relaxing a static kfifi is nessescary ??
#if 0
	kfifo_free(recv_fifo);
#endif
	printk(KERN_DEBUG "Stop Kernel Therad");
	return 0;
}


static irqreturn_t recive_irq_handler(unsigned int irq, void *data){
	interrupt_flag = 1;
	getnstimeofday(&irq_timestamp);
	wake_up_interruptible(&RCV_RQ);
	//printk_ratelimited(KERN_DEBUG"IRQ recvied ");
	
	return IRQ_HANDLED;
}

/*******************************************************************************
 * FILE Opterations for the char device
 ******************************************************************************/


int open_GPS(struct inode *node, struct file *file){
	if(opened){
		return -EBUSY;
	}
	opened = 1;

	return 0;
}
int close_GPS(struct inode *node,struct file *file){
	opened = 0;
	return 0;
}

ssize_t read_GPS(struct file *file_pt,char __user* buffer,size_t len,loff_t* offset){
	ssize_t ret = 0, copied = 0;
	printk(KERN_DEBUG"try to read %d bytes form the KFIFO",len);
	
	mutex_lock(&buffer_lock);
	ret = kfifo_to_user(&recv_fifo,buffer,len,&copied);
	mutex_unlock(&buffer_lock);
	//printk(KERN_DEBUG "Copied %d bytes to usr", copied);
	printk(KERN_DEBUG "kfifo to user return value: %d",ret);
	return ret ? ret : copied; 
}


static struct file_operations fops = {
		.owner = THIS_MODULE,
		.open = open_GPS,
		.release = close_GPS,
		.read = read_GPS
};
/*
 * Inline Function wich can occour while inti the module
 *
 */

inline void kthread_creation_failed(void){
}
inline void gpio_TX_failed(void){
	gpio_free(GPS_RX_PIN);
	kthread_creation_failed();
}

inline void irq_getting_number_failed(void){
	gpio_free(GPS_TX_PIN);
	gpio_TX_failed();
}
inline void chardev_creation_failed(void){
	free_irq(irq_falling_endge_Number,NULL);
	irq_getting_number_failed();
}
inline void class_creation_failed(void){
	unregister_chrdev(major_number, DEVICE_NAME);
	chardev_creation_failed();
}
inline void device_create_failed(void){
	class_destroy(driver_class);
	class_creation_failed();
}
static int __init module_entrence(void){
	int result = 0;
	// allocate Ram for buffering

	kernel_thread = kthread_run(sendFunction,NULL,"GPS_SENDER");
	if(IS_ERR(kernel_thread)){
		kthread_creation_failed();
		return -1;
	}
	if(!gpio_is_valid(GPS_RX_PIN) || !gpio_is_valid(GPS_TX_PIN)){
		printk(KERN_INFO"GPIO's are not Valid for this Board");
		kthread_creation_failed();// basicly its the same
		return -1;
	}
	// Lock GPIOS
	if(gpio_request(GPS_RX_PIN,"sysfs") != 0){
		printk(KERN_DEBUG"Could not get PIN:%d",GPS_RX_PIN);
		kthread_creation_failed();
		return -1;
	}
	printk(KERN_DEBUG "Requested %d",GPS_RX_PIN);
	if(gpio_request(GPS_TX_PIN,"sysfs") != 0){
		printk(KERN_DEBUG"Could not get PIN:%d",GPS_TX_PIN);
		gpio_TX_failed();
		return -1;
	}
	printk(KERN_DEBUG "Requested %d",GPS_TX_PIN);
	// set GPIO Direction
	gpio_direction_output(GPS_TX_PIN, 1);
	gpio_direction_input(GPS_RX_PIN);
	// set irq for the GPIO
	irq_falling_endge_Number = gpio_to_irq(GPS_RX_PIN);
	printk(KERN_DEBUG "GET IRQ_MAPPING: %d" ,irq_falling_endge_Number);
	if(irq_falling_endge_Number < 0 ){
		irq_getting_number_failed();
		return -1;
	}
	result = request_irq(irq_falling_endge_Number, (irq_handler_t) recive_irq_handler,IRQF_TRIGGER_FALLING,"GPS_FALLING_EDGE",NULL);
	printk(KERN_DEBUG "IRQ_REQUEST_RESULT: %d" ,result);
	if(result < 0){
		printk(KERN_DEBUG "IRQ_REQUEST FAILED" );
		irq_getting_number_failed();
		return -1;
	}

	printk(KERN_DEBUG "Register Char Device");
	if((major_number = register_chrdev(0, DEVICE_NAME, &fops)) < 0){
		chardev_creation_failed();
		return -1;
	}
	if(IS_ERR(driver_class = class_create(THIS_MODULE,CLASS_NAME))){
		class_creation_failed();
		return -1;
	}

	charDevice = device_create(driver_class, NULL, MKDEV(major_number,0), NULL, CLASS_NAME);
	if((result = IS_ERR(charDevice))){
		device_create_failed();
		return -1;
	}

	printk(KERN_INFO "GPS_UART driver for NEO-Blub loaded");
	return result;

}


static void __exit module_release(void){

	// Free Irq from the system
	free_irq(irq_falling_endge_Number,NULL);
	printk(KERN_DEBUG "free irq");

	// Free the GPIO form this Module
	gpio_free(GPS_RX_PIN);
	gpio_free(GPS_TX_PIN);
	printk(KERN_DEBUG "free gpio");
	// stop the KErnel therad
	kthread_should_run = 0;
	wake_up_interruptible(&RCV_RQ);
	printk(KERN_DEBUG "stop Kernel Thread");
	device_destroy(driver_class, MKDEV(major_number,0));
	printk(KERN_DEBUG"Destroyed Device in /dev/");
	class_destroy(driver_class);
	printk(KERN_DEBUG"Destroyed Class ");
	unregister_chrdev(major_number, DEVICE_NAME);
	printk(KERN_DEBUG "Release CharDevice");
	printk(KERN_INFO "GPS_UART driver for NEO-Blub unloaded");
	return;
}

module_init(module_entrence);
module_exit(module_release);
MODULE_LICENSE("GPL");
MODULE_VERSION("dev");
MODULE_DESCRIPTION("Simpel Implentation of uart Handling inside the Kernel");
MODULE_AUTHOR("Bambi");
