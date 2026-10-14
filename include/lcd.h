#ifndef SPEED_TRAP_LCD_INTERFACE_H
#define SPEED_TRAP_LCD_INTERFACE_H

/* Interface used by the application; the LCD driver is supplied separately. */
void lcd_init(void);
void lcd_moveto(unsigned char row, unsigned char column);
void lcd_stringout(char *text);
void lcd_writecommand(unsigned char command);
void lcd_writedata(unsigned char data);

#endif
