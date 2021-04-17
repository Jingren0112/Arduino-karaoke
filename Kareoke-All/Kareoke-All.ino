#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST 1
#define BUZZER 5
#define BUTTON 4
#define BUTTON_ALT 2        //this is interrupt pin
#define SOUND_SENSOR A2

//Tris here added a potentiometer to call
#define POTENTIOMETER 0
#define LED 6

#include "Arduino_SensorKit.h"
U8X8_SSD1306_128X64_NONAME_HW_I2C OledHW( U8X8_PIN_NONE);
int tempo[2] = {120, 100};
const int melody[2][100] PROGMEM = {
  {
    // Silent Night, Original Version
    // Score available at https://musescore.com/marcsabatella/scores/3123436
    NOTE_G4, -4, NOTE_A4, 8, NOTE_G4, 4,
    NOTE_E4, -2,
    NOTE_G4, -4, NOTE_A4, 8, NOTE_G4, 4,
    NOTE_E4, -2,
    NOTE_D5, 2, NOTE_D5, 4,
    NOTE_B4, -2,
    NOTE_C5, 2, NOTE_C5, 4,
    NOTE_G4, -2,

    NOTE_A4, 2, NOTE_A4, 4,
    NOTE_C5, -4, NOTE_B4, 8, NOTE_A4, 4,
    NOTE_G4, -4, NOTE_A4, 8, NOTE_G4, 4,
    NOTE_E4, -2,
    NOTE_A4, 2, NOTE_A4, 4,
    NOTE_C5, -4, NOTE_B4, 8, NOTE_A4, 4,
    NOTE_G4, -4, NOTE_A4, 8, NOTE_G4, 4,
    NOTE_E4, -2,

    NOTE_D5, 2, NOTE_D5, 4,
    NOTE_F5, -4, NOTE_D5, 8, NOTE_B4, 4,
    NOTE_C5, -2,
    NOTE_E5, -2,
    NOTE_C5, 4, NOTE_G4, 4, NOTE_E4, 4,
    NOTE_G4, -4, NOTE_F4, 8, NOTE_D4, 4,
    NOTE_C4, -2,
    NOTE_C4, -1,
  }, {
    // Happy Birthday
    // Score available at https://musescore.com/user/8221/scores/26906
    NOTE_C4, 4, NOTE_C4, 8,
    NOTE_D4, -4, NOTE_C4, -4, NOTE_F4, -4,
    NOTE_E4, -2, NOTE_C4, 4, NOTE_C4, 8,
    NOTE_D4, -4, NOTE_C4, -4, NOTE_G4, -4,
    NOTE_F4, -2, NOTE_C4, 4, NOTE_C4, 8,

    NOTE_C5, -4, NOTE_A4, -4, NOTE_F4, -4,
    NOTE_E4, -4, NOTE_D4, -4, NOTE_AS4, 4, NOTE_AS4, 8,
    NOTE_A4, -4, NOTE_F4, -4, NOTE_G4, -4,
    NOTE_F4, -2,
  }
};

const char* const lyric[][100] PROGMEM = {
  { "silent night", "holy night",
    "all is calm", "all is bright", "round yon virgin", "mother and child",
    "holy infant, so", "tender and mild,", "sleep in heavenly", "peace", "sleep in heavenly", "peace."
  }, {"happy birthday to",
      "you, happy birth","day to you","happy birthday to","the name, happy birth", "day to you"
  }
};
int rows = sizeof(melody) / sizeof(melody[0]);
int cols = sizeof(melody[0]) / sizeof(int);
int thisNote = 0;                 //count the current note
int i, j = 0;                  // i count the current song, also count the current song's subtitle
int wholenote, divider, noteDuration = 0;
int size = sizeof(lyric) / sizeof(lyric[0]);
int sum = 0; //count the synchronization
int subTitle = 0; //count the number of notes for subtitle
int start = 1; //flag for if this is the first time running
int reset = 0;
int adjusted=0; //if potentiometer is turned
int pometerVal; //the previous value of PM
unsigned int last_call; //handle button press time.
boolean buttonState = true; 

