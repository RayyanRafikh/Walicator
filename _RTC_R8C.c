//*************************************************************************************************//
//*************************************************************************************************//
//------------------------------------------ DS1307 RTC -------------------------------------------//
//*************************************************************************************************//
//*************************************************************************************************//

//-------------------------------------------------------------------------------------------------//
//-------------------------------- Variables & Functions Declaration ------------------------------//
//-------------------------------------------------------------------------------------------------//

void ds1307_init(void);
void ds1307_set_time(char day, char mth, char year, char dow, char hrs, char min, char sec);
void ds1307_get_time(void);
char bin2bcd(char bin_value);
char bcd2bin(char bcd_value);
char ds1307read_sec(void);
char day,mth,year,dow,hrs,min,sec;

//-------------------------------------------------------------------------------------------------//
//---------------------------------------- Main Routines ------------------------------------------//
//-------------------------------------------------------------------------------------------------//

void ds1307_init()
{
	char temp,temp1;
	temp = i2c_read(0xd0,0);
	temp1 = bcd2bin(temp);
	temp1 = temp1 & 0x7f;
	delay_us(5);
	temp = bin2bcd(temp1);	
	i2c_write(0xd0,0,temp);
	i2c_write(0xd0,7,0x80);
}

//-------------------------------------------------------------------------------------------------//

void ds1307_set_time(char day, char mth, char year, char dow, char hrs, char min, char sec)
{
	sec = sec & 0x7F;
	hrs = hrs & 0x3F;
	
	sec = bin2bcd(sec);
	min = bin2bcd(min);
	hrs = bin2bcd(hrs);
	dow = bin2bcd(dow);
	year = bin2bcd(year);
	mth = bin2bcd(mth);
	day = bin2bcd(day);
	
	i2c_write(0xD0,0x00,sec);
	i2c_write(0xD0,0x01,min);
	i2c_write(0xD0,0x02,hrs);
	i2c_write(0xD0,0x03,dow);
	i2c_write(0xD0,0x04,day);
	i2c_write(0xD0,0x05,mth);
	i2c_write(0xD0,0x06,year);
	i2c_write(0xD0,0x07,0x11);
}

//-------------------------------------------------------------------------------------------------//

void ds1307_get_time(void)
{
	year =i2c_read(0xD0,0x06);
	mth = i2c_read(0xD0,0x05);
	day = i2c_read(0xD0,0x04);
	dow = i2c_read(0xD0,0x03);
	hrs = i2c_read(0xD0,0x02);
	min = i2c_read(0xD0,0x01);
	sec = i2c_read(0xD0,0x00);
	
	dow = dow & 0x7f;
	day = day & 0x3f;
	mth = mth & 0x1f;
	hrs = hrs & 0x3f;
	min = min & 0x7f;
	sec = sec & 0x7f;
	
	day = bcd2bin(day);
	mth = bcd2bin(mth);
	year =bcd2bin(year);
	dow = bcd2bin(dow);
	hrs = bcd2bin(hrs);
	min = bcd2bin(min);
	sec = bcd2bin(sec);
}

//-------------------------------------------------------------------------------------------------//
//---------------------------- BIN TO BCD & BCD TO BIN Conversion Routines ------------------------//
//-------------------------------------------------------------------------------------------------//

char bin2bcd(char bin_value)
{
  char temp;
  char retval;

  temp = bin_value;
  retval = 0;

  while(1)
  {
    if(temp >= 10)										// Get tens digit by multiple subtraction of 10 from the bin value.
    {
      temp -= 10;
      retval += 0x10;
    }
    else 												// Get the ones digit by adding the remainder.
    {
      retval += temp;
      break;
    }
  }

  return(retval);
}

char bcd2bin(char bcd_value)
{
  char temp;
  temp = bcd_value;
  temp >>= 1;											// Shifting upper digit right by 1 is same as multiplying by 8.
  temp &= 0x78;											// Isolate the bits for the upper digit.
  return(temp + (temp >> 2) + (bcd_value & 0x0f));		// Now return: (Tens * 8) + (Tens * 2) + Ones
}

//-------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------//