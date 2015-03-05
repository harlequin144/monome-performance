#include <Bounce.h>


const int BOUNCE_TIME = 20;
int press_count = 0;
boolean led_state = false;
int but_pin_map[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,14,15,16};

Bounce but[16] = {
  
  Bounce(0, BOUNCE_TIME),
  Bounce(1, BOUNCE_TIME),
  Bounce(2, BOUNCE_TIME),
  Bounce(3, BOUNCE_TIME),
  Bounce(4, BOUNCE_TIME),
  Bounce(5, BOUNCE_TIME),
  Bounce(6, BOUNCE_TIME),
  Bounce(7, BOUNCE_TIME),
  Bounce(8, BOUNCE_TIME),
  Bounce(9, BOUNCE_TIME),
  Bounce(10, BOUNCE_TIME),
  Bounce(11, BOUNCE_TIME),
  Bounce(12, BOUNCE_TIME),
  Bounce(14, BOUNCE_TIME),
  Bounce(15, BOUNCE_TIME),
  Bounce(16, BOUNCE_TIME)
  
  /*
  Bounce(),Bounce(),Bounce(),Bounce(),
  Bounce(),Bounce(),Bounce(),Bounce(),
  Bounce(),Bounce(),Bounce(),Bounce(),
  Bounce(),Bounce(),Bounce(),Bounce()
  */
};



void setup() {                
  //Serial.begin(38400);
  Serial.begin(57600);

  for(int i = 0; i < 16; i++){
    //pinMode(but_pin_map[i], INPUT);
    pinMode(but_pin_map[i], INPUT_PULLUP);
    //but[i] = Bounce(but_pin_map[i], 10);  // 10 ms debounce
    //but[i].attach(but_pin_map[i]
  }
  
  pinMode(13, OUTPUT);
  Serial.println("Pushbutton Bounce library test:");
}





void loop() {
  //if (pushbutton.update()) {
  //  if (pushbutton.fallingEdge()) {
  //    count = count + 1;
  //    countAt = millis();
  //  }
  //} else {
  //  if (count != countPrinted) {
  //    unsigned long nowMillis = millis();
  //    if (nowMillis - countAt > 100) {
  //      Serial.print("count: ");
  //      Serial.println(count);
  //      countPrinted = count;
  //    }
  //  }
  //}



  for(int b = 0; b < 16; b++) {
    if( but[b].update() ) {
      //Serial.print(b, HEX);  
      if( but[b].risingEdge() ){
        Serial.write(b);  
        Serial.write(1);  
        if( b < 11 ){
          press_count = press_count + 1;
          if( led_state == false ){
            digitalWrite(13, HIGH);   // set the LED on
            led_state = true;
          }
        }
      }
      else if (but[b].fallingEdge() ) {
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
       //Serial.println(press_count);
    }
      
  }
  
}