void setup() {
  //Tris here added pin13 for the lightbub
  pinMode (LED, OUTPUT);
  pinMode (BUTTON, INPUT);
  pinMode (BUTTON_ALT, INPUT);      //set the interrupt pin to input which can be controlled by button
  attachInterrupt(digitalPinToInterrupt(BUTTON_ALT),interruptFunction,RISING);//trigger interrupt to run the function: interruptFunction with rising the button.
  pometerVal=map (analogRead(POTENTIOMETER), 0, 1023, 0, 255); //read the previous value
  OledHW.begin();
  OledHW.setFlipMode(true);
  OledHW.setFont(u8x8_font_chroma48medium8_r);
  OledHW.setCursor(0, 0);    // Set the Coordinates
  last_call=millis();         //set the time between each interrupt
  Serial.begin(9600);
}

void loop() {
  //Tris : Set a val to read the recent value of PM
  int val = analogRead(POTENTIOMETER);        //read current 
 //map up, get different voltage
  val = map ( val, 0, 1023,0,255);        //value of potentionmeter
  if(pometerVal!=val){        //check if potentiometer changed
    if ((pometerVal-val)>20){
      if(i<1){          //if current song is at position 0
        i=rows-2;       //play last song on the list
      }else{            
        i--;  
      }
      j=0;
        subTitle=0;
       thisNote=0;
       OledHW.clear();//clear led
       pometerVal=val;  //update potentiometer
   } else if((pometerVal-val)<-20){
        if(i>rows-1){          //if current song is at position 1
          i=-1;       //play first song on the list
          
        }else{            
          i++;  
        }
        j=0;
        subTitle=0;
        thisNote=0;
       OledHW.clear();
       pometerVal=val;
   }
   
 } 
 
  if (buttonState == false) {     //if pause, run empty function
    if(analogRead(SOUND_SENSOR)>800){         //check if sound is greater than threshold
      buttonState=true;
    }
   }
  else if (buttonState == true) {     //if not pause
    if (thisNote == 0) {              //if it's the first note then initialize subtitle 
        j=0;  
        subTitle = 0;
    if (i > (rows-1) || start == 1) { //if it's the last song or the first time running 
      start = 0;
      i = -1;
    }
    i++;
    wholenote = (60000 * 4) / tempo[i];
    divider = 0, noteDuration = 0;
    OledHW.clearDisplay();
    char* str = (char*)pgm_read_word_near(&(lyric[i][j]));    //read lyric
    int z = 0;
    int line=1;
    for (int b = z+14; b>0&&z < strlen(str); b--) {     //divide subtitle if too long
        String str1 = str; 
      if (str[b] == ' '&&(b+z)<=strlen(str)) {
        OledHW.print(str1.substring(z,b)); 
        OledHW.setCursor(0, line);
        z=b+1;
        b=z+14;
        line++;
      } else if(b>strlen(str)){
        OledHW.print(str1.substring(z,strlen(str))); 
        z=strlen(str);
      }
    }
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    j++;
  }
 
  if(pgm_read_word_near(&(melody[i][thisNote])) == 0) {
    thisNote = 0;
  }else {
    divider = pgm_read_word_near(&(melody[i][thisNote + 1]));   //set divider for the song
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }
    if (subTitle >= wholenote *4/3 ) {
      subTitle = 0;
      OledHW.clear();
      char* str = (char*)pgm_read_word_near(&(lyric[i][j]));
      int z = 0;
      int line=1;
      for (int b = z+14; b>0&&z < strlen(str); b--) {           //loop to see if the subtitle is too long to display
          String str1 = str; 
        if (str[b] == ' '&&(b+z)<=strlen(str)) {    //if too long, divide it on word based. (every line will have entire word written)
          OledHW.print(str1.substring(z,b)); 
          OledHW.setCursor(0, line);              //set next line cursor
          z=b+1;
          b=z+14;
          line++;
        } else if(b>strlen(str)){             
          OledHW.print(str1.substring(z,strlen(str))); 
          z=strlen(str);
        }
      }
      j++;
    }
    if (pgm_read_word_near(&(melody[i][thisNote])) == 1) {
      tone(BUZZER, 0, noteDuration * 0.9);
    } else {
      tone(BUZZER, pgm_read_word_near(&(melody[i][thisNote])), noteDuration * 0.9);
    }
    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(BUZZER);
    thisNote++;                 //next note
    subTitle += noteDuration;  //add the duration of subtitle
  } 
  }
}

void interruptFunction(){
  if((millis()-last_call)>50){    //set debounce to prevent user rapidly press the button.
    buttonState = !buttonState;
  }
  last_call=millis();             //reset last call time;
}
