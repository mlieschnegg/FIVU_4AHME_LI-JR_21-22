#include "global.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "uart.h"

#define SERVO_PWM_PERIOD_MS             20                                  //PWM-period in milli seconds
#define SERVO_TIMER_CLOCK               16000000/8                          //prescaled timer clock of timer 1
#define SERVO_TIMER_CLOCKS_PER_MS       SERVO_TIMER_CLOCK/1000 
#define SERVO_CLOCKS_PER_PWM_PERIOD     SERVO_PWM_PERIOD_MS*SERVO_TIMER_CLOCKS_PER_MS

#define SERVO_POS_MS_MIN                0.5     // ms
#define SERVO_POS_MS_MAX                2.4     // ms
#define SERVO_POS_DEGREE_MIN            0.0     // degree
#define SERVO_POS_DEGREE_MAX            180.0   // degree
#define SERVO_POS_VOLT_MIN              0.0     //volt
#define SERVO_POS_VOLT_MAX              5.0     //volt


//ToDo: add reference value for the ADC
#define ADC_VREF_VALUE                  0     //volt

typedef struct 
{
    char *cmd;
    void (*func)(char *);
} TerminalCMD_t;

uint8_t  pos_degree_poti = 0; //degree

void SERVO_initializeServo(void);
void SERVO_moveAbsolute(uint8_t degree);


//callbacks for received commands

//move Servo to absolute position
void execMoveAServo(char *s)
{
    uint8_t pos_degree;
    pos_degree = atoi(s);
    SERVO_moveAbsolute(pos_degree);
    sprintf(s, "MOVE to pos: %d deg\n", pos_degree);
    UART_printText(s);
}

//a list off all supported commmands
TerminalCMD_t mycommands[] = {
    {
        "MOVEA=",
        execMoveAServo
    }
};

uint8_t executeCommand(char *s)
{
    for(uint8_t i = 0, n=0; i < sizeof(mycommands)/sizeof(mycommands[0]); ++i)
    {
        n = strlen(mycommands[i].cmd);
        if (strncmp(s, mycommands[i].cmd, n) == 0)
        {
            if (mycommands[i].func != NULL)
                mycommands[i].func(&s[n]);
            return 0;
        }
    }
    return 1;
}


void SERVO_initializeServo(void)
{
    //FAST PWM SETTINGS USING TIMER1
    DDRB |= (1 << PB1);                                     //set PB1 for OC1A to output mode
    TCCR1A = (1 << COM1A1) | (1 << WGM11);                  //Clear OC1A on compare match, set OC1A at BOTTOM
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);     //Fast PWM in Mode 14, prescaler = 8
    OCR1A = SERVO_POS_MS_MIN * SERVO_TIMER_CLOCKS_PER_MS;                             //Capture Compare value for 1 ms pulse width = servor middle position
    ICR1 = SERVO_CLOCKS_PER_PWM_PERIOD;                     //Top value to set periodicity
}

void SERVO_moveAbsolute(uint8_t degree)
{
    
    //  0% ...   0 degree ... 1 ms
    //100% ... 180 degree ... 2 ms

    const double k_ms_per_degree = (SERVO_POS_MS_MAX-SERVO_POS_MS_MIN) / (SERVO_POS_DEGREE_MAX - SERVO_POS_DEGREE_MIN);
    const double d_ms = SERVO_POS_MS_MAX - k_ms_per_degree * SERVO_POS_DEGREE_MAX;

    double pos_ms = k_ms_per_degree * degree + d_ms;
    uint16_t pos_ocra = pos_ms * SERVO_TIMER_CLOCKS_PER_MS;

    OCR1A = pos_ocra;
}

void ADC_initializeADC(void)
{

    //ADC SETTINGS

    //ToDo:
    //1.) Enable AVCC with external capacitor at AREF pin
    //2.) Switch the mux to channel ADC3 (=Temperature Sensor)
    //3.) Set conversion speed to F_CPU/128. full resolution of the internal ADC will be reached at a speed of 50...150 kHz
    //4.) Enable the auto triggering mode -> auto trigger enable -> mode must be set in ADCSRB
    //5.) Enable the free running mode
    //6.) Enable the ADC
    //7.) Start the first conversion this will also start free running mode
}

double ADC_readVoltage(void)
{
    //ToDo: calculate and return the measured voltage
}

int main()
{
    char s[UART_BUFFER_SIZE] = "";

    SERVO_initializeServo();
    ADC_initializeADC();

    UART_initializeInterface(115200, PARITY_NO);

    UART_printText("Exercise 13 - ADC SERVO\n");
    
    while(1)
    {

        if (ADCSRA & (0x01 << ADIF))
        {
            ADCSRA |= (0x01 << ADIF);

            double voltage = ADC_readVoltage();
            
            if (voltage < SERVO_POS_VOLT_MIN)
                voltage = SERVO_POS_VOLT_MIN;
            else if (voltage > SERVO_POS_VOLT_MAX)
                voltage = SERVO_POS_VOLT_MAX;
            
            //ToDo: Calculate and update the Servo-PWM Value
            //ToDo: Only update the servo pwm vlaue if the measured voltage has changed!
            //      Use the function SERVO_moveAbsolute to move the servo
        }
        if (UART_getText(s) == UART_OK)
        {
            if (executeCommand(s))
                UART_printText("Invalid Command!\n");
        }
    }
    return 0;
}
