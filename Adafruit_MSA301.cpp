/*!
 * @file     Adafruit_MSA301.cpp
 *
 * @mainpage Adafruit MSA301 Accelerometer Breakout
 *
 * @section intro_sec Introduction
 *
 * This is a library for the Adafruit MSA301 Accel breakout board
 * ----> https://www.adafruit.com
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section author Author
 *
 * Limor Fried (Adafruit Industries)
 *
 * @section license License
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#include <Adafruit_MSA301.h>

/**************************************************************************/
/*!
    @brief  Instantiates a new MSA301 class
*/
/**************************************************************************/
Adafruit_MSA301::Adafruit_MSA301() {}

/**************************************************************************/
/*!
    @brief  Instantiates a new MSA311 class
*/
/**************************************************************************/
Adafruit_MSA311::Adafruit_MSA311() {}

/*!
 *    @brief  Sets up the hardware and initializes I2C
 *    @param  i2c_address
 *            The I2C address to be used.
 *    @param  wire
 *            The Wire object to be used for I2C connections.
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_MSA311::begin(uint8_t i2c_address, TwoWire *wire) {
  return Adafruit_MSA301::begin(i2c_address, wire);
}

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
    from 1 Hz to 1000Hz
    @param dataRate Enumerated msa301_dataRate_t
*/
/**************************************************************************/
void Adafruit_MSA301::setDataRate(msa301_dataRate_t dataRate) {
  Adafruit_BusIO_Register ODR =
      Adafruit_BusIO_Register(i2c_dev, MSA301_REG_ODR, 1);
  Adafruit_BusIO_RegisterBits dataratebits =
      Adafruit_BusIO_RegisterBits(&ODR, 4, 0);
  dataratebits.write((uint8_t)dataRate);
}

/**************************************************************************/
/*!
    @brief  Gets the data rate for the MSA301 (controls power consumption)
    @return Enumerated msa301_dataRate_t from 1 Hz to 1000Hz
*/
/**************************************************************************/
msa301_dataRate_t Adafruit_MSA301::getDataRate(void) {
  Adafruit_BusIO_Register ODR =
      Adafruit_BusIO_Register(i2c_dev, MSA301_REG_ODR, 1);
  Adafruit_BusIO_RegisterBits dataratebits =
      Adafruit_BusIO_RegisterBits(&ODR, 4, 0);
  return (msa301_dataRate_t)dataratebits.read();
}

/**************************************************************************/
/*!
    @brief  What axes of the accelerometer we want enabled for reading
    @param enableX True to enable X axis
    @param enableY True to enable Y axis
    @param enableZ True to enable Z axis
*/
/**************************************************************************/
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

/**************************************************************************/
/*!
    @brief Set the power mode, MSA301_NORMALMODE, MSA301_LOWPOWERMODE or
    MSA301_SUSPENDMODE
    @param mode Enumerated msa301_powermode_t
*/
/**************************************************************************/
void Adafruit_MSA301::setPowerMode(msa301_powermode_t mode) {
  Adafruit_BusIO_Register PowerMode =
      Adafruit_BusIO_Register(i2c_dev, MSA301_REG_POWERMODE, 1);
  Adafruit_BusIO_RegisterBits powermodebits =
      Adafruit_BusIO_RegisterBits(&PowerMode, 2, 6);
  powermodebits.write((uint8_t)mode);
}

/**************************************************************************/
/*!
    @brief Get the power mode
    @returns Enumerated msa301_powermode_t, MSA301_NORMALMODE,
   MSA301_LOWPOWERMODE or
    MSA301_SUSPENDMODE
*/
/**************************************************************************/
msa301_powermode_t Adafruit_MSA301::getPowerMode(void) {
  Adafruit_BusIO_Register PowerMode =
      Adafruit_BusIO_Register(i2c_dev, MSA301_REG_POWERMODE, 1);
  Adafruit_BusIO_RegisterBits powermodebits =
      Adafruit_BusIO_RegisterBits(&PowerMode, 2, 6);
  return (msa301_powermode_t)powermodebits.read();
}

