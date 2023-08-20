#include "mbed.h"
#include <inttypes.h>

#define RED_LED_B1 p5
#define RED_LED_B2 p6
#define RED_LED_B3 p7
#define RED_LED_Fire p8

#define YELLOW_LED_1 p9
#define YELLOW_LED_2 p10
#define YELLOW_LED_3 p11
#define YELLOW_LED_4 p12
#define YELLOW_LED_5 p13
#define YELLOW_LED_6 p14

#define YELLOW_LED_7 p19
#define YELLOW_LED_8 p20


#define BLUE_LED p15 

#define Customer_adult p16
#define Customer_child p17 
#define fire_alarm p18

int8_t adult_customer = 0x00, child_customer = 0x00, available_space = 8; 
uint8_t seats_avail = 0xff;
int8_t alarm_blown = 0x00;

//Define outputs
DigitalOut barber_1(RED_LED_B1);
DigitalOut barber_2(RED_LED_B2);
DigitalOut barber_3(RED_LED_B3);
DigitalOut Fire_Alarm(RED_LED_Fire);
DigitalOut Clock(BLUE_LED);

//DigitalOut SEAT_1(YELLOW_LED_1);
//DigitalOut SEAT_2(YELLOW_LED_2);
//DigitalOut SEAT_3(YELLOW_LED_3);
//DigitalOut SEAT_4(YELLOW_LED_4);
//DigitalOut SEAT_5(YELLOW_LED_5);
//DigitalOut SEAT_6(YELLOW_LED_6);

BusOut Seat(YELLOW_LED_1, YELLOW_LED_2, YELLOW_LED_3, YELLOW_LED_4, YELLOW_LED_5, YELLOW_LED_6, YELLOW_LED_7, YELLOW_LED_8);

//Define interrupt inputs
//Intreupts treating each customer
InterruptIn Customer1(Customer_adult);
InterruptIn Customer2(Customer_child);
InterruptIn fire(fire_alarm);

//Define ISRs for the interrupts
void adult_customerFunc() {
    if(available_space>0) {
        adult_customer++;
        printf("adult %d\n", adult_customer);
        available_space--;
        Seat = (uint8_t)(Seat>>1);
        //Seat = seats_avail;
        printf("available %d\n", available_space);
    }
}

void child_customerFunc() {
    if(available_space>0) {
        child_customer+=1;
        available_space-=2;
        printf("available %d\n", available_space);
        printf("child %d\n", child_customer);
        Seat = (uint8_t)(Seat>>2);
    }
    
}

void fireAlarm() {
    if(!alarm_blown) {
        Fire_Alarm = 0x01;
        Seat = 0x00;
        barber_1 = barber_2 = barber_3 = 0x00;
        alarm_blown = 1;
    }
    else {
        Fire_Alarm = 0x00;
        Seat = 0xff;
        barber_1 = barber_2 = barber_3 = 0x00;
        alarm_blown = 0x00;
        //since all customers left
        adult_customer = child_customer = 0x00; 
        available_space = 8;
    }
}


/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int32_t main(){
    
	int8_t barber1_status = 0, barber2_status = 0, barber3_status = 0;
	int8_t barber1_timer = 12, barber2_timer = 12, barber3_timer = 12;
	//Initially turn off all LEDs
    barber_1 = barber_2 = barber_3 = Fire_Alarm = 0x00;
	//Write your code here
	
	//Interrupt handlers
	//Attach the address of the ISR to the rising edge
	Customer1.rise(&adult_customerFunc);
	Customer2.rise(&child_customerFunc);
	fire.rise(&fireAlarm);
	//Write your code here
	Seat = 0xff;
	//wait 100 ms
	while(1) {
	    if(available_space==0x00) {
	        Fire_Alarm = 0x01;
	    } 
	    else {
	        Fire_Alarm = 0x00;
	    }
	    if((!barber1_status) && (adult_customer>0x00)) {
	        barber_1 = barber1_status = !barber1_status;        
	        adult_customer--;
	        available_space++;
	        //seats_avail = seats_avail<<1;
            Seat = (uint8_t)((Seat)|0x1);
	        printf("available %d adult customer %d\n", available_space, adult_customer);
	    }
	    
	    if((!barber2_status) && (adult_customer>0x00)) {
	        barber_2 = barber2_status = !barber2_status;        
	        adult_customer--;
	        available_space++;
	        Seat = (uint8_t)((Seat<<1)|0x1);
	        //printf("seats available %x\n", Seat);
            //Seat = seats_avail;
	        printf("available %d adult customer %d\n", available_space, adult_customer);
	    }
	    
	    if((!barber3_status) && (child_customer>0x00)) {
	        barber_3 = barber3_status = !barber3_status;        
	        child_customer--;
	        Seat = (uint8_t)((Seat<<1)|0x1);
	    }
	    
	    if(barber1_status) {
	        if (barber1_timer>=0) {
	            barber1_timer--;
	        }
	        else {
	            barber1_timer = 12;
	            barber_1 = barber1_status = !barber1_status;
	        }
	    }
	    
	    if(barber2_status) {
	        if (barber2_timer>=0) {
	            barber2_timer--;
	        }
	        else {
	            barber2_timer = 12;
	            barber_2 = barber2_status = !barber2_status;
	        }
	    }
	    
	    if(barber3_status) {
	        if (barber3_timer>=0) {
	            barber3_timer--;
	        }
	        else {
	            barber3_timer = 12;
	            barber_3 = barber3_status = !barber3_status;
	            available_space+=2;
	            Seat = (uint8_t)((Seat<<1)|0x1);
	            //printf("child seats available %x\n", Seat);
                //Seat = seats_avail;
	        }
	    }
		wait_ms(1000);
		Clock = !Clock;
	}
	printf("test\n");
	return 0x00;
}

