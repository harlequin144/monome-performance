#include <Bounce2.h>


const int BOUNCE_TIME = 20;
int press_count = 0;
boolean led_state = false;
int but_pin_map[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,14,15,16};

Bounce but[16] = {
  Bounce(), Bounce(), Bounce(), Bounce(),
  Bounce(), Bounce(), Bounce(), Bounce(),
  Bounce(), Bounce(), Bounce(), Bounce(),
  Bounce(), Bounce(), Bounce(), Bounce()
};


void setup() {                
  Serial.begin(57600);

  for(int i = 0; i < 16; i++){
    pinMode(but_pin_map[i], INPUT_PULLUP);
    
    but[i].attach(but_pin_map[i]);
    but[i].interval(BOUNCE_TIME);
  }
  
  pinMode(13, OUTPUT);
  //Serial.println("Pushbutton Bounce library test:");
}


void loop() { 
  for(int b = 0; b < 16; b++) {
    if( but[b].update() ){
      if( but[b].rose() ){
          Serial.write(b);  
          Serial.write(1);  
          if( b < 11 ){
            press_count = press_count + 1;
            if( led_state == false ){
              digitalWrite(13, HIGH);
              led_state = true;
            }
          }
          
        }
        
        else if( but[b].fell() ) {
          Serial.write(b);  
          Serial.write(0);  
          if( b < 11 ){ 
            press_count = press_count - 1;
            if( (led_state == true) && (press_count == 0) ){
              digitalWrite(13, LOW); 
              led_state = false;
            }
          }
          
        }
      
    }
      
  }
  
}
