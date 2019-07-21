/**************************************************************************/
/*!
    @file     Adafruit_MSA301.cpp
    @author   Limor Fried (Adafruit Industries)
    @license  BSD (see license.txt)

    This is a library for the Adafruit MSA301 Accel breakout board
    ----> https://www.adafruit.com

    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!

*/
/**************************************************************************/

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>
#include <Adafruit_MSA301.h>


/**************************************************************************/
/*!
    @brief  Instantiates a new MSA301 class
*/
/**************************************************************************/
Adafruit_MSA301::Adafruit_MSA301() {}


/*!
 *    @brief  Sets up the hardware and initializes I2C
 *    @param  i2c_address
 *            The I2C address to be used.
 *    @param  wire
 *            The Wire object to be used for I2C connections.
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_MSA301::begin(uint8_t i2c_address, TwoWire *wire) {
  i2c_dev = new Adafruit_I2CDevice(i2c_address, wire);

  if (!i2c_dev->begin()) {
    Serial.println("Failed to init i2c address");
    return false;
  }

  // Check connection
  Adafruit_BusIO_Register chip_id = 
    Adafruit_BusIO_Register(i2c_dev, MSA301_REG_PARTID, 1);


  // make sure we're talking to the right chip
  if (chip_id.read() != 0x13) {
    // No MSA301 detected ... return false
    return false;
  }

  // enable all axes
  enableAxes(true, true, true);
  // normal mode
  setPowerMode(MSA301_NORMALMODE);
  // 500Hz rate
  setDataRate(MSA301_DATARATE_500_HZ);
  // 250Hz bw
  setBandwidth(MSA301_BANDWIDTH_250_HZ);
  setRange(MSA301_RANGE_4_G);
  setResolution(MSA301_RESOLUTION_14);

  /*
  // DRDY on INT1
  writeRegister8(MSA301_REG_CTRL3, 0x10);

  // Turn on orientation config
  //writeRegister8(MSA301_REG_PL_CFG, 0x40);

  */
  /*
  for (uint8_t i=0; i<0x30; i++) {
    Serial.print("$");
    Serial.print(i, HEX); Serial.print(" = 0x");
    Serial.println(readRegister8(i), HEX);
  }
  */

  return true;
}




/**************************************************************************/
/*!
    @brief  Sets the data rate for the MSA301 (controls power consumption)
*/
/**************************************************************************/
void Adafruit_MSA301::setDataRate(msa301_dataRate_t dataRate)
{
  Adafruit_BusIO_Register ODR = 
    Adafruit_BusIO_Register(i2c_dev, MSA301_REG_ODR, 1);
  Adafruit_BusIO_RegisterBits dataratebits = Adafruit_BusIO_RegisterBits(&ODR, 4, 0);
  dataratebits.write((uint8_t)dataRate);
}

/**************************************************************************/
/*!
    @brief  Sets the data rate for the MSA301 (controls power consumption)
*/
/**************************************************************************/
msa301_dataRate_t Adafruit_MSA301::getDataRate(void)
{
  Adafruit_BusIO_Register ODR = 
    Adafruit_BusIO_Register(i2c_dev, MSA301_REG_ODR, 1);
  Adafruit_BusIO_RegisterBits dataratebits = Adafruit_BusIO_RegisterBits(&ODR, 4, 0);
  return (msa301_dataRate_t)dataratebits.read();
}

void Adafruit_MSA301::enableAxes(bool enableX, bool enableY, bool enableZ) {
  Adafruit_BusIO_Register ODR = 
    Adafruit_BusIO_Register(i2c_dev, MSA301_REG_ODR, 1);

  Adafruit_BusIO_RegisterBits x = Adafruit_BusIO_RegisterBits(&ODR, 1, 7);
  Adafruit_BusIO_RegisterBits y = Adafruit_BusIO_RegisterBits(&ODR, 1, 6);
  Adafruit_BusIO_RegisterBits z = Adafruit_BusIO_RegisterBits(&ODR, 1, 5);
  x.write(!enableX); // these are *disable* bits, yeah...
  y.write(!enableY);
  z.write(!enableZ);
}

void Adafruit_MSA301::setPowerMode(msa301_powermode_t mode)
{
  Adafruit_BusIO_Register PowerMode = 
    Adafruit_BusIO_Register(i2c_dev, MSA301_REG_POWERMODE, 1);
  Adafruit_BusIO_RegisterBits powermodebits = Adafruit_BusIO_RegisterBits(&PowerMode, 2, 6);
  powermodebits.write((uint8_t)mode);
}


