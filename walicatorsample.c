#include<reg51.h>
#include "_DLY_R8C.c"
#include "_LCD_R8C.c"
#include "_I2C_R8C.c"
#include "_RTC_R8C.c"

sbit led_full = P3^7; 
sbit led_almostfull = P3^6;
sbit led_semifull = P3^5;
sbit led_half = P3^4;
sbit led_quarter = P3^3;
sbit led_low = P3^2;
sbit led_empty = P3^1;
sbit sens_echo = P1^2;
sbit sens_trig = P1^3;
sbit SET = P3^0;
float distance; /* distance between top of tank(ultrasonic sensor) & top surface of water */
float capacity; /* total capacity of the tank selected */
float tank_length = 2.35; /* length of tank in metres */
float tank_breadth = 2.61; /* breadth of tank in metres*/
float tank_height = 0.9; /*height of tank in metres*/
float tank_remainingwaterheight; /* height of water left in tank in metres */
float tank_remainingwatervolume; /* volume of wate rleft in tank in mteres */
int tank_heightpercent; /* percentage of height of water left in tank */
int percent_water; /* percentage of water left in tank */
int x,y,e,r;

void main(){
	
	capacity = tank_length*tank_breadth*tank_height;
	
	lcd_init();    
	lcd_clear();
	
	ds1307_init();  
	
	SET = 1;
	if(SET == 0){
		ds1307_set_time(9,7,20,2,5,20,00);
	}
	
	while(1){
		
		sens_trig = 0;
		delay_us(2); // wait for 2 microseconds
		
		sens_trig = 1;
		ds1307_get_time();
    r = min;		
    x = sec;		
		delay_us(10); // wait for 10 microsecnds 
		
		sens_trig = 0;
		
		if (sens_echo == 1){
			ds1307_get_time();
			e = min;
			y = sec;
			if (e = r){
				break;
			}
			else{
				y = y + 60;
			}
			
			distance = (((y-x)*343)/2);
			tank_remainingwaterheight = tank_height-distance;
			tank_heightpercent = ((tank_remainingwaterheight/tank_height)*100);
			tank_remainingwatervolume = tank_remainingwaterheight*tank_breadth*tank_length;
			percent_water = ((tank_remainingwatervolume/capacity)*100);
			
			if (0 <= tank_heightpercent > 6){
				led_empty = 1;
				lcd_printxy(1,1,"Empty");
				lcd_printxy(2,1,"Water Level: ");
				lcd_print_b(percent_water);
			}
			else if(6 <= tank_heightpercent > 21){
				led_low = 1;
				lcd_printxy(1,1,"Low");
				lcd_printxy(2,1,"Water Level: ");
				lcd_print_b(percent_water);
			}
			else if(21 <= tank_heightpercent > 38){
				led_quarter = 1;
				lcd_printxy(1,1,"Quarter Full");
				lcd_printxy(2,1,"Water Level: ");
				lcd_print_b(percent_water);
			}
			else if(38 <= tank_heightpercent > 63){
				led_half = 1;
				lcd_printxy(1,1,"Half Full");
				lcd_printxy(2,1,"Water Level: ");
				lcd_print_b(percent_water);
			}
			else if(63 <= tank_heightpercent > 88){
				led_semifull = 1;
				lcd_printxy(1,1,"Semi Full");
				lcd_printxy(2,1,"Water Level: ");
				lcd_print_b(percent_water);
			}
			else if(88 <= tank_heightpercent > 96){
				led_almostfull = 1;
				lcd_printxy(1,1,"Almost Full");
				lcd_printxy(2,1,"Water Level: ");
				lcd_print_b(percent_water);
		  } 
			else{
				led_full = 1;
				lcd_printxy(1,1,"Full : ");
				lcd_print_b(percent_water);
			}
		}
	}
}