//-------------------------------------------------------------------------------------------------//
//----------------------------------- Software Delay ----------------------------------------------//
//-------------------------------------------------------------------------------------------------//

void delay_us(unsigned int us);	  			// Delay in ms
void delay_ms(long ms);	  				    // Delay in ms


void delay_us(unsigned int us)
{  
	unsigned int z;
	us = us * 2;
	for(z=0;z<us;z++);           			// For 20MHz
}
void delay_ms(long ms)
{
	long j;
	ms = ms * 14;			 		// For 20MHz
	for(j=0;j<ms;j++);
}