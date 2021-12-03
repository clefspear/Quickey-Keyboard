#include <Keyboard.h>

/*char keyboard[6][14] = { {'E', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 'B' },
                      {'T', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 'j'       },
                      {'C', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', 'q', 'E'   },
                      {'L', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 'R'          },
                      {'L', 'n', 'L',     ' '     ,'R', 'M', 'R', 'F'            },
                      {'N', 'A', 'A', '{', '(', '<', '>', ')', '}', 'N', 'N', 'A'              } };
                      

*/


//try making last row without 0x00 or add 0x00 to the rows below 14 columns and check results
int keyboard[6][14] = {
                      {0xB1, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x2D, 0x3D, 0xB2},
                      {0xB3, 0x71, 0x77, 0x65, 0x72, 0x74, 0x79, 0x75, 0x69, 0x6F, 0x70, 0x5B, 0x5D, 0x5C},
                      {0xC1, 0x61, 0x73, 0x64, 0x66, 0x67, 0x68, 0x6A, 0x6B, 0x6C, 0x3B, 0x27, 0x00, 0xB0}, //13 columns here
                      {0x85, 0x7A, 0x78, 0x63, 0x76, 0x62, 0x6E, 0x6D, 0x2C, 0x2E, 0x2F, 0x00, 0x00, 0x81}, //12 columns here
                      {0x80, 0x83, 0x82, 0x00, 0x00,   0x20   , 0x00, 0x00, 0x00, 0x00, 0x86, 0x87, 0xD8, 0x84}, //FN turned to LEFT arrow button and Menu turned to UP arrow. 8 columns here
                      {0x00, 0x00, 0x7B, 0x28, 0x3C, 0x3E, 0x29, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
                      };
                      

int port_rows[6] = {18, 19, 20, 21, 22, 23};
int port_cols[14] = {17, 15, 16, 14, 3, 2, 0, 1, 30, 8, 9, 10, 5, 13};

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 5;

int buttonState;
int lastButtonState = 0x00;

//setting up inputs and outputs.
void setup() {
  Keyboard.begin();
  //initializing rows as output
  pinMode(18, OUTPUT); //row 0 PF7
  pinMode(19, OUTPUT); //row 4 PF6
  pinMode(20, OUTPUT); //row 3 PF5
  pinMode(21, OUTPUT); //row 2 PF4
  pinMode(22, OUTPUT); //row 1 PF1
  pinMode(23, OUTPUT); //row 1 PF0
  
  //initializing columns as input and enabling pull-up resistors
  pinMode(0, INPUT_PULLUP);  //column 7 PD1
  pinMode(1, INPUT_PULLUP);  //column 6 PD2
  pinMode(2, INPUT_PULLUP);  //column 5 PD1
  pinMode(3, INPUT_PULLUP);  //column 4 PD0
  pinMode(5, INPUT_PULLUP);  //column 12 PC6
  pinMode(8, INPUT_PULLUP);  //column 9 PB4
  pinMode(9, INPUT_PULLUP);  //column 10 PB5
  pinMode(10, INPUT_PULLUP); //column 11 PB6
  pinMode(13, INPUT_PULLUP); //column 13 PC7
  pinMode(14, INPUT_PULLUP); //column 3 PB3
  pinMode(15, INPUT_PULLUP); //column 1 PB1
  pinMode(16, INPUT_PULLUP); //column 2 PB2
  pinMode(17, INPUT_PULLUP); //column 0 PB0
  pinMode(30, INPUT); //column 8 PD5
}

//int key=0x00;
//scanning each row and column for a pressed key.
//Try making another scan loop that goes in the opposite order.
int keyboard_scan(){
  //setting all the rows to 1
  digitalWrite(18, HIGH);
  digitalWrite(19, HIGH);
  digitalWrite(20, HIGH);
  digitalWrite(21, HIGH);
  digitalWrite(22, HIGH);
  digitalWrite(23, HIGH);
  digitalWrite(30, HIGH); //enable pull-up resistor in column 8
  int i = 0;
  int j = 0;
  for(i = 0; i < 6; i++){
    //setting a row to 0
    digitalWrite(port_rows[i], LOW);
        delay(5);
        
    //scan through the columns to see which button is pressed.
    for(j = 0; j < 14; j++){
      if( (digitalRead(port_cols[j]) == 0 )){
        return keyboard[i][j];
      }
    }
    digitalWrite(port_rows[i], HIGH); //set the row back to 1 and move on to the next row
    digitalWrite(30, LOW); //disable pull-up resistor on column 8 to prevent ghosting

  }
  return 0x00; //If nothing is found, return 0xFFF
}

//reverse scans the key matrix
int rev_keyboard_scan(){
  //setting all the rows to 1
  digitalWrite(18, HIGH);
  digitalWrite(19, HIGH);
  digitalWrite(20, HIGH);
  digitalWrite(21, HIGH);
  digitalWrite(22, HIGH);
  digitalWrite(23, HIGH);
  digitalWrite(30, HIGH); //enable pull-up resistor in column 8
  int i = 0;
  int j = 0;
  for(i = 6; i < 0; i--){
    //setting a row to 0
    digitalWrite(port_rows[i], LOW);
        delay(5);
        
    //scan through the columns to see which button is pressed.
    for(j = 14; j < 0; j--){
      if( (digitalRead(port_cols[j]) == 0 )){
        return keyboard[i][j];
      }
    }
    digitalWrite(port_rows[i], HIGH); //set the row back to 1 and move on to the next row
    digitalWrite(30, LOW); //disable pull-up resistor on column 8 to prevent ghosting

  }
  return 0x00; //If nothing is found, return 0xFFF
}
int lastKey;
void loop() {
  int key = keyboard_scan();
  //int rev = rev_keyboard_scan();
  if(key != 0x00){
    
    Keyboard.press(key); //sends pressed character 
    delay(25); //debounce
    
    //Keyboard.releaseAll();   
    if(lastKey != key){
      Keyboard.release(lastKey); //releases the previously pressed character
      Keyboard.press(key); //send actual pressed character 
      delay(25); //debounce
      }
    lastKey = key;
  }
  else
    Keyboard.releaseAll();
}


//TEST DEBOUNCE CODE ----------------------------------------

   //read the state of the switch into a local variable:
//  int reading = keyboard_scan();
//  //reading = 0x00 if nothing pressed
//  //reading > 0x00 if something pressed
//  
//  // check to see if you just pressed the button
//  // (i.e. the input went from LOW to HIGH), and you've waited long enough
//  // since the last press to ignore any noise:
//  // If the switch changed, due to noise or pressing:
//  if (reading != lastButtonState) {
//    // reset the debouncing timer
//    lastDebounceTime = millis();
//  }
//  if ((millis() - lastDebounceTime) > debounceDelay) {
//    // whatever the reading is at, it's been there for longer than the debounce fffff,,,,,,,,,,,,,,,,,,,,8ikmm,,,kk
//    // delay, so take it as the actual current state:
//    // if the button state has changed:
//    if (reading != buttonState || reading != 0x0) {
//      buttonState = reading;
//      Keyboard.press(buttonState);
//     }
//     else {
//      Keyboard.releaseAll();
//    }
//  }
//  // save the reading. Next time through the loop, it'll be the lastButtonState:
//  lastButtonState = reading;
//}
