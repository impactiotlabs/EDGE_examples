#include "I2C.h"
 
const int MCP7940_I2C  = 0x6F;  // I2C Address for the RTC
 
const int REG_RTCSEC   = 0x00;  // Register Address: Time Second
const int REG_RTCMIN   = 0x01;  // Register Address: Time Minute
const int REG_RTCHOUR  = 0x02;  // Register Address: Time Hour
const int REG_RTCWKDAY = 0x03;  // Register Address: Date Day of Week
const int REG_RTCDATE  = 0x04;  // Register Address: Date Day
const int REG_RTCMTH   = 0x05;  // Register Address: Date Month
const int REG_RTCYEAR  = 0x06;  // Register Address: Date Year
 
byte      timeStamp[7];         // Byte array holding a full time stamp.
 
// Array position is the same as the register address.
 
void setup()
{
 
  Serial.begin (9600);
  
  I2c.begin    ();       // Initialize the I2C library
  I2c.pullup   (0);      // Disable the internal pullup resistors
  I2c.setSpeed (0);      // Enable 100kHz I2C Bus Speed
  I2c.timeOut  (250);    // Set a 250ms timeout before the bus resets
   
  Serial.print    ("Current Time: ");
  // These are the values that will be written to the MCP7940
  timeStamp[0]  = convertToBcd(   00);    // SECONDS
  timeStamp[1]  = convertToBcd(  23);    // MINUTES
  timeStamp[2]  = convertToBcd(  10);    // HOURS
  timeStamp[3]  = convertToBcd(   6);    // DAY OF WEEK (arbitrary value 1 - 7)
  timeStamp[4]  = convertToBcd(  06);    // DAY
  timeStamp[5]  = convertToBcd(  11);    // MONTH
  timeStamp[6]  = convertToBcd(  17);    // YEAR
 
  // Write our time stamp to the time/date registers
  I2c.write(MCP7940_I2C, REG_RTCSEC, timeStamp, 7);
 
  // Initialize our chip with any further configuration data
  init_MCP7940();
  
}
 
void loop()
{
  
  I2c.read(MCP7940_I2C, REG_RTCSEC,  7, timeStamp);
  
  Serial.print    ("Current Time: ");
  Serial.print    (convertFromBcd(timeStamp[2] & 0x3F));
  Serial.print    (":");
  if              (convertFromBcd(timeStamp[1] & 0x7F) / 10 == 0) Serial.print ("0");
  Serial.print    (convertFromBcd(timeStamp[1] & 0x7F));
  Serial.print    (":");
  if              (convertFromBcd(timeStamp[0] & 0x7F) / 10 == 0) Serial.print ("0");  
  Serial.println  (convertFromBcd(timeStamp[0] & 0x7F));
 
  delay(1000);
  
}
 
void init_MCP7940() {
  
  byte    registerValue = 0x00;    // Holds the received register value
  byte    twelveHour    = 0x00;    // 0 = 24 Hour Clock Mode / 1 = 12 Hour Clock Mode
  byte    startClock    = 0x01;    // 0 = Start Oscillator   / 1 = Stop Oscillator
   byte    batEnable     = 0x01; 

  
  // Turn on/off: 12 hour vs. 24 hour clock
  I2c.read    (MCP7940_I2C, REG_RTCHOUR, 1);
  registerValue = I2c.receive();
  if (twelveHour == 0x00) I2c.write (MCP7940_I2C, REG_RTCHOUR, bitClear (registerValue, 6));
  if (twelveHour == 0x01) I2c.write (MCP7940_I2C, REG_RTCHOUR, bitSet   (registerValue, 6));
 
  // Turn on/off: Oscillator (starts the clock)
  I2c.read    (MCP7940_I2C, REG_RTCSEC, 1);
  registerValue = I2c.receive();
  if (startClock == 0x00) I2c.write (MCP7940_I2C, REG_RTCSEC, bitClear (registerValue, 7));
  if (startClock == 0x01) I2c.write (MCP7940_I2C, REG_RTCSEC, bitSet   (registerValue, 7));
  ConfigureRegister  (REG_RTCWKDAY, batEnable,   3);   
}
 void ConfigureRegister(byte registerAddress, byte value, byte positions) {
  
  //  registerAddress: the single byte register that you will be writing to
  //  value:           a single bit of data, 1 or 0, that you want set or cleared in the register
  //  positions:       the location of that single bit of data within the register, 0 indexed
  
  byte registerValue = 0x00;    // Holds each config value when read from the register
  I2c.read (MCP7940_I2C, registerAddress, 1);
  registerValue      = I2c.receive();
  if (value == 0x00) I2c.write (MCP7940_I2C, registerAddress, bitClear (registerValue, positions));
  if (value == 0x01) I2c.write (MCP7940_I2C, registerAddress, bitSet   (registerValue, positions));
  
 }
byte convertToBcd(byte byteDecimal) {
 
  return (byteDecimal / 10) << 4 | (byteDecimal % 10);
 
}
 
byte convertFromBcd(byte byteBCD) {
 
  byte byteMSB = 0;
  byte byteLSB = 0;
 
  byteMSB      = (byteBCD & B11110000) >> 4;
  byteLSB      = (byteBCD & B00001111);
 
  return       ((byteMSB * 10) + byteLSB);
}


