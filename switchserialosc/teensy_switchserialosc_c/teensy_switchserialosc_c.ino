/* Pushbutton with Pullup, Teensyduino Tutorial #3
   http://www.pjrc.com/teensy/tutorial3.html

   This example code is in the public domain.
*/

int but_state[12];


void setup() {                
  Serial.begin(38400);
  for(int i = 0; i < 12; i++){
    but_state[i] = HIGH;
    pinMode(i, INPUT);
  }
}


void loop()                     
{
  for(int i = 0; i<12; i++){
    if (digitalRead(i) != but_state[i]) {
      but_state[i] = digitalRead(i);
      if(but_state[i] == HIGH){
      //  Serial.println("Button is not pressed...");
        Serial.print(i, HEX);  
        Serial.print(1, HEX);  
      } else {
        Serial.print(i, HEX);  
        Serial.print(0, HEX);  
      }  
    }
  }
  
  delay(50);
}

