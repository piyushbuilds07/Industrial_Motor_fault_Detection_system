#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

// ================= UART =================
void UART_init()
{
    UCSR0A = (1<<U2X0);
    UBRR0H = 0;
    UBRR0L = 103;

    UCSR0B = (1<<TXEN0);
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

void UART_send(char c)
{
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = c;
}

void UART_string(char *str)
{
    while(*str) UART_send(*str++);
}

void UART_print_num(uint16_t num)
{
    UART_send((num/100)+'0');
    UART_send(((num/10)%10)+'0');
    UART_send((num%10)+'0');
}

// ================= ADC =================
void ADC_init()
{
    ADMUX = (1<<REFS0);
    ADCSRA = (1<<ADEN)|(1<<ADPS1)|(1<<ADPS0);
}

uint16_t ADC_read(uint8_t ch)
{
    ADMUX = (ADMUX & 0xF0) | ch;
    ADCSRA |= (1<<ADSC);
    while(ADCSRA & (1<<ADSC));
    return ADC;
}

// ================= BUZZER =================
void buzzer_fast()
{
    PORTB |= (1<<PB0);
    _delay_ms(100);
    PORTB &= ~(1<<PB0);
    _delay_ms(100);
}

void buzzer_slow()
{
    PORTB |= (1<<PB0);
    _delay_ms(300);
    PORTB &= ~(1<<PB0);
    _delay_ms(300);
}

// ================= MAIN =================
int main()
{
    UART_init();
    ADC_init();

    // PB0 = buzzer, PB1 = LED, PB2 = relay
    DDRB |= (1<<PB0)|(1<<PB1)|(1<<PB2);

    float offset = 2.48;   // ?? CALIBRATE THIS

    while(1)
    {
        // ===== READ ADC =====
        uint16_t adc_temp = ADC_read(0);
        uint16_t adc_curr = ADC_read(1);

        // ===== TEMPERATURE =====
        float v_temp = adc_temp * 5.0 / 1023.0;
        uint16_t temp = v_temp * 100;

        // ===== CURRENT =====
        float v_curr = adc_curr * 5.0 / 1023.0;
        float current = (v_curr - offset)/0.185;

        if(current < 0) current = -current;

        uint16_t curr_int = (uint16_t)current;

        // ===== DISPLAY =====
        UART_string("T=");
        UART_print_num(temp);
        UART_string("C ");

        UART_string("I=");
        UART_print_num(curr_int);
        UART_string("A ");

        // ===== FAULT LOGIC =====
        if(current > 3.0)
        {
            PORTB &= ~(1<<PB2);   // relay OFF
            PORTB |= (1<<PB1);    // LED ON

            UART_string("FAULT: OVERLOAD\r\n");

            buzzer_fast();        // ? fast beep
        }
        else if(temp > 60)
        {
            PORTB &= ~(1<<PB2);
            PORTB |= (1<<PB1);

            UART_string("FAULT: OVERHEAT\r\n");

            buzzer_slow();        // ? slow beep
        }
        else
        {
            PORTB |= (1<<PB2);    // relay ON
            PORTB &= ~(1<<PB1);   // LED OFF
            PORTB &= ~(1<<PB0);   // buzzer OFF

            UART_string("STATUS: NORMAL\r\n");

            _delay_ms(500);
        }
    }
}