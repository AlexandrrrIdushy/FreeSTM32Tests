#include "stm32f4xx_hal.h"
#include <libopencm3/stm32/rcc.h>

static void i2c_slave_init(uint8_t ownaddress)
{
   rcc_periph_clock_enable(RCC_GPIOB);
   rcc_periph_clock_enable(RCC_I2C1);

   nvic_enable_irq(NVIC_I2C1_EV_IRQ);

   // configure i2c pins
   gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
                 GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO_I2C1_SDA); //PB7
   gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
                 GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO_I2C1_SCL); //PB6

   i2c_reset(I2C1);
   i2c_peripheral_disable(I2C1);

   i2c_set_speed(I2C1, i2c_speed_sm_100k, I2C_CR2_FREQ_36MHZ);
   i2c_set_own_7bit_slave_address(I2C1, ownaddress);
   i2c_enable_interrupt(I2C1, I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN);
   i2c_peripheral_enable(I2C1);

   // slave needs to acknowledge on receiving bytes
   // set it after enabling Peripheral i.e. PE = 1
   i2c_enable_ack(I2C1);
}