/**************************************************************************/
/*!
    @brief Set the bandwidth, ranges from 1.95Hz to 500Hz
    @param bandwidth Enumerated msa301_range_t
*/
/**************************************************************************/
void Adafruit_MSA301::setBandwidth(msa301_bandwidth_t bandwidth) {
  Adafruit_BusIO_Register PowerMode =
      Adafruit_BusIO_Register(i2c_dev, MSA301_REG_POWERMODE, 1);
  Adafruit_BusIO_RegisterBits bandwidthbits =
      Adafruit_BusIO_RegisterBits(&PowerMode, 4, 1);
  bandwidthbits.write((uint8_t)bandwidth);
}

/**************************************************************************/
/*!
    @brief Get the bandwidth
    @return Enumerated msa301_bandwidth_t, ranges from 1.95Hz to 500Hz
*/
/**************************************************************************/
msa301_bandwidth_t Adafruit_MSA301::getBandwidth(void) {
  Adafruit_BusIO_Register PowerMode =
      Adafruit_BusIO_Register(i2c_dev, MSA301_REG_POWERMODE, 1);
  Adafruit_BusIO_RegisterBits bandwidthbits =
      Adafruit_BusIO_RegisterBits(&PowerMode, 4, 1);
  return (msa301_bandwidth_t)bandwidthbits.read();
}

/**************************************************************************/
/*!
    @brief Set the resolution range: +-2g, 4g, 8g, or 16g.
    @param range Enumerated msa301_range_t
*/
/**************************************************************************/
void Adafruit_MSA301::setRange(msa301_range_t range) {
  Adafruit_BusIO_Register ResRange =
      Adafruit_BusIO_Register(i2c_dev, MSA301_REG_RESRANGE, 1);
  Adafruit_BusIO_RegisterBits rangebits =
      Adafruit_BusIO_RegisterBits(&ResRange, 2, 0);
  rangebits.write((uint8_t)range);
}

/**************************************************************************/
/*!
    @brief Read the resolution range: +-2g, 4g, 8g, or 16g.
    @returns Enumerated msa301_range_t
*/
/**************************************************************************/
msa301_range_t Adafruit_MSA301::getRange(void) {
  Adafruit_BusIO_Register ResRange =
      Adafruit_BusIO_Register(i2c_dev, MSA301_REG_RESRANGE, 1);
  Adafruit_BusIO_RegisterBits rangebits =
      Adafruit_BusIO_RegisterBits(&ResRange, 2, 0);
  return (msa301_range_t)rangebits.read();
}

/**************************************************************************/
/*!
    @brief Set the resolution - 8, 10, 12, or 14bits
    @param resolution Enumerated msa301_resolution_t
*/
/**************************************************************************/
void Adafruit_MSA301::setResolution(msa301_resolution_t resolution) {
  Adafruit_BusIO_Register ResRange =
      Adafruit_BusIO_Register(i2c_dev, MSA301_REG_RESRANGE, 1);
  Adafruit_BusIO_RegisterBits resbits =
      Adafruit_BusIO_RegisterBits(&ResRange, 2, 2);
  resbits.write((uint8_t)resolution);
}

/**************************************************************************/
/*!
    @brief Read the resolution - 8, 10, 12, or 14bits
    @returns Enumerated msa301_resolution_t
*/
/**************************************************************************/
msa301_resolution_t Adafruit_MSA301::getResolution(void) {
  Adafruit_BusIO_Register ResRange =
      Adafruit_BusIO_Register(i2c_dev, MSA301_REG_RESRANGE, 1);
  Adafruit_BusIO_RegisterBits resbits =
      Adafruit_BusIO_RegisterBits(&ResRange, 2, 2);
  return (msa301_resolution_t)resbits.read();
}

/**************************************************************************/
/*!
  @brief  Read the XYZ data from the accelerometer and store in the internal
  x, y and z (and x_g, y_g, z_g) member variables.
*/
/**************************************************************************/