msa301_powermode_t Adafruit_MSA301::getPowerMode(void)
{
  Adafruit_BusIO_Register PowerMode = 
    Adafruit_BusIO_Register(i2c_dev, MSA301_REG_POWERMODE, 1);
  Adafruit_BusIO_RegisterBits powermodebits = Adafruit_BusIO_RegisterBits(&PowerMode, 2, 6);
  return (msa301_powermode_t)powermodebits.read();
}


void Adafruit_MSA301::setBandwidth(msa301_bandwidth_t bandwidth)
{
  Adafruit_BusIO_Register PowerMode = 
    Adafruit_BusIO_Register(i2c_dev, MSA301_REG_POWERMODE, 1);
  Adafruit_BusIO_RegisterBits bandwidthbits = Adafruit_BusIO_RegisterBits(&PowerMode, 4, 1);
  bandwidthbits.write((uint8_t)bandwidth);
}


msa301_bandwidth_t Adafruit_MSA301::getBandwidth(void)
{
  Adafruit_BusIO_Register PowerMode = 
    Adafruit_BusIO_Register(i2c_dev, MSA301_REG_POWERMODE, 1);
  Adafruit_BusIO_RegisterBits bandwidthbits = Adafruit_BusIO_RegisterBits(&PowerMode, 4, 1);
  return (msa301_bandwidth_t)bandwidthbits.read();
}



/**************************************************************************/
/*!
    @brief  Sets the g range for the accelerometer
*/
/**************************************************************************/
void Adafruit_MSA301::setRange(msa301_range_t range)
{
  Adafruit_BusIO_Register ResRange = 
    Adafruit_BusIO_Register(i2c_dev, MSA301_REG_RESRANGE, 1);
  Adafruit_BusIO_RegisterBits rangebits = Adafruit_BusIO_RegisterBits(&ResRange, 2, 0);
  rangebits.write((uint8_t)range);
}

/**************************************************************************/
/*!
    @brief  Gets the g range for the accelerometer
*/
/**************************************************************************/
msa301_range_t Adafruit_MSA301::getRange(void)
{
  Adafruit_BusIO_Register ResRange = 
    Adafruit_BusIO_Register(i2c_dev, MSA301_REG_RESRANGE, 1);
  Adafruit_BusIO_RegisterBits rangebits = Adafruit_BusIO_RegisterBits(&ResRange, 2, 0);
  return (msa301_range_t)rangebits.read();
}


/**************************************************************************/
/*!
    @brief  
*/
/**************************************************************************/
void Adafruit_MSA301::setResolution(msa301_resolution_t resolution)
{
  Adafruit_BusIO_Register ResRange = 
    Adafruit_BusIO_Register(i2c_dev, MSA301_REG_RESRANGE, 1);
  Adafruit_BusIO_RegisterBits resbits = Adafruit_BusIO_RegisterBits(&ResRange, 2, 2);
  resbits.write((uint8_t)resolution);
}

/**************************************************************************/
/*!
    @brief  
*/
/**************************************************************************/
msa301_resolution_t Adafruit_MSA301::getResolution(void)
{
  Adafruit_BusIO_Register ResRange = 
    Adafruit_BusIO_Register(i2c_dev, MSA301_REG_RESRANGE, 1);
  Adafruit_BusIO_RegisterBits resbits = Adafruit_BusIO_RegisterBits(&ResRange, 2, 2);
  return (msa301_resolution_t)resbits.read();
}



void Adafruit_MSA301::read(void) {
  uint8_t buffer[6];

  Adafruit_BusIO_Register XYZDataReg = 
    Adafruit_BusIO_Register(i2c_dev, MSA301_REG_OUT_X_L, 6);
  XYZDataReg.read(buffer, 6);
  x = buffer[0]; x |= buffer[1] << 8;
  y = buffer[2]; y |= buffer[3] << 8;
  z = buffer[4]; z |= buffer[5] << 8;

  msa301_range_t range = getRange();
  float divider = 3.9; // 3.9mg in 2g
  if (range == MSA301_RANGE_16_G) divider = 31.3 * 1000.0;
  if (range == MSA301_RANGE_8_G) divider = 15.6 * 1000.0;
  if (range == MSA301_RANGE_4_G) divider = 7.8 * 1000.0;
  if (range == MSA301_RANGE_2_G) divider = 3.9 * 1000.0;

  x_g = (float)x / divider;
  y_g = (float)y / divider;
  z_g = (float)z / divider;
}


