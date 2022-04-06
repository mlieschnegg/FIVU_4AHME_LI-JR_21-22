#ifndef _MPU6050_H_
#define _MPU6050_H_



//https://www.robotics.org.za/GY-521
#define MPU6050_DEFAULT_ADDRESS     0x68            //Default Adress (AD0 is low)


//Register Address (RA) Definitions


#define MPU6050_RA_PWR_MGMT_1               0x6B
#define MPU6050_RA_CONFIG                   0x1A
#define MPU6050_RA_GYRO_CONFIG              0x1B
#define MPU6050_RA_ACCEL_CONFIG             0x1C
#define MPU6050_RA_SMPLRT_DIV               0x19



#define MPU6050_RA_ACCEL_XOUT_H             0x3B
#define MPU6050_RA_SENOR_XOUT_START         MPU6050_RA_ACCEL_XOUT_H

#define MPU6050_PWR_MGMT_1_DEVICE_RESET_BIT 7
#define MPU6050_PWR_MGMT_1_SLEEP_BIT        6
#define MPU6050_PWR_MGMT_1_CLKSEL0_BIT      0
#define MPU6050_PWR_MGMT_1_CLKSEL1_BIT      1
#define MPU6050_PWR_MGMT_1_CLKSEL2_BIT      2
  

#define MPU6050_CONFIG_DLPF_CFG0_BIT        0
#define MPU6050_CONFIG_DLPF_CFG1_BIT        1
#define MPU6050_CONFIG_DLPF_CFG2_BIT        2

#define MPU6050_GYRO_CONFIG_FSEL0_BIT       3     
#define MPU6050_GYRO_CONFIG_FSEL1_BIT       4

#define MPU6050_ACCEL_CONFIG_FSEL0_BIT       3     
#define MPU6050_ACCEL_CONFIG_FSEL1_BIT       4


void MPU6050_initializeSensor(void);
void MPU6050_readSensorData(void);
double MPU6050_getACCX();
double MPU6050_getACCY();
double MPU6050_getACCZ();
double MPU6050_getTemp();
double MPU6050_getGYRX();
double MPU6050_getGYRY();
double MPU6050_getGYRZ();
#endif /* _MPU6050_H_ */
