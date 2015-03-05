#include <Bounce.h>

int but_pin_map[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,14,15,16};

int but_state[16] = {LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,};
//int num_pressed = 0;

int pin_val = 0;
int but = 0;
//boolean but_on = false;
//boolean led_state = false;


void setup() {                
  Serial.begin(38400);

  for(int i = 0; i < 16; i++){
    pinMode(but_pin_map[i], INPUT);
  }
  
  //pinMode(13, OUTPUT);
}


void loop() {
  pin_val = 0;
  but = 0;
  //but_on = false;
  
  for(but = 0; but < 16; but++) {
    //pin_val = digitalRead(but_pin_map[but]);
      Serial.print(but , HEX);  
      Serial.print(digitalRead(but_pin_map[but]) , HEX);  
      
    /*if(pin_val != but_state[but]) {
      but_state[but] = pin_val;

      if(pin_val == HIGH){
        Serial.print(but, HEX);  
        //Serial.print(pin_val, HEX);
        Serial.print(1, HEX);
      } else {
        Serial.print(but, HEX);  
        Serial.print(0, HEX);  
      }
      
    }
  */  
  }
  
  
  delay(500);
}
