#define F_CPU 8000000UL // Use 16000000UL if your ATmega clock is 16MHz

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>

// ================= PIN DEFINITIONS =================
// PORTB
#define BUZZER PB0
#define RED_LED PB1
#define GREEN_LED PB2
#define BLUE_LED PB3
#define RELAY PB4

// PORTD
#define FAN PD6

// ================= THRESHOLDS =================
#define CURRENT_NORMAL 0.20
#define CURRENT_LIMIT 0.50
#define CURRENT_RESET 0.45

#define TEMP_FAN_ON 40
#define TEMP_FAN_OFF 38
#define TEMP_MOTOR_OFF 60
#define TEMP_RESET 55

// ACS712 5A sensitivity = 185mV/A = 0.185V/A
#define ACS712_OFFSET 2.50
#define ACS712_SENSITIVITY 0.185

// ================= UART =================
void UART_init(void)
{
    // 9600 baud
#if F_CPU == 8000000UL
    UBRR0H = 0;
    UBRR0L = 51;
#else
    UBRR0H = 0;
    UBRR0L = 103;
#endif

    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_send(char c)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    UDR0 = c;
}

void UART_string(const char *str)
{
    while (*str)
    {
        UART_send(*str++);
    }
}

void UART_float(float value)
{
    char buffer[10];
    dtostrf(value, 4, 2, buffer); // width 4, 2 decimal places
    UART_string(buffer);
}

// ================= ADC =================
void ADC_init(void)
{
    ADMUX = (1 << REFS0); // AVCC = 5V reference

    // Prescaler 64 for 8MHz, ADC clock = 125kHz
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
}

uint16_t ADC_read(uint8_t ch)
{
    ADMUX = (ADMUX & 0xF0) | (ch & 0x0F);
    ADCSRA |= (1 << ADSC);

    while (ADCSRA & (1 << ADSC))
        ;

    return ADC;
}

// ================= SENSOR READINGS =================
float read_temperature(void)
{
    uint16_t adc_temp = ADC_read(0); // ADC0 = LM35
    float v_temp = adc_temp * 5.0 / 1023.0;
    float temp = v_temp * 100.0; // LM35 = 10mV/°C
    return temp;
}

float read_current(void)
{
    uint16_t adc_curr = ADC_read(1); // ADC1 = ACS712 / POT
    float v_curr = adc_curr * 5.0 / 1023.0;

    float current = (v_curr - ACS712_OFFSET) / ACS712_SENSITIVITY;

    if (current < 0)
        current = -current;

    return current;
}

// ================= OUTPUT FUNCTIONS =================
void motor_on(void)
{
    PORTB |= (1 << RELAY);
}

void motor_off(void)
{
    PORTB &= ~(1 << RELAY);
}

void fan_on(void)
{
    PORTD |= (1 << FAN);
}

void fan_off(void)
{
    PORTD &= ~(1 << FAN);
}

void buzzer_on(void)
{
    PORTB |= (1 << BUZZER);
}

void buzzer_off(void)
{
    PORTB &= ~(1 << BUZZER);
}

void set_normal_led(void)
{
    PORTB &= ~((1 << RED_LED) | (1 << BLUE_LED));
    PORTB |= (1 << GREEN_LED);
}

void set_warning_led(void)
{
    PORTB &= ~((1 << GREEN_LED) | (1 << RED_LED));
    PORTB |= (1 << BLUE_LED); // BLUE = temperature warning / fan ON
}

void set_fault_led(void)
{
    PORTB &= ~((1 << GREEN_LED) | (1 << BLUE_LED));
    PORTB |= (1 << RED_LED);
}

// ================= MAIN =================
int main(void)
{
    float temp = 0;
    float current = 0;
    uint8_t fault = 0;

    UART_init();
    ADC_init();

    // PB0 buzzer, PB1 RED, PB2 GREEN, PB3 BLUE, PB4 relay
    DDRB |= (1 << BUZZER) | (1 << RED_LED) | (1 << GREEN_LED) | (1 << BLUE_LED) | (1 << RELAY);

    // PD6 fan
    DDRD |= (1 << FAN);

    motor_on();
    fan_off();
    buzzer_off();
    set_normal_led();

    UART_string("SYSTEM STARTED\r\n");

    while (1)
    {
        temp = read_temperature();
        current = read_current();

        UART_string("T=");
        UART_float(temp);
        UART_string(" C  I=");
        UART_float(current);
        UART_string(" A  ");

        // ========== FAULT CONDITION ==========
        if (current >= CURRENT_LIMIT)
        {
            fault = 1;

            motor_off();
            fan_on();
            buzzer_on();
            set_fault_led();

            UART_string("OVERLOAD - MOTOR OFF\r\n");
        }
        else if (temp >= TEMP_MOTOR_OFF)
        {
            fault = 1;

            motor_off();
            fan_on();
            buzzer_on();
            set_fault_led();

            UART_string("OVERHEAT - MOTOR OFF\r\n");
        }

        // ========== RESET CONDITION ==========
        else if (fault == 1 && current < CURRENT_RESET && temp < TEMP_RESET)
        {
            fault = 0;

            motor_on();
            buzzer_off();
            fan_off();
            set_normal_led();

            UART_string("RESET - NORMAL\r\n");
        }

        // ========== NORMAL / WARNING ==========
        else if (fault == 0)
        {
            motor_on();
            buzzer_off();

            if (temp >= TEMP_FAN_ON)
            {
                fan_on();
                set_warning_led();
                UART_string("WARNING - FAN ON\r\n");
            }
            else if (temp <= TEMP_FAN_OFF)
            {
                fan_off();
                set_normal_led();
                UART_string("NORMAL\r\n");
            }
            else
            {
                UART_string("NORMAL\r\n");
            }
        }

        _delay_ms(500);
    }
}