#include <reg52.h>
#include <stdio.h>
#include <LCD_8_bit.h>
#include <math.h>

#define sound_velocity 34300  	/* sound velocity in cm per second */
#define period_in_us pow(10,-6)
#define Clock_period 1.085*period_in_us		/* Frequency for timer of 8051 with 11.0592 MHz crystal is 1/12th of 11.0592 MHz which is 921.6 MHz, therefore timer period is 1/(timer frequency)*/

sbit Trigger_pin = P1^3;        	/* Trigger pin on the HCSR04 module*/
sbit Echo_pin = P1^4;							/* Echo pin on the HCSR04 module*/
sbit led_full = P3^7; 
sbit led_almostfull = P3^6;
sbit led_semifull = P3^5;
sbit led_half = P3^4;
sbit led_quarter = P3^3;
sbit led_low = P3^2;
sbit led_empty = P3^1;

void Delay_us()
{
		TL0=0xF5;
		TH0=0xFF;
		TR0=1;
		while (TF0==0);
		TR0=0;
		TF0=0;
}

void init_timer(){
	TMOD=0x01;										/*initialize Timer*/
	TF0=0;
	TR0 = 0;
}

void send_trigger_pulse(){
	  Trigger_pin= 1;           	/* pull trigger pin HIGH */
    Delay_us();               	/* provide 10uS Delay*/
    Trigger_pin = 0;          	/* pull trigger pin LOW*/
}

void main(){
	float tank_length = 235.0;           /* length of tank in cm */
  float tank_breadth = 261.0;          /* breadth of tank in cm*/
  float tank_height = 90.0;            /*height of tank in cm*/
	float tank_remainingwaterheight;   /* height of water left in tank*/
	float tank_heightpercent;          /*percentage of height of water left in tank*/
	float tank_remainingwatervolume;   /*volume of water left in tank*/
	float percent_water;               /*percentage of volume of water left in tank*/
	float distance_measurement;      /* distance between sensor probe and surface of water in cm */
	float value;
  float max_heightpercent_detectable;
	int min_dis = 2; /* minimum distance detectable by the ultrasonic sensor in cm */
	unsigned char percentage[10];     /* percentage of water as string(text)*/
	LCD_Init();										    /* Initialize 16x2 LCD */	
	init_timer();									   /* Initialize Timer*/
	
  while(1){
		send_trigger_pulse();				/* send trigger pulse of 10us */
    
		while(!Echo_pin);           /* Waiting for Echo */
    TR0 = 1;                    /* Timer Starts */
    while(Echo_pin && !TF0);    /* Waiting for Echo goes LOW */
    TR0 = 0;                    /* Stop the timer */
	  
		/* calculate distance with timer */
		value = Clock_period*sound_velocity; 
		distance_measurement = (TL0|(TH0<<8));										/* read timer reg for time count */
		distance_measurement = (distance_measurement*value)/2.0;  /* find distance(in cm) */
	  tank_remainingwaterheight = tank_height-distance_measurement;
		tank_heightpercent = (tank_remainingwaterheight/tank_height)*100.0;
	  tank_remainingwatervolume = tank_remainingwaterheight*tank_breadth*tank_length;
		percent_water = ((tank_remainingwatervolume/(tank_length*tank_breadth*tank_height))*100.0);
	  sprintf(percentage, "%.2f", percent_water);
		max_heightpercent_detectable = (((tank_height-min_dis)/tank_height)*100.0);
		if ((tank_heightpercent>0 && tank_heightpercent<6.0)){
			  LCD_String_xy(1,1,"                ");
			  LCD_String_xy(2,1,"                ");
				led_empty = 1;
				led_low = 0;
				led_quarter = 0;
				led_half = 0;
				led_semifull = 0;
				led_almostfull = 0;
				led_full = 0;
			  LCD_String_xy(1,1,"Empty           ");          /* show distance on 16x2 LCD */
			  LCD_String_xy(2,1,"Water: ");		
		    LCD_String(percentage);	
        LCD_String(" %");			
			  delay(100);
			}
			else if(tank_heightpercent>=6.0 && tank_heightpercent<21.0){
				LCD_String_xy(1,1,"                ");
			  LCD_String_xy(2,1,"                ");
				led_empty = 0;
				led_low = 1;
				led_quarter = 0;
				led_half = 0;
				led_semifull = 0;
				led_almostfull = 0;
				led_full = 0;
				LCD_String_xy(1,1,"Low             ");
			  LCD_String_xy(2,1,"Water: ");											
		    LCD_String(percentage);	
        LCD_String(" %");			
			  delay(100);
			}
			else if(tank_heightpercent>=21.0 && tank_heightpercent<38.0){
				LCD_String_xy(1,1,"                ");
			  LCD_String_xy(2,1,"                ");
				led_empty = 0;
				led_low = 0;
				led_quarter = 1;
				led_half = 0;
				led_semifull = 0;
				led_almostfull = 0;
				led_full = 0;
				LCD_String_xy(1,1,"Quarter full    ");
			  LCD_String_xy(2,1,"Water: ");												
		    LCD_String(percentage);	
        LCD_String(" %");		
			  delay(100);
			}
			else if(tank_heightpercent>=38.0 && tank_heightpercent<63.0){
				LCD_String_xy(1,1,"                ");
			  LCD_String_xy(2,1,"                ");
				led_empty = 0;
				led_low = 0;
				led_quarter = 0;
				led_half = 1;
				led_semifull = 0;
				led_almostfull = 0;
				led_full = 0;
				LCD_String_xy(1,1,"Half full       ");
			  LCD_String_xy(2,1,"Water: ");												
		    LCD_String(percentage);	
        LCD_String(" %");			
			  delay(100);
			}
			else if(tank_heightpercent>=63.0 && tank_heightpercent<88.0){
				LCD_String_xy(1,1,"                ");
			  LCD_String_xy(2,1,"                ");
				led_empty = 0;
				led_low = 0;
				led_quarter = 0;
				led_half = 0;
				led_semifull = 1;
				led_almostfull = 0;
				led_full = 0;
				LCD_String_xy(1,1,"Semi full       ");
			  LCD_String_xy(2,1,"Water: ");											
		    LCD_String(percentage);	
        LCD_String(" %");		
			  delay(100);
			}
			else if(tank_heightpercent>=88.0 && tank_heightpercent<94){
				LCD_String_xy(1,1,"                ");
			  LCD_String_xy(2,1,"                ");
				led_empty = 0;
				led_low = 0;
				led_quarter = 0;
				led_half = 0;
				led_semifull = 0;
				led_almostfull = 1;
				led_full = 0;
				LCD_String_xy(1,1,"Almost full     ");
			  LCD_String_xy(2,1,"Water: ");												
		    LCD_String(percentage);	
        LCD_String(" %");		
			  delay(100);
		  } 
			else{ /* in all other cases, that is 1. when the tank_heightpercent becomes negative(of any magnitude), i.e. when the distance becomes greater 
				than height(which is an error which occurs only when the object is 
				within 5-7 cm from the sensor) and when the tank_heightpercent > 100*/
				LCD_String_xy(1,1,"                ");
			  LCD_String_xy(2,1,"                ");
				led_empty = 0;
				led_low = 0;
				led_quarter = 0;
				led_half = 0;
				led_semifull = 0;
				led_almostfull = 0;
				led_full = 1;
				LCD_String_xy(1,1,"Full            ");
			  LCD_String_xy(2,1,"Water: ");												
		    LCD_String(percentage);	
        LCD_String(" %");		
			  delay(100);
			}
		}
}
