
#include <Adafruit_GFX.h>        
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "MAX30105.h"           
#include "heartRate.h"    //khai báo thư viện      

MAX30105 sensor;

const byte RATE_SIZE  = 4; 
byte rates[RATE_SIZE]; 
byte rateSpot = 0;
long lastBeat = 0; 
float beatsPerMinute;
int beatAvg;      //khai báo để sử dụng tính toán nhịp tim

#define SCREEN_WIDTH  128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET    -1 //khai báo giá trị cho LCD

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  




void setup() {  
  display.begin(SSD1306_SWITCHCAPVCC,  0x3C); 
  display.display();
  delay(3000);
  sensor.begin(Wire, I2C_SPEED_FAST); 
  sensor.setup();
  sensor.setPulseAmplitudeRed(0xff);
  Serial.begin(9600);
}

void loop() {
 long irValue = sensor.getIR();    
      display.clearDisplay();                                         
    display.setTextSize(2);                                 
    display.setTextColor(WHITE);  
    display.setCursor(1,0);                
    display.println("nhip tim");             
    display.setCursor(1,18);                
    display.println(beatAvg);  
    display.display();
    
  if (checkForBeat(irValue) == true)                       
  {
    display.clearDisplay();                                
    
    display.setTextSize(2);                                
    display.setTextColor(WHITE);             
    display.setCursor(1,0);                
    display.println("nhip tim");             
    display.setCursor(1,18);                
    display.println(beatAvg); 
    Serial.println(beatAvg);
    display.display();                               
    delay(100);
   
    long delta = millis()  - lastBeat;                 
    lastBeat  = millis();

    beatsPerMinute = 60 / (delta / 1000.0);           

    if (beatsPerMinute < 255 && beatsPerMinute > 20)              
    {
      rates[rateSpot++] = (byte)beatsPerMinute;
      rateSpot %= RATE_SIZE; 

      
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE  ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }
 

 
}
