#include "Arduino_SensorKit.h"
U8X8_SSD1306_128X64_NONAME_HW_I2C OledHW(U8X8_PIN_NONE);
char *myStrings[] = {"This is string 1", "This is string 2", "This is string 3",
                     "This is string 4", "This is string 5", "This is string 6"
                    };
int size=sizeof(myStrings)/sizeof(myStrings[0]);
int i=0;
void setup() {
  // put your setup code here, to run once:
  OledHW.begin();
  OledHW.setFlipMode(true);
  OledHW.setFont(u8x8_font_chroma48medium8_r);
  OledHW.setCursor(0, 0);    // Set the Coordinates
}

void loop() {
  // put your main code here, to run repeatedly:
  if(i>size){
    i=0;
  }else{
    OledHW.print(myStrings[i]);
    i++;
  }
  delay(1000);
}
