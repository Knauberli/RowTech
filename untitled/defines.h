#ifndef DEFINES_H
#define DEFINES_H
#define GRAVITY_CONSTANT 9.81

// i2c Gyro Defines
#define I2C_GYRO_DEFAULT_BUS "/dev/i2c-3"
#define I2C_GYRO_SLAVE_ADDRES 0b1101000



#define REG_SELF_TEST_X 0x0d
#define REG_SELF_TEST_Y 0x0e
#define REG_SELF_TEST_Z 0x0f
#define REG_SELF_TEST_A 0x10

#define REG_SMPLTR_DIV 0x19
#define REG_CONFIG 0x1a
#define REG_GYRO_CONFIG 0x1b
#define REG_ACCEL_CONFIG 0x1c

#define REG_FIFO_EN 0x23
#define REG_I2C_MST_CTL 0x24
#define REG_I2C_SLV0_ADDR 0x25
#define REG_I2C_SLV0_REG 0x26
#define REG_I2C_SLV0_CTRL 0x27

#define REG_I2C_SLV1_ADDR 0x28
#define REG_I2C_SLV1_REG 0x29
#define REG_I2C_SLV1_CTRL 0x2a

#define REG_I2C_SLV2_ADDR 0x2b
#define REG_I2C_SLV2_REG 0x2c
#define REG_I2C_SLV2_CTRL 0x2d

#define REG_I2C_SLV3_ADDR 0x2e
#define REG_I2C_SLV3_REG 0x2f
#define REG_I2C_SLV3_CTRL 0x30

#define REG_I2C_SLV4_ADDR 0x31
#define REG_I2C_SLV4_REG 0x32
#define REG_I2C_SLV4_DO 0x33
#define REG_I2C_SLV4_CTL 0x34
#define REG_I2C_SLV4_Di 0x35

#define REG_I2C_MST_STATUS 0x36
#define REG_INT_PIN_CFG 0x37
#define REG_INT_ENABLE 0x38
#define REG_INT_STATUS 0x3a

#define REG_ACCEL_XOUT_H 0x3b
#define REG_ACCEL_XOUT_L 0x3c

#define REG_ACCEL_YOUT_H 0x3d
#define REG_ACCEL_YOUT_L 0x3e

#define REG_ACCEL_ZOUT_H 0x3f
#define REG_ACCEL_ZOUT_L 0x40

#define REG_TEMP_OUT_H 0x41
#define REG_TEMP_OUT_L 0x42

#define REG_GYRO_XOUT_H 0x43
#define REG_GYRO_XOUT_L 0x44

#define REG_GYRO_YOUT_H 0x45
#define REG_GYRO_YOUT_L 0x46

#define REG_GYRO_ZOUT_H 0x47
#define REG_GYRO_ZOUT_L 0x48

#define REG_SENS_DATA_00 0x49
#define REG_SENS_DATA_01 0x4a
#define REG_SENS_DATA_02 0x4b
#define REG_SENS_DATA_03 0x4c
#define REG_SENS_DATA_04 0x4d
#define REG_SENS_DATA_05 0x4e
#define REG_SENS_DATA_06 0x4f
#define REG_SENS_DATA_07 0x50
#define REG_SENS_DATA_08 0x51
#define REG_SENS_DATA_09 0x52
#define REG_SENS_DATA_10 0x53
#define REG_SENS_DATA_11 0x54
#define REG_SENS_DATA_12 0x55
#define REG_SENS_DATA_13 0x56
#define REG_SENS_DATA_14 0x57
#define REG_SENS_DATA_15 0x58
#define REG_SENS_DATA_16 0x59
#define REG_SENS_DATA_17 0x5a
#define REG_SENS_DATA_18 0x5b
#define REG_SENS_DATA_19 0x5c
#define REG_SENS_DATA_20 0x5d
#define REG_SENS_DATA_21 0x5e
#define REG_SENS_DATA_22 0x5f
#define REG_SENS_DATA_24 0x60

#define REG_I2C_SLV0_DO 0x63
#define REG_I2C_SLV1_DO 0x64
#define REG_I2C_SLV2_DO 0x65
#define REG_I2C_SLV3_D0 0x66

#define REG_I2C_MST_DELAY_CTRL 0x67
#define REG_SIGNAL_PATH_RESET 0x68

#define REG_USER_CTRL 0x6a
#define REG_PWR_MGMT_1 0x6b
#define REG_PWR_MGMT_2 0x6c

#define REG_FIFO_COUNT_H 0x72
#define REG_FIFO_COUNT_L 0x73
#define REG_FIFO_R_W 0x74
#define REG_WHO_AM_I 0x75


#define ACCEL_SHORT_TO_DOUBLE_FACTOR_0 16384.0
#define ACCEL_SHORT_TO_DOUBLE_FACTOR_1 8192.0
#define ACCEL_SHORT_TO_DOUBLE_FACTOR_2 4096.0
#define ACCEL_SHORT_TO_DOUBLE_FACTOR_3 2048.0

#define GYRO_SHORT_TO_DOUBLE_FACTOR_0 131.0
#define GYRO_SHORT_TO_DOUBLE_FACTOR_1 65.5
#define GYRO_SHORT_TO_DOUBLE_FACTOR_2 32.8
#define GYRO_SHORT_TO_DOUBLE_FACTOR_3 16.4

#endif // DEFINES_H
