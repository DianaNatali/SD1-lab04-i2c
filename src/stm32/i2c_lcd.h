#ifndef I2C_LCD_H
#define I2C_LCD_H

void i2c_init(void);
void lcd_init(void);
void lcd_command(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_puts(char *str);
void lcd_clear(void);


#endif
