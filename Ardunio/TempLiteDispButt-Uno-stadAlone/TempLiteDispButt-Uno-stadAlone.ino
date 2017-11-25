
#include <Wire.h>
#include <stdlib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2  // Data wire is plugged into port 2 on the Arduino.
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static unsigned char PROGMEM const logo16_glcd_bmp[] =
{ B00000000, B11000000,
 B00000001, B11000000,
 B00000001, B11000000,
 B00000011, B11100000,
 B11110011, B11100000,
 B11111111, B11111000,
 B01111111, B11111111,
 B00111111, B11111111,
 B00011111, B11111100,
 B00001111, B11110000,
 B00011111, B11100000,
 B00111111, B11100000,
 B00111111, B11110000,
 B01111100, B11110000,
 B01110000, B01110000,
 B00000000, B00110000 };
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int Led=13;                  // Define LED port.
int buttonpin=12;            // Define switch port.
int val = 0;                 // Define digital variable val.
int inByte = 0;              // Incoming serial byte.
int flushByte = 0;           // Incoming serial byte.
int sensorPin = 2;           // Vars for photoresistor.
int value = 0; 

OneWire oneWire(ONE_WIRE_BUS);  // Setup a oneWire instance to 
                                //  communicate with any OneWire
								//  devices (not just Maxim/Dallas
  								//  temperature ICs). 
DallasTemperature sensors(&oneWire);  // Pass our oneWire reference
                                      //  to Dallas Temperature.
void setup(void)
{
    pinMode(Led,OUTPUT);          // Define LED as a output port.
    pinMode(buttonpin,INPUT);     // Define switch as a output port.

                                 // By default, the display will generate
								 //  the high voltage from the 3.3v line
								 //  internally! (neat!).
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize with the 
	                                            //  I2C addr set to 0x3C
    Serial.begin(9600);      // Start serial port.
    // while (!Serial) {
        // ;                    // Wait for serial port to connect.
                             // //	Needed for native USB port only.
    // }
    readTime(3000);
    display.clearDisplay();   // Clears the screen and buffer.
    textSetup();
    display.println("Waiting for remote");
    display.display();
    //establishContact();         // Send a byte to establish contact,
                                //	until the receiver responds.
    display.println("Ready...");
    readTime(1000);
    while(Serial.available() > 0)     // Flush input buffer.
		flushByte = Serial.read();
    sensors.begin();             // Start up the library.
  }

void loop(void)
{ 
    val=digitalRead(buttonpin);       // Poll the push button.
    if(val==LOW)  {               // This is done if the button is pushed.
        digitalWrite(Led,HIGH);
        textSetup();
        displayMiniGraphic();
        displayTemperature();
    	display.setCursor(10,42);
        displayLight();
        readTime(4000);
	} else {
        digitalWrite(Led,LOW);
    }
    if (Serial.available() > 0) {       // Check to see if there is any data 
        digitalWrite(Led, HIGH);     //  in the serial port buffer to be read.
        inByte = Serial.read();         // Get incoming byte.
    	response();
        digitalWrite(Led,LOW);
    }
}

void establishContact(void) {
  while (Serial.available() <= 0) {
    Serial.println("Waiting: 0");       // Send an initial string.
    delay(300);
  }
}

void textSetup(void) {
   display.clearDisplay();   // Clears the screen and buffer.
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10,0);
}

void readTime(int rT) {
	display.display();
    delay(rT);
    display.clearDisplay();
}
	  
void sendInfoOnInvalidRequest(void) {
    Serial.print("Invalid: ASCII ");
    Serial.println(inByte);
    Serial.print("Invalid: Char ");
    Serial.println((char)inByte);
}
	  
void displayInfoOnInvalidRequest(void) {
    display.print("Invalid: ASCII ");
    display.println(inByte);
	display.print("Invalid Char ");
    display.println((char)inByte);
}  	

void response(void) {
    textSetup();
    display.println("Remote data request");
    if (inByte == 'T'|| inByte == 't') {
        display.setCursor(10,20);
    	displayTemperature();  
        Serial.print("Temperature: ");
        Serial.println(sensors.getTempFByIndex(0)); 
    } else if (inByte == 'L'|| inByte == 'l') {	
        display.setCursor(10,30);
        displayLight();
        Serial.print("LightValue: ");
        Serial.println(value, DEC); 
    } else if (inByte >= 32 && inByte <= 126) {
        sendInfoOnInvalidRequest();
    	displayInfoOnInvalidRequest();
	} else {
        sendInfoOnInvalidRequest();
    }	
	readTime(4000);
}

void displayMiniGraphic(void) {	
    display.clearDisplay();
    display.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
    display.display();
}

void displayTemperature(void){
    sensors.requestTemperatures(); 
    display.print("Temperature: ");
    display.println(sensors.getTempFByIndex(0)); 
}

void displayLight(void) {
    value = analogRead(sensorPin);
    display.print("LightValue: ");
    display.println(value, DEC); 
}






