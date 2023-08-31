#include "mbed.h"
#include <inttypes.h>

#define SPEAKER p21
#define AIN1 p15
#define AIN2 p16
#define RED_LED p5

DigitalOut LED(RED_LED);
// Define the PWM speaker output

// Define analog inputs

//Write your code here
AnalogIn temp(AIN1);
AnalogIn level(AIN2);
PwmOut speaker(SPEAKER);

//Define variables
float i;
/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/
int32_t main(){
    int time = 2000;
    while(1){
        // Create a saw-tooth sound wave
        // Make the period and volume adjustable using the potentiometers
        printf("temp %f water level %f\n", temp.read(), level.read());
        speaker = 0.0;
        if(level.read()>0.80) {
            LED = 0x00;
            if(temp.read()>0.8) {
                time = 1000;
                 for (i=0; i<26; i=i+2) {
                     speaker.period(1.0/300.0);
                     speaker = float(i)/50.0;
                     wait_ms(500);
                     speaker.period(1.0/900.0);
                     wait_ms(500);
                 }
            } else if((temp.read()>=0.5) && (temp.read()<=0.79)) {
                speaker.period(1.0/300.0);
                speaker = float(i)/50.0;
                time = 2000;
            } else {
                 speaker.period(1.0/300.0);
                 speaker = float(i)/50.0;
                 wait_ms(100);
                 time = 1000;
            }
        } 
        if(level.read()<0.75) {
            for (i=0; i<1; i=i+0.05) {
                LED = !LED;
                speaker.period(0.010-(0.008*i));
                speaker = 0.25;//float(i)/50.0;
                wait_ms(500);
            }
        }
        wait_ms(time);
    }
    return 0x00;
}