void Adafruit_MSA301::read(void) {
  uint8_t buffer[6];

  Adafruit_BusIO_Register XYZDataReg =
      Adafruit_BusIO_Register(i2c_dev, MSA301_REG_OUT_X_L, 6);
  XYZDataReg.read(buffer, 6);
  x = buffer[0];
  x |= buffer[1] << 8;
  y = buffer[2];
  y |= buffer[3] << 8;
  z = buffer[4];
  z |= buffer[5] << 8;

  // 14 bits of data in 16 bit range
  x >>= 2;
  y >>= 2;
  z >>= 2;

  msa301_range_t range = getRange();
  float scale = 1;
  if (range == MSA301_RANGE_16_G)
    scale = 512;
  if (range == MSA301_RANGE_8_G)
    scale = 1024;
  if (range == MSA301_RANGE_4_G)
    scale = 2048;
  if (range == MSA301_RANGE_2_G)
    scale = 4096;

  x_g = (float)x / scale;
  y_g = (float)y / scale;
  z_g = (float)z / scale;
}

/**************************************************************************/
/*!
  @brief  Set the click detection register thresholds
  @param  tap_quiet TAP_QUIET flag (check datasheet for details)
  @param  tap_shock TAP_SHOCK flag (check datasheet for details)
  @param  tapduration How long to listen for a second tap (check datasheet for
  details)
  @param  tapthresh How strong the tap signal has to be (check datasheet for
  details)
*/
/**************************************************************************/

void Adafruit_MSA301::setClick(bool tap_quiet, bool tap_shock,
                               msa301_tapduration_t tapduration,
                               uint8_t tapthresh) {
  Adafruit_BusIO_Register TapDur =
      Adafruit_BusIO_Register(i2c_dev, MSA301_REG_TAPDUR, 1);
  Adafruit_BusIO_RegisterBits quietbit =
      Adafruit_BusIO_RegisterBits(&TapDur, 1, 7);
  quietbit.write(tap_quiet);
  Adafruit_BusIO_RegisterBits shockbit =
      Adafruit_BusIO_RegisterBits(&TapDur, 1, 6);
  shockbit.write(tap_shock);
  Adafruit_BusIO_RegisterBits durationbits =
      Adafruit_BusIO_RegisterBits(&TapDur, 3, 0);
  durationbits.write(tapduration);

  Adafruit_BusIO_Register TapTh =
      Adafruit_BusIO_Register(i2c_dev, MSA301_REG_TAPTH, 1);
  Adafruit_BusIO_RegisterBits threshbits =
      Adafruit_BusIO_RegisterBits(&TapTh, 5, 0);
  threshbits.write(tapthresh);
}

/**************************************************************************/
/*!
    @brief  Gets the most recent click detect status register value
    @returns 8 bits of interrupt status, check datasheet for what CLICKSTATUS
   bits are
*/
/**************************************************************************/
uint8_t Adafruit_MSA301::getClick(void) {
  Adafruit_BusIO_Register ClickStatus =
      Adafruit_BusIO_Register(i2c_dev, MSA301_REG_CLICKSTATUS, 1);

  return ClickStatus.read();
}

