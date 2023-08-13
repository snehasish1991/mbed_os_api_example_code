#include "mbed.h"
#include <inttypes.h>
#include "Sht31.h"


#define BUTTON_1 p5
#define BUTTON_2 p6

#define SWITCH_1 p7
#define SWITCH_2 p8

#define RED_LED_1 p9
#define RED_LED_2 p10
#define BLUE_LED  p11
#define YELLOW_LED p12

#define I2C_SDA p28
#define I2C_SCL p27

#define TRUE 1
#define FALSE 0

#define excess_temp 35 

//const int addr7bit = 0x48;      // 7 bit I2C address
//const int addr8bit = 0x48 << 1; // 8bit I2C address, 0x90

// Define the Input pins
DigitalIn start(BUTTON_1);
DigitalIn stop(BUTTON_2);
DigitalIn guard_switch(SWITCH_1);
DigitalIn temperature_control(SWITCH_2);


//Define the Output pins
DigitalOut fault_led_1(RED_LED_1);
DigitalOut fault_led_2(RED_LED_2);
DigitalOut running_state_led(BLUE_LED);
DigitalOut ready_to_start_led(YELLOW_LED);

Sht31 temp(I2C_SDA, I2C_SCL);

//Define Input/Output buses
	//Write your code here

void ControlLED_DigitalIO ();
void ControlLED_BusIO ();

int8_t machine_status = 0x00;

/*
void i2c_setup(I2C *i2c)
{
    char cmd[2]={0x00};
    cmd[0] = 0x01;
    cmd[1] = 0x00;
    i2c->write(addr8bit, cmd, 2);
}

void i2c_get_temp(float *temp, I2C *i2c)
{
    char cmd[2]={0x00};
    cmd[0] = 0x00;
    i2c->write(addr8bit, cmd, 1);
    i2c->read(addr8bit, cmd, 2);
    *temp = (float((cmd[0] << 8) | cmd[1]) / 256.0);
}
*/

int32_t main() {
    
    float temperature = 0.00;
    int8_t flag = 0x00;
    //set buttons to PULL UP
    start.mode(PullUp);
    stop.mode(PullUp);
    guard_switch.mode(PullUp);
    temperature_control.mode(PullUp);
    ready_to_start_led = 1;
   // temperature_control = 0x01;
    fault_led_1 = 0x00;
    //i2c_setup(&i2c)
    //check if initially guard switch is not engaged and temperature is within control
    if((guard_switch) || (temperature>=excess_temp)){
        ready_to_start_led = !ready_to_start_led;
        printf("Machine in not ready state\n");    
    }
    while(TRUE) {
        //i2c_get_temp(&temp, &i2c);
        temperature = temp.readTemperature();
        //printf("current temp %3.2fC \n", temperature);
        //check if stop button is pressed
        if(stop)
        {
            //check machine status, whether it was started, but went into a reset either 
            //due to rise in temperature or guard button was engaged
            if(machine_status) {
                printf("Machine stopped\n");
                machine_status = 0x00;
            }
            else {
                printf("Machine not started\n");
            }
            running_state_led = 0x00;
            ready_to_start_led = !running_state_led;
        }
        //check if machine can be started
        if((!guard_switch) && (temperature<excess_temp))
        {
            if(!machine_status) {
                ready_to_start_led = 1;
            }
            fault_led_1 = fault_led_2 = 0;
            //start button pressed and
            //check machine status, whether it was started, but went into a reset either 
            //due to rise in temperature or guard button was engaged
            if(machine_status) {
            if(start || (machine_status && flag))
            {
                printf("Machine started\n");
                machine_status = 0x01;
                flag = 0x00;
                running_state_led = 0x01;
                ready_to_start_led = !running_state_led;
            }
            
        }
        //action on engaging guard button
        if(guard_switch) {
            printf("Guard Switch engaged....machine stopped\n");
            fault_led_1 = !fault_led_1;
            flag = 0x01; 
            ready_to_start_led = running_state_led = 0x00;
        }
        //action if temperature went beyond threshold
        if(temperature>=excess_temp) {
            printf("High temperature....machine stopped\n");
            fault_led_2 = !fault_led_2;
            flag = 0x01;
            ready_to_start_led = running_state_led = 0x00;
        }
        //ControlLED_DigitalIO ();
        // ControlLED_BusIO ();
        wait(0.5);
    }
   // temp.reset();
    return 0x00;
}

void ControlLED_DigitalIO (){
          //Write your code here
}

void ControlLED_BusIO (){
    //Write your code here
}
