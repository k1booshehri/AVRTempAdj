#include <mega32.h>
#include <alcd.h>
#include <delay.h>
int period;
int i;
int temprature;
int state;
int firstdelay;

unsigned char int_to_char(int x) {
	return x + '0';
}

void put_on_lcd(int value, int c) {
	unsigned int r = 1;
	unsigned char tens = int_to_char(value / 10);
	unsigned char ones = int_to_char(value % 10);
	lcd_gotoxy(c, r);
	lcd_putchar(tens);
	lcd_gotoxy(c + 1, r);
	lcd_putchar(ones);
}

// Voltage Reference: AVCC pin
#define ADC_VREF_TYPE ((0<<REFS1) | (1<<REFS0) | (0<<ADLAR))

// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input) {
	ADMUX = adc_input | ADC_VREF_TYPE;
	// Delay needed for the stabilization of the ADC input voltage
	delay_us(10);
    // Start the AD conversion
    ADCSRA |= (1 << ADSC);
    // Wait for the AD conversion to complete
    while ((ADCSRA & (1 << ADIF)) == 0);
    ADCSRA |= (1 << ADIF);
    return ADCW;
}

void main(void) {
    DDRD = 0x0F;		/* Make PORTD lower pins as output */
    DDRB = 0x0F;
   	period = 20;		/* Set period in between two steps */
    
    // ADC initialization
    // ADC Clock frequency: 250.000 kHz
    // ADC Voltage Reference: AVCC pin
    // ADC Auto Trigger Source: ADC Stopped
    ADMUX |= (1<<MUX0);
    //ADMUX=ADC_VREF_TYPE;
    ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (1<<ADPS2) | (0<<ADPS1) | (1<<ADPS0);
    SFIOR=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);
    
    lcd_init(16);
    
    lcd_gotoxy(3, 0);
    lcd_puts("LM35 Sensor");
    
    lcd_gotoxy(2, 1);
    lcd_puts("Temp :");
    
    lcd_gotoxy(12, 1);
    lcd_putchar(223);
    
    lcd_gotoxy(13, 1);
    lcd_puts("C");
    
    state=1;
    firstdelay=1;
    
    while (1) {
        if(firstdelay==1){
        delay_ms(1000);
        firstdelay=0;
        }
        temprature = read_adc(0);
        temprature = temprature * 0.48898;
        put_on_lcd(temprature, 9);
        
        /* Rotate Stepper Motor clockwise with Half step sequence */
        if (temprature>35 && state==1)
			state=2;
		else if (temprature<15)
			state=3;
		else if (temprature>30 && state==3)
			state=1;
		else if (temprature<25 && state==2)
			state=1;
		if(state==2){
		
		for(i=0;i<12;i++)
		{
			PORTD = 0x09;
			delay_ms(period);
			PORTD = 0x08;
			delay_ms(period);
			PORTD = 0x0C;
			delay_ms(period);
			PORTD = 0x04;
			delay_ms(period);
			PORTD = 0x06;
			delay_ms(period);
			PORTD = 0x02;
			delay_ms(period);
			PORTD = 0x03;
			delay_ms(period);
			PORTD = 0x01;
			delay_ms(period);
		}
	}
		if (state==3){
		for(i=0;i<12;i++)
		{
			PORTB = 0x09;
			delay_ms(period);
			PORTB = 0x08;
			delay_ms(period);
			PORTB = 0x0C;
			delay_ms(period);
			PORTB = 0x04;
			delay_ms(period);
			PORTB = 0x06;
			delay_ms(period);
			PORTB = 0x02;
			delay_ms(period);
			PORTB = 0x03;
			delay_ms(period);
			PORTB = 0x01;
			delay_ms(period);
		}
	}
	
	
	
    }
}
