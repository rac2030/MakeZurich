#ifndef SDP3X_H
#define SDP3X_H

// convert two 8 bit values to one word
#define BIU16(data, start) (((uint16_t)(data)[start]) << 8 | ((data)[start + 1]))

// SDP3x sensor I2C address
#define SDP_1_ADDRESS 0x21

typedef enum {
   SDP_MEASUREMENT_COMMAND_0 = 0x36,
   SDP_MEASUREMENT_COMMAND_1 = 0x2F
} SDP3xCommands;

class SDP3xClass
{
  private:
  int16_t readSensor(void);

  public:
    float GetPressureDiff(void);
};

extern SDP3xClass SDP3x;

#endif