/**************************************************************************/
/*!
    @brief  Set INT to output for single or double click
*/
/**************************************************************************/

void Adafruit_MSA301::setClick(bool tap_quiet, bool tap_shock, msa301_tapduration_t tapduration, uint8_t tapthresh) {
  Adafruit_BusIO_Register TapDur = 
    Adafruit_BusIO_Register(i2c_dev, MSA301_REG_TAPDUR, 1);
  Adafruit_BusIO_RegisterBits quietbit = Adafruit_BusIO_RegisterBits(&TapDur, 1, 7);
  quietbit.write(tap_quiet);
  Adafruit_BusIO_RegisterBits shockbit = Adafruit_BusIO_RegisterBits(&TapDur, 1, 6);
  shockbit.write(tap_shock);
  Adafruit_BusIO_RegisterBits durationbits = Adafruit_BusIO_RegisterBits(&TapDur, 3, 0);
  durationbits.write(tapduration);

  Adafruit_BusIO_Register TapTh = 
    Adafruit_BusIO_Register(i2c_dev, MSA301_REG_TAPTH, 1);
  Adafruit_BusIO_RegisterBits threshbits = Adafruit_BusIO_RegisterBits(&TapTh, 5, 0);
  threshbits.write(tapthresh);
}



uint8_t Adafruit_MSA301::getClick(void) {
  Adafruit_BusIO_Register ClickStatus = 
    Adafruit_BusIO_Register(i2c_dev, MSA301_REG_CLICKSTATUS, 1);

  return ClickStatus.read();
}

void Adafruit_MSA301::enableInterrupts(bool orient, bool singletap, bool doubletap,
				       bool activeX, bool activeY, bool activeZ,
				       bool newData, bool freefall) {
  Adafruit_BusIO_Register IntSet0 = 
    Adafruit_BusIO_Register(i2c_dev, MSA301_REG_INTSET0, 1);
  Adafruit_BusIO_Register IntSet1 = 
    Adafruit_BusIO_Register(i2c_dev, MSA301_REG_INTSET1, 1);
  uint8_t irqs = 0;
  irqs |= orient << 6;
  irqs |= singletap << 5;
  irqs |= doubletap << 4;
  irqs |= activeX << 0;
  irqs |= activeY << 1;
  irqs |= activeZ << 2;
  IntSet0.write(irqs);
  irqs = 0;
  irqs |= newData << 4;
  irqs |= freefall << 3;
  IntSet1.write(irqs);
}

uint8_t Adafruit_MSA301::getMotionInterruptStatus(void) {
  Adafruit_BusIO_Register IntStatus = 
    Adafruit_BusIO_Register(i2c_dev, MSA301_REG_MOTIONINT, 1);

  return IntStatus.read();
}

uint8_t Adafruit_MSA301::getDataInterruptStatus(void) {
  Adafruit_BusIO_Register IntStatus = 
    Adafruit_BusIO_Register(i2c_dev, MSA301_REG_DATAINT, 1);

  return IntStatus.read();
}


/**************************************************************************/
/*!
    @brief  Gets the most recent sensor event
*/
/**************************************************************************/
bool Adafruit_MSA301::getEvent(sensors_event_t *event) {
  /* Clear the event */
  memset(event, 0, sizeof(sensors_event_t));

  event->version   = sizeof(sensors_event_t);
  event->sensor_id = _sensorID;
  event->type      = SENSOR_TYPE_ACCELEROMETER;
  event->timestamp = 0;

  read();

  event->acceleration.x = x_g * SENSORS_GRAVITY_STANDARD;
  event->acceleration.y = y_g * SENSORS_GRAVITY_STANDARD;
  event->acceleration.z = z_g * SENSORS_GRAVITY_STANDARD;
    
  return true;
}

/**************************************************************************/
/*!
    @brief  Gets the sensor_t data
*/
/**************************************************************************/
void Adafruit_MSA301::getSensor(sensor_t *sensor) {
  /* Clear the sensor_t object */
  memset(sensor, 0, sizeof(sensor_t));

  /* Insert the sensor name in the fixed length char array */
  strncpy (sensor->name, "MSA301", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name)- 1] = 0;
  sensor->version     = 1;
  sensor->sensor_id   = _sensorID;
  sensor->type        = SENSOR_TYPE_ACCELEROMETER;
  sensor->min_delay   = 0;
  sensor->max_value   = 0;
  sensor->min_value   = 0;
  sensor->resolution  = 0;
}

