#define F_CPU 16000000UL
#define BPS 9600
#define BaudRate ((F_CPU)/(BPS*16UL)-1)
#include <avr/io.h>
#include <util/delay.h>
#define LOLUL 

char keypad[4][4] ={{'1','2','3','A'},
					{'4','5','6','B'},
					{'7','8','9','C'},
					{'*','0','#','D'}};



//some rows have less than 14 columns, might affect the functionality of the
char keyboard[6][14] = { {'ESC', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 'BKSPC' },
                      {'TAB', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\ '       },
                      {'CAPS', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', ' quote ', 'ENTR'   },
                      {'LSHIFT', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 'RSHIFT'          },
                      {'LCTRL', 'WIN', 'LALT',     ' '     ,'RALT', 'MENU', 'RCTRL', 'FN'            },
                      {'NA', 'NA', 'NA', '{', '(', '<', '>', ')', '}', 'NA', 'NA', 'NA'              } };

void usart_init(){
	UBRR0H = (BaudRate>>8); //shifting the baud rate to the right 8 bits gives us the leftmost 8 bits and stores them in UBRR0H
	UBRR0L = BaudRate;  //gets the first 8 bits from the baud rate and stores them in UBRR0L
	UCSR0B |= (1<<TXEN0) | (1<<RXEN0);  //enables both transmission and reception
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01); //sets it to 8-bit data format
	//USBC is already 0, which corresponds to 1 stop bit
}

void usart_transmit(char data){
	while(!(UCSR0A & (1<<UDRE0))); //checks if buffer is empty and then loads it with data
	UDR0 = data;
}

//sets rows to outputs and columns to inputs and enables pull-up resistors on input.
void keypad_init(){
	//rows (output)
	DDRF |= (1<<DDF0);
    DDRF |= (1<<DDF1);
    DDRF |= (1<<DDF4);
    DDRF |= (1<<DDF5);
    DDRF |= (1<<DDF6);
    DDRF |= (1<<DDF7);

	//columns (input)
	DDRB &= ~(1<<DDB0);
	DDRB &= ~(1<<DDB1);
	DDRB &= ~(1<<DDB2);
	DDRB &= ~(1<<DDB3);
    DDRB &= ~(1<<DDB4);
    DDRB &= ~(1<<DDB5);
    DDRB &= ~(1<<DDB6);
    DDRC &= ~(1<<DDC6);
    DDRC &= ~(1<<DDC7);
    DDRD &= ~(1<<DDD0);
    DDRD &= ~(1<<DDD1);
    DDRD &= ~(1<<DDD2);
    DDRD &= ~(1<<DDD3);
    DDRD &= ~(1<<DDD5);
	//enables pull-up resistors
	PORTB |= (1<<PORTB0);
	PORTB |= (1<<PORTB1);
	PORTB |= (1<<PORTB2);
	PORTB |= (1<<PORTB3);
    PORTB |= (1<<PORTB4);
    PORTB |= (1<<PORTB5);
    PORTB |= (1<<PORTB6);
    PORTC |= (1<<PORTC6);
    PORTC |= (1<<PORTC7);
    PORTD |= (1<<PORTD0);
    PORTD |= (1<<PORTD1);
    PORTD |= (1<<PORTD2);
    PORTD |= (1<<PORTD3);
    PORTD |= (1<<PORTD5);
}

//scans which key is being pressed
char keypad_scan(){
	//setting all the rows to 1
	PORTD |= (1<<PORTD4);
	PORTD |= (1<<PORTD5);
	PORTD |= (1<<PORTD6);
	PORTD |= (1<<PORTD7);
	
	uint8_t i = 0;
	uint8_t j = 0;
	for(i = 0; i < 4; i++){
		//setting a row to 0
		PORTD &= ~(1<< (i+4) ); //(i+4) to get the correct pin(s) on the AVR.
		_delay_us(5);
		for(j = 0; j < 4; j++){
			if( (PINB & (1<<j)) == 0 )
			return keypad[i][j];
		}
		//set the row back to 1 and move on to the next row
		PORTD |= (1<< (i+4) );
	}
	return 0;		//If nothing is found, return 0
}

int main(void){
	usart_init();
	keypad_init();
	char data;

	while (1){
		data = keypad_scan();
		if(data != 0){
			usart_transmit(data);
			_delay_ms(500);			//to avoid debouncing
		}
	}
	return 0;
}
