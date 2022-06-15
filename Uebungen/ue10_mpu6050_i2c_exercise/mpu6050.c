#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "twi.h"

#include "mpu6050.h"

uint8_t gl_uc_twi_buffer[TWI_BUFFER_SIZE];
double gl_f_accval[3];
double gl_f_gyrval[3];
double gl_f_tempval;


void MPU6050_initializeSensor(void)
{
    
    TWI_initializeTWI();

    //Reset Device
    gl_uc_twi_buffer[0] = MPU6050_RA_PWR_MGMT_1;
    gl_uc_twi_buffer[1] = (1 << MPU6050_PWR_MGMT_1_DEVICE_RESET_BIT);
    TWI_txBytes(MPU6050_DEFAULT_ADDRESS, gl_uc_twi_buffer, 2);
    TWI_waitUntilIdle();
    //wait until device has reseted
    _delay_ms(100);


    //select internal 8 MHz clock source -> set clksel to zero
    //disable sleep mode    -> set sleep bit to zero
    gl_uc_twi_buffer[0] = MPU6050_RA_PWR_MGMT_1;
    
    //select internal 8 MHz clock source -> set clksel to zero
    //disable sleep mode    -> set sleep bit to zero
    gl_uc_twi_buffer[1] = 0;
    TWI_txBytes(MPU6050_DEFAULT_ADDRESS, gl_uc_twi_buffer, 2);
    TWI_waitUntilIdle();


    //set range for gyroscope
    gl_uc_twi_buffer[0] = MPU6050_RA_GYRO_CONFIG;
    gl_uc_twi_buffer[1] = 0; //page. 14, no self-test, FSR ... +- 250 °/s
    TWI_txBytes(MPU6050_DEFAULT_ADDRESS, gl_uc_twi_buffer, 2);
    TWI_waitUntilIdle();


    //set range for accelerometer
    gl_uc_twi_buffer[0] = MPU6050_RA_ACCEL_CONFIG;
    gl_uc_twi_buffer[1] = 0; //page 15,  no self test, FSR ... +- 2g
    TWI_txBytes(MPU6050_DEFAULT_ADDRESS, gl_uc_twi_buffer, 2);
    TWI_waitUntilIdle();


    //set bandwidth of DLPF
    gl_uc_twi_buffer[0] = MPU6050_RA_CONFIG;
    //page 13. -> set BW of DLPF to aprox. 50 Hz
    gl_uc_twi_buffer[1] = (1 << MPU6050_CONFIG_DLPF_CFG0_BIT) | (1 << MPU6050_CONFIG_DLPF_CFG1_BIT);
    TWI_txBytes(MPU6050_DEFAULT_ADDRESS, gl_uc_twi_buffer, 2);
    TWI_waitUntilIdle();


    //set sample rate
    //Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV) 
    //where Gyroscope Output Rate = 8kHz when the DLPF is disabled (DLPF_CFG = 0 or 7), and 1kHz 
    //when the DLPF is enabled
    
    gl_uc_twi_buffer[0] = MPU6050_RA_SMPLRT_DIV;
    gl_uc_twi_buffer[1] = 1000 / 100 - 1;            //set sample rate to 100 Hz
    TWI_txBytes(MPU6050_DEFAULT_ADDRESS, gl_uc_twi_buffer, 2);
    TWI_waitUntilIdle();

}


void MPU6050_readSensorData(void)
{
    gl_uc_twi_buffer[0] = MPU6050_RA_ACCEL_XOUT_H;
    //TWI_txrxBytes(MPU6050_DEFAULT_ADDRESS, gl_uc_twi_buffer, 1, 14);
    TWI_txrxBytes(MPU6050_DEFAULT_ADDRESS, gl_uc_twi_buffer, 1, 14);
    TWI_waitUntilIdle();

    //acc values
    //TODO: convert the register values of the MPU6050 from digits to acceleration of unit "g"
    gl_f_accval[0] = ((int16_t) ((gl_uc_twi_buffer[0] << 8) | (gl_uc_twi_buffer[1])))/16384.0;
    gl_f_accval[1] = ((int16_t) ((gl_uc_twi_buffer[2] << 8) | (gl_uc_twi_buffer[3])))/16384.0;
    gl_f_accval[2] = ((int16_t) ((gl_uc_twi_buffer[4] << 8) | (gl_uc_twi_buffer[5])))/16384.0;

    //gl_f_tempval
    gl_f_tempval = ((int16_t) (gl_uc_twi_buffer[6] << 8) | (gl_uc_twi_buffer[7]))/340.0 + 36.53;

    //gyro values
    gl_f_gyrval[0] = ((int16_t) ((gl_uc_twi_buffer[8] << 8) | (gl_uc_twi_buffer[9])))/131.0;
    gl_f_gyrval[1] = ((int16_t) ((gl_uc_twi_buffer[10] << 8) | (gl_uc_twi_buffer[11])))/131.0;
    gl_f_gyrval[2] = ((int16_t) ((gl_uc_twi_buffer[12] << 8) | (gl_uc_twi_buffer[13])))/131.0;
    
}

double MPU6050_getACCX()
{
    return gl_f_accval[0];
}

double MPU6050_getACCY()
{
    return gl_f_accval[1];
}

double MPU6050_getACCZ()
{
    return gl_f_accval[2];
}

double MPU6050_getGYRX()
{
    return gl_f_gyrval[0];
}

double MPU6050_getGYRY()
{
    return gl_f_gyrval[1];
}

double MPU6050_getGYRZ()
{
    return gl_f_gyrval[2];
}

double MPU6050_getTemp()
{
    return gl_f_tempval;
}