/**************************************************************************/
/*!
    @brief  Set which interrupts are enabled
    @param singletap Whether to trigger INT on single tap interrupt
    @param doubletap Whether to trigger INT on double tap interrupt
    @param activeX Whether to trigger INT on X axis activity interrupt
    @param activeY Whether to trigger INT on Y axis activity interrupt
    @param activeZ Whether to trigger INT on Z axis activity interrupt
    @param newData Whether to trigger INT on new data available interrupt
    @param freefall Whether to trigger INT on freefall interrupt
    @param orient Whether to trigger INT on orientation interrupt
*/
/**************************************************************************/
void Adafruit_MSA301::enableInterrupts(bool singletap, bool doubletap,
                                       bool activeX, bool activeY, bool activeZ,
                                       bool newData, bool freefall,
                                       bool orient) {
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

/**************************************************************************/
/*!
    @brief  Set which interrupts are mapped to the INT pin
    @param singletap Whether to trigger INT on single tap interrupt
    @param doubletap Whether to trigger INT on double tap interrupt
    @param activity Whether to trigger INT on activity interrupt
    @param newData Whether to trigger INT on new data available interrupt
    @param freefall Whether to trigger INT on freefall interrupt
    @param orient Whether to trigger INT on orientation interrupt
*/
/**************************************************************************/

void Adafruit_MSA301::mapInterruptPin(bool singletap, bool doubletap,
                                      bool activity, bool newData,
                                      bool freefall, bool orient) {
  Adafruit_BusIO_Register IntMap0 =
      Adafruit_BusIO_Register(i2c_dev, MSA301_REG_INTMAP0, 1);
  Adafruit_BusIO_Register IntMap1 =
      Adafruit_BusIO_Register(i2c_dev, MSA301_REG_INTMAP1, 1);
  uint8_t irqs = 0;
  irqs |= orient << 6;
  irqs |= singletap << 5;
  irqs |= doubletap << 4;
  irqs |= activity << 2;
  irqs |= freefall << 0;
  IntMap0.write(irqs);
  irqs = newData << 0;
  IntMap1.write(irqs);
}

/**************************************************************************/
/*!
    @brief  Gets the most recent motion interrupt status register value
    @returns 8 bits of interrupt status, check datasheet for what MOTION bits
   are
*/
/**************************************************************************/

uint8_t Adafruit_MSA301::getMotionInterruptStatus(void) {
  Adafruit_BusIO_Register IntStatus =
      Adafruit_BusIO_Register(i2c_dev, MSA301_REG_MOTIONINT, 1);

  return IntStatus.read();
}

/**************************************************************************/
/*!
    @brief  Gets the most recent data interrupt status register value
    @returns 8 bits of interrupt status, check datasheet for what DATAINT bits
   are
*/
/**************************************************************************/

uint8_t Adafruit_MSA301::getDataInterruptStatus(void) {
  Adafruit_BusIO_Register IntStatus =
      Adafruit_BusIO_Register(i2c_dev, MSA301_REG_DATAINT, 1);

  return IntStatus.read();
}

/**************************************************************************/
/*!
    @brief  Gets the most recent sensor event, Adafruit Unified Sensor format
    @param  event Pointer to an Adafruit Unified sensor_event_t object that
   we'll fill in
    @returns True on successful read
*/
/**************************************************************************/
bool Adafruit_MSA301::getEvent(sensors_event_t *event) {
  /* Clear the event */
  memset(event, 0, sizeof(sensors_event_t));

  event->version = sizeof(sensors_event_t);
  event->sensor_id = _sensorID;
  event->type = SENSOR_TYPE_ACCELEROMETER;
  event->timestamp = 0;

  read();

  event->acceleration.x = x_g * SENSORS_GRAVITY_STANDARD;
  event->acceleration.y = y_g * SENSORS_GRAVITY_STANDARD;
  event->acceleration.z = z_g * SENSORS_GRAVITY_STANDARD;

  return true;
}

/**************************************************************************/
/*!
    @brief  Gets the sensor_t device data, Adafruit Unified Sensor format
    @param  sensor Pointer to an Adafruit Unified sensor_t object that we'll
   fill in
*/
/**************************************************************************/
void Adafruit_MSA301::getSensor(sensor_t *sensor) {
  /* Clear the sensor_t object */
  memset(sensor, 0, sizeof(sensor_t));

  /* Insert the sensor name in the fixed length char array */
  strncpy(sensor->name, "MSA301", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name) - 1] = 0;
  sensor->version = 1;
  sensor->sensor_id = _sensorID;
  sensor->type = SENSOR_TYPE_ACCELEROMETER;
  sensor->min_delay = 0;
  sensor->max_value = 0;
  sensor->min_value = 0;
  sensor->resolution = 0;
}
