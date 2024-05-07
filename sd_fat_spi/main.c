#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

// registers
static const uint8_t REG_DEVID = 0x00;
static const uint8_t REG_POWER_CTL = 0x2D;
static const uint8_t REG_DATAX0 = 0x32;

// other constants
static const uint8_t DEVID = 0xE5;
static const float SENSITIVITY_2G = 1.0 / 256;  // (g/LSB)
static const float EARTH_GRAVITY = 9.80665;     // Earth's gravity (in m/s^2)

// function prototypes
void reg_write(spi_inst_t *spi, const uint cs, const uint8_t reg, const uint8_t data);
int reg_read(spi_inst_t *spi, const uint cs, const uint8_t reg, uint8_t *buf, uint8_t nbytes);

// functions
void reg_write(spi_inst_t *spi, const uint cs, const uint8_t reg, const uint8_t data) {
  uint8_t msg[2];
  
  // construct message (set ~W bit low, MB bit low)
  msg[0] = 0x00 | reg;
  msg[1] = data;
  
  // write to register
  gpio_put(cs, 0);
  spi_write_blocking(spi, msg, 2);
  gpio_put(cs, 1);
}

int reg_read(spi_inst_t *spi, const uint cs, const uint8_t reg, uint8_t *buf, uint8_t nbytes) {
  int num_bytes_read = 0;
  uint8_t mb = 0;
  
  // determine if multiple byte (MB) bit should be set
  if (nbytes < 1) {
    return -1;
  } else if (nbytes == 1) {
    mb = 0;
  } else {
    mb = 1;
  }

  // construct message (set ~W bit high)
  uint8_t msg = 0x80 | (mb << 6) | reg;
  
  // read from register
  gpio_put(cs, 0);
  spi_write_blocking(spi, &msg, 1);
  num_bytes_read = spi_read_blocking(spi, 0, buf, nbytes);
  gpio_put(cs, 1);
  
  return num_bytes_read;
}

int main() {
  int16_t acc_x;
  int16_t acc_y;
  int16_t acc_z;
  float acc_x_f;
  float acc_y_f;
  float acc_z_f;

  // pins
  const uint cs_pin = 17;
  const uint sck_pin = 18;
  const uint mosi_pin = 19;
  const uint miso_pin = 16;
  
  // buffer to store raw reads
  uint8_t data[6];
  
  // ports
  spi_inst_t *spi = spi0;
  
  // initialize chosen serial port
  stdio_init_all();
  
  // initialize CS pin high
  gpio_init(cs_pin);
  gpio_set_dir(cs_pin, GPIO_OUT);
  gpio_put(cs_pin, 1);
  
  // initialize SPI port at 1 MHz
  spi_init(spi, 1000 * 1000);
  
  // set SPI format
  // instance, bits per xfer, polarity (CPOL), phase (CPHA)
  spi_set_format(spi0, 8, 1, 1, SPI_MSB_FIRST);
  
  // initialize SPI pins
  gpio_set_function(sck_pin, GPIO_FUNC_SPI);
  gpio_set_function(mosi_pin, GPIO_FUNC_SPI);
  gpio_set_function(miso_pin, GPIO_FUNC_SPI);
  
  // workaround: perform throwaway read to make SCK idle high
  reg_read(spi, cs_pin, REG_DEVID, data, 1);
  
  // read device ID to make sure we can communicate
  reg_read(spi, cs_pin, REG_DEVID, data, 1);
  if (data[0] != DEVID) {
    printf("ERROR: could not communicate with sensor!\r\n");
    while(true);
  }
  
  // read power control register
  reg_read(spi, cs_pin, REG_POWER_CTL, data, 1);
  printf("0x%02X\r\n", data[0]);
  
  // start taking measurements
  data[0] |= (1 << 3);
  reg_write(spi, cs_pin, REG_POWER_CTL, data[0]);
  
  // read it back to make sure it is set
  reg_read(spi, cs_pin, REG_POWER_CTL, data, 1);
  printf("0x%02X\r\n", data[0]);

  // pause before taking measurements
  sleep_ms(2000);
  
  // loop forever
  while(true) {
    // read X, Y and Z values from registers (16 bits each)
    reg_read(spi, cs_pin, REG_DATAX0, data, 6);
    
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
    
    sleep_ms(100);
  }
}
