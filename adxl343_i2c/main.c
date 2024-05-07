#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// I2C address
static const uint8_t ADXL345_ADDR = 0x53;

// Registers
static const uint8_t REG_DEVID = 0x00;
static const uint8_t REG_POWER_CTL = 0x2D;
static const uint8_t REG_DATAX0 = 0x32;

// Other constants
static const uint8_t DEVID = 0xE5;
static const float SENSITIVITY_2G = 1.0 / 256;  // (g/LSB)
static const float EARTH_GRAVITY = 9.80665;     // in m/s^2

int reg_write(i2c_inst_t *i2c, const uint addr, const uint8_t reg, uint8_t *buf, const uint8_t nbytes);
int reg_read(i2c_inst_t *i2c, const uint addr, const uint8_t reg, uint8_t *buf, const uint8_t nbytes);

int main() {
  int16_t acc_x;
  int16_t acc_y;
  int16_t acc_z;
  float acc_x_f;
  float acc_y_f;
  float acc_z_f;
  
  // pins
  const uint sda_pin = 16;
  const uint scl_pin = 17;
  
  // ports
  i2c_inst_t *i2c = i2c0;
  
  // buffer to store raw reads
  uint8_t data[6];
  
  // initialize chosen serial port
  stdio_init_all();
  
  // initialize i2c port at 400 kHz
  i2c_init(i2c, 400 * 1000);
  
  // initialize i2c pins
  gpio_set_function(sda_pin, GPIO_FUNC_I2C);
  gpio_set_function(scl_pin, GPIO_FUNC_I2C);

  // read device ID, make sure it is as expected
  reg_read(i2c, ADXL345_ADDR, REG_DEVID, data, 1);
  if (data[0] != DEVID) {
    printf("ERROR: could not communicate with ADXL345! (Or received unexpected device ID)\r\n(expecting 0x%02X, got 0x%02X)\r\n", DEVID, data[0]);
    while(true);
  }
  
  // read power control register
  reg_read(i2c, ADXL345_ADDR, REG_POWER_CTL, data, 1);
  printf("PCR before setup: 0x%02X\r\n", data[0]);

  // tell ADXL345 to start taking measurements
  data[0] |= (1 << 3);
  reg_write(i2c, ADXL345_ADDR, REG_POWER_CTL, &data[0], 1);
  
  // read back power control register
  reg_read(i2c, ADXL345_ADDR, REG_POWER_CTL, data, 1);
  printf("PCR after setup:  0x%02X\r\n", data[0]);

  // wait a bit before taking measurements
  sleep_ms(2000);
  
  // take measurements forever
  while(true) {
    // read X, Y and Z values from registers (16 bits each)
    reg_read(i2c, ADXL345_ADDR, REG_DATAX0, data, 6);
    
    // convert 2 bytes (little-endian) into 16-bit integer (signed)
    acc_x = (int16_t)((data[1] << 8) | data[0]);
    acc_y = (int16_t)((data[3] << 8) | data[2]);
    acc_z = (int16_t)((data[5] << 8) | data[4]);
    
    // convert measurements to m/s^2
    acc_x_f = acc_x * SENSITIVITY_2G * EARTH_GRAVITY;
    acc_y_f = acc_y * SENSITIVITY_2G * EARTH_GRAVITY;
    acc_z_f = acc_z * SENSITIVITY_2G * EARTH_GRAVITY;
    
    // print results
    printf("X: %.2f | Y: %.2f | Z: %.2f\r\n", acc_x_f, acc_y_f, acc_z_f);
    
    // short delay between measurements
    sleep_ms(100);
  }
}

int reg_write(i2c_inst_t *i2c, const uint addr, const uint8_t reg, uint8_t *buf, const uint8_t nbytes)
{
  int num_bytes_read = 0;
  uint8_t msg[nbytes + 1];
  
  // check to make sure caller is sending 1 or more bytes
  if (nbytes < 1) {
    return 0;
  }
  
  // append register address to front of data packet
  msg[0] = reg;
  
  // construct data packet
  for (int i = 0; i < nbytes; i++) {
    msg[i+1] = buf[i];
  }
  
  // write data to registe(s) over i2c
  i2c_write_blocking(i2c, addr, msg, (nbytes+1), false);
  
  return num_bytes_read;
}

int reg_read(i2c_inst_t *i2c, const uint addr, const uint8_t reg, uint8_t *buf, const uint8_t nbytes)
{
  int num_bytes_read = 0;
  
  // check to make sure caller is asking for 1 or more bytes
  if (nbytes < 1) {
    return 0;
  }
  
  // read data from register(s) over i2c
  i2c_write_blocking(i2c, addr, &reg, 1, true);
  num_bytes_read = i2c_read_blocking(i2c, addr, buf, nbytes, false);
  
  return num_bytes_read;
}
