/**************************************************************************/
/*!
    @file     Adafruit_MSA301.h
    @author   Limor Fried (Adafruit Industries)
    @license  MIT (see license.txt)

    This is a library for the Adafruit MSA301 Accel breakout board
    ----> https://www.adafruit.com/

    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!

*/
/**************************************************************************/


#ifndef ADAFRUIT_MSA301_H
#define ADAFRUIT_MSA301_H

#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_BusIO_Register.h>

/*=========================================================================
I2C ADDRESS/BITS
-----------------------------------------------------------------------*/
#define MSA301_I2CADDR_DEFAULT  (0x26)
/*=========================================================================*/

#define MSA301_REG_PARTID        0x01
#define MSA301_REG_OUT_X_L       0x02
#define MSA301_REG_OUT_X_H       0x03
#define MSA301_REG_OUT_Y_L       0x04
#define MSA301_REG_OUT_Y_H       0x05
#define MSA301_REG_OUT_Z_L       0x06
#define MSA301_REG_OUT_Z_H       0x07
#define MSA301_REG_MOTIONINT     0x09
#define MSA301_REG_DATAINT       0x0A
#define MSA301_REG_CLICKSTATUS   0x0B
#define MSA301_REG_RESRANGE      0x0F
#define MSA301_REG_ODR           0x10
#define MSA301_REG_POWERMODE     0x11
#define MSA301_REG_INTSET0       0x16
#define MSA301_REG_INTSET1       0x17
#define MSA301_REG_INTMAP0       0x19
#define MSA301_REG_INTMAP1       0x1A
#define MSA301_REG_TAPDUR        0x2A
#define MSA301_REG_TAPTH         0x2B

typedef enum {
  MSA301_RANGE_2_G           = 0b00,   // +/- 2g (default value)
  MSA301_RANGE_4_G           = 0b01,   // +/- 4g
  MSA301_RANGE_8_G           = 0b10,   // +/- 8g
  MSA301_RANGE_16_G          = 0b11,   // +/- 16g
} msa301_range_t;

typedef enum {
  MSA301_AXIS_X         = 0x0,
  MSA301_AXIS_Y         = 0x1,
  MSA301_AXIS_Z         = 0x2,
} msa301_axis_t;


/* Used with register 0x10 (MSA301_REG_ODR) to set datarate */
typedef enum {
  MSA301_DATARATE_1_HZ       = 0b0000, // 1 Hz
  MSA301_DATARATE_1_95_HZ    = 0b0001, // 1.95 Hz
  MSA301_DATARATE_3_9_HZ     = 0b0010, // 3.9 Hz
  MSA301_DATARATE_7_81_HZ    = 0b0011, // 7.81 Hz
  MSA301_DATARATE_15_63_HZ   = 0b0100, // 15.63 Hz
  MSA301_DATARATE_31_25_HZ   = 0b0101, // 31.25 Hz
  MSA301_DATARATE_62_5_HZ    = 0b0110, // 62.5 Hz
  MSA301_DATARATE_125_HZ     = 0b0111, // 125 Hz
  MSA301_DATARATE_250_HZ     = 0b1000, // 250 Hz
  MSA301_DATARATE_500_HZ     = 0b1001, // 500 Hz
  MSA301_DATARATE_1000_HZ    = 0b1010, // 1000 Hz
} msa301_dataRate_t;

typedef enum {
  MSA301_BANDWIDTH_1_95_HZ    = 0b0000, // 1.95 Hz
  MSA301_BANDWIDTH_3_9_HZ     = 0b0011, // 3.9 Hz
  MSA301_BANDWIDTH_7_81_HZ    = 0b0100, // 7.81 Hz
  MSA301_BANDWIDTH_15_63_HZ   = 0b0101, // 15.63 Hz
  MSA301_BANDWIDTH_31_25_HZ   = 0b0110, // 31.25 Hz
  MSA301_BANDWIDTH_62_5_HZ    = 0b0111, // 62.5 Hz
  MSA301_BANDWIDTH_125_HZ     = 0b1000, // 125 Hz
  MSA301_BANDWIDTH_250_HZ     = 0b1001, // 250 Hz
  MSA301_BANDWIDTH_500_HZ     = 0b1010, // 500 Hz
} msa301_bandwidth_t;

typedef enum {
  MSA301_NORMALMODE    = 0b00,
  MSA301_LOWPOWERMODE  = 0b01,
  MSA301_SUSPENDMODE   = 0b010,
} msa301_powermode_t;

typedef enum {
  MSA301_RESOLUTION_14    = 0b00,
  MSA301_RESOLUTION_12    = 0b01,
  MSA301_RESOLUTION_10    = 0b10,
  MSA301_RESOLUTION_8     = 0b11,
} msa301_resolution_t;


typedef enum {
  MSA301_TAPDUR_50_MS     = 0b000,
  MSA301_TAPDUR_100_MS    = 0b001,
  MSA301_TAPDUR_150_MS    = 0b010,
  MSA301_TAPDUR_200_MS    = 0b011,
  MSA301_TAPDUR_250_MS    = 0b100,
  MSA301_TAPDUR_375_MS    = 0b101,
  MSA301_TAPDUR_500_MS    = 0b110,
  MSA301_TAPDUR_700_MS    = 0b111,
} msa301_tapduration_t;


typedef enum {
  MSA301_INT_ORIENT = 0b100000,
  MSA301_INT_SINGLETAP,
  MSA301_INT_DOUBLETAP,
  MSA301_INT_ACTIVETAP,
  MSA301_INT_NEWDATA,
} msa301_interrupt_t;

class Adafruit_MSA301 : public Adafruit_Sensor {
 public:
  Adafruit_MSA301(void);
  bool begin(uint8_t i2c_addr=MSA301_I2CADDR_DEFAULT, TwoWire *wire = &Wire);

  void setDataRate(msa301_dataRate_t dataRate);
  msa301_dataRate_t getDataRate(void);
  void enableAxes(bool x, bool y, bool z);

  void setPowerMode(msa301_powermode_t mode);
  msa301_powermode_t getPowerMode(void);
  void setBandwidth(msa301_bandwidth_t bandwidth);
  msa301_bandwidth_t getBandwidth(void);
  void setRange(msa301_range_t range);
  msa301_range_t getRange(void);
  void setResolution(msa301_resolution_t resolution);
  msa301_resolution_t getResolution(void);

  void read();
  bool getEvent(sensors_event_t *event);
  void getSensor(sensor_t *sensor);

  void enableInterrupts(bool singletap=false, bool doubletap=false,
			bool activeX=false, bool activeY=false, bool activeZ=false,
			bool newData=false, bool freefall=false, bool orient=false);
  void mapInterruptPin(bool singletap=false, bool doubletap=false, bool activity=false, 
		    bool newData=false, bool freefall=false, bool orient=false);
  
  uint8_t getClick(void);
  uint8_t getMotionInterruptStatus(void) ;
  uint8_t getDataInterruptStatus(void);

  void setClick(bool tap_quiet, bool tap_shock, msa301_tapduration_t tapduration, uint8_t tapthresh);

  /*

  uint8_t getOrientation(void);

  */


  int16_t x, y, z;
  float x_g, y_g, z_g;

 private:
  Adafruit_I2CDevice *i2c_dev;

  int32_t _sensorID;
};

#endif
