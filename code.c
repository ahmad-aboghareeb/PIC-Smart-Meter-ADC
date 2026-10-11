#include <16F877A.h>
#device ADC=10
#use delay(crystal=16000000)


#define LCD_RS PIN_D0
#define LCD_RW PIN_D1
#define LCD_E  PIN_D2
#define LCD_D4 PIN_D4
#define LCD_D5 PIN_D5
#define LCD_D6 PIN_D6
#define LCD_D7 PIN_D7


void lcd_pulse_enable() {
    output_high(LCD_E);
    delay_us(2);
    output_low(LCD_E);
    delay_us(50);
}

void lcd_send_nibble(int8 nibble) {
    
    output_low(LCD_D4);
    output_low(LCD_D5);
    output_low(LCD_D6);
    output_low(LCD_D7);
    
    
    if(bit_test(nibble, 0)) output_high(LCD_D4); 
    if(bit_test(nibble, 1)) output_high(LCD_D5); 
    if(bit_test(nibble, 2)) output_high(LCD_D6); 
    if(bit_test(nibble, 3)) output_high(LCD_D7); 
    
    lcd_pulse_enable();
}

void lcd_send_byte(int8 address, int8 data) {
    if(address) output_high(LCD_RS); else output_low(LCD_RS);
    output_low(LCD_RW); 
    lcd_send_nibble(data >> 4);
    lcd_send_nibble(data & 0x0F);
    if(address == 0) delay_ms(2); 
}

void lcd_init_custom() {
    output_low(LCD_RS);
    output_low(LCD_RW);
    output_low(LCD_E);
    delay_ms(50);

    
    lcd_send_nibble(0x03); 
    delay_ms(5);           
    lcd_send_nibble(0x03); 
    delay_us(150);        
    lcd_send_nibble(0x03); 
    delay_us(150);
    lcd_send_nibble(0x02); 
    delay_us(150);

    // Configure display
    lcd_send_byte(0, 0x28);
    lcd_send_byte(0, 0x08); 
    lcd_send_byte(0, 0x01); 
    delay_ms(2);           
    lcd_send_byte(0, 0x06);
    lcd_send_byte(0, 0x0C); 
}

void lcd_print(char* str) {
    while(*str) {
        lcd_send_byte(1, *str++);
    }
}
// -----------------------------------

void main() {
    int16 raw_v, raw_i;
    int32 voltage, current, power;
    char buffer[32]; 

    lcd_init_custom();
    
    setup_adc_ports(ALL_ANALOG); 
    setup_adc(ADC_CLOCK_DIV_32); 

    while(TRUE) {
        set_adc_channel(0);
        delay_us(50); 
        raw_v = read_adc();

        set_adc_channel(1);
        delay_us(50); 
        raw_i = read_adc();

        voltage = (int32)raw_v * 250 / 1023;
        current = (int32)raw_i * 50 / 1023;
        power = voltage * current;

       
        lcd_send_byte(0, 0x01);
        sprintf(buffer, "V:%LuV I:%LuA", voltage, current);
        lcd_print(buffer);
        
        
        lcd_send_byte(0, 0xC0);
        sprintf(buffer, "P:%Lu Watts", power);
        lcd_print(buffer);
        
        delay_ms(500);
    }
}
