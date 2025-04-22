#include <stdio.h>
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define I2C_MASTER_NUM              I2C_NUM_0
#define I2C_MASTER_SCL_IO           22
#define I2C_MASTER_SDA_IO           21
#define I2C_MASTER_FREQ_HZ          50000
#define I2C_MASTER_TX_BUF_DISABLE   0
#define I2C_MASTER_RX_BUF_DISABLE   0

#define LCD_ADDR                    0x27

#define LCD_BACKLIGHT               0x08
#define LCD_ENABLE_BIT              0x04
#define LCD_RS_BIT                  0x01

void i2c_master_init() {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode,
                       I2C_MASTER_RX_BUF_DISABLE,
                       I2C_MASTER_TX_BUF_DISABLE, 0);
}

void lcd_write_nibble(uint8_t nibble, uint8_t rs) {
    uint8_t data = (nibble << 4) | LCD_BACKLIGHT;
    if (rs) data |= LCD_RS_BIT;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (LCD_ADDR << 1) | I2C_MASTER_WRITE, true);

    i2c_master_write_byte(cmd, data | LCD_ENABLE_BIT, true);   
    i2c_master_write_byte(cmd, data & ~LCD_ENABLE_BIT, true); 

    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(50));
    i2c_cmd_link_delete(cmd);
}

void lcd_send_byte(uint8_t byte, uint8_t rs) {
    lcd_write_nibble(byte >> 4, rs);    
    lcd_write_nibble(byte & 0x0F, rs);  
}

void lcd_command(uint8_t cmd) {
    lcd_send_byte(cmd, 0);  
    vTaskDelay(pdMS_TO_TICKS(2));
}

void lcd_data(uint8_t data) {
    lcd_send_byte(data, 1);
    vTaskDelay(pdMS_TO_TICKS(2));
}

void lcd_init() {
    lcd_command(0x28); 
    lcd_command(0x0C); 
    lcd_command(0x06);
    lcd_command(0x01); 
    vTaskDelay(pdMS_TO_TICKS(10));
}

void lcd_puts(char *str) {
    while (*str) {
        lcd_data((uint8_t)*str++);
    }
}

void app_main() {
    i2c_master_init();
    lcd_init();

    lcd_puts("Sist. Dig. 1");
}
