dir=$PWD
cd gps_driver 
make && sudo -E  make install
# E as argument saves the enviroment variales 
cd $dir
var=$(cat /etc/modules | grep gps_uart_driver)
# copy files with rules 
sudo cp 90-gps.rules /etc/udev/rules.d/ -v
#reload rules 
sudo udevadm trigger
