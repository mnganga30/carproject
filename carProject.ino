#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h> 


#define RELAY_ON 0
#define RELAY_OFF 1

#define start  26
#define on  24
#define acc  22
#define starter  2

char* myStrings[]={"none","martin", "Grace", "Bernard","This is string 4", "This is string 5","This is string 6"};


int getFingerprintIDez();




const int  buttonPin2 = 9;    
const int  buttonPin = 8;  

const int ledPin = 7;       
const int ledPin2 = 6;       
const int ledPin3 = 5;   

bool begins = true;
bool found = false;

bool firstClear = true;
bool firstClearacc  =true;
bool firstClearon = true;
bool firstClearst = true;
bool firstStart= true;

const int numReadings = 10;

int who;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int inputPin = A0;
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int buttonState2 = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
int state =0;

// pin #10 is IN from sensor (GREEN wire)
// pin #11 is OUT from arduino  (WHITE wire)

SoftwareSerial mySerial(10, 11);
LiquidCrystal lcd(28, 30, 36, 38, 40, 42); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7) 

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
    pinMode(buttonPin, INPUT);
    pinMode(buttonPin2, INPUT);
    
    pinMode(ledPin, OUTPUT);
    pinMode(ledPin2, OUTPUT);
    pinMode(ledPin3, OUTPUT);
    
    pinMode(start, OUTPUT);
    pinMode(on, OUTPUT);
    pinMode(acc, OUTPUT);
    pinMode(starter, OUTPUT);
    
    
    digitalWrite(start, RELAY_OFF);
    digitalWrite(on, RELAY_OFF);
    digitalWrite(acc, RELAY_OFF);
    digitalWrite(starter, RELAY_OFF);
    
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);

    lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display 
    
    Serial.begin(9600);
    
//    while (!Serial);  // For Yun/Leo/Micro/Zero/...
    
    Serial.println("Adafruit finger detect test");
    
    // set the data rate for the sensor serial port
    finger.begin(57600);
    
    if (finger.verifyPassword()) {
        Serial.println("Found fingerprint sensor!");
    } else {
        Serial.println("Did not find fingerprint sensor :(");
        while (1);
    }
    Serial.println("Waiting for valid finger...");
      lcd.print(" Scan finger   "); // Prints "Arduino" on the LCD 
      
      for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0; 
      
    
}


void loop() {
              getFingerprintIDez();
              lcd.print(" Scan finger   "); // Prints "Arduino" on the LCD 
                
                while(found == true) {   
                    buttonState = digitalRead(buttonPin);
                    buttonState2 = digitalRead(buttonPin2);
                    if(buttonState2 == HIGH)
                    {  buttonPushCounter =4; }
                    
                    if (buttonState != lastButtonState) {
                        if (buttonState == HIGH) {
                            buttonPushCounter++;
                            Serial.println("on");
                            Serial.print("number of button pushes:  ");
                            Serial.println(buttonPushCounter);
                            begins = false;
                        } else { Serial.println("off");}
                        delay(50);
                    }
                    lastButtonState = buttonState;
                    state = buttonPushCounter % 4 ;
            
                    
                switch (state) {
                case 1:
                    Serial.println("acc");
                    accMethod(); 
                    break;
                case 2:
                    Serial.println(" on");
                    onMethod();
                    break;
                case 3:
                   Serial.println("start");
                   startMethod();
                   break;
                 default:
                  Serial.println("off");
                  offMethod();    
            }
                   
                }
}

uint8_t getFingerprintID() {
    uint8_t p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
    case FINGERPRINT_NOFINGER:
        Serial.println("No finger detected");
        return p;
    case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return p;
    case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        return p;
    default:
        Serial.println("Unknown error");
        return p;
    }
    
    // OK success!
    
    p = finger.image2Tz();
    switch (p) {
    case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
    case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        return p;
    case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return p;
    case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        return p;
    case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        return p;
    default:
        Serial.println("Unknown error");
        return p;
    }
    
    // OK converted!
    p = finger.fingerFastSearch();
    if (p == FINGERPRINT_OK) {
        Serial.println("Found a print match!");
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
        Serial.println("Communication error");
        return p;
    } else if (p == FINGERPRINT_NOTFOUND) {
        Serial.println("Did not find a match");
        return p;
    } else {
        Serial.println("Unknown error");
        return p;
    }   
    
    // found a match!
    lcd.print("Found ID #"); lcd.print(finger.fingerID); // Prints "Arduino" on the LCD  
    Serial.print("Found ID #"); Serial.print(finger.fingerID); 
    Serial.print(" with confidence of "); Serial.println(finger.confidence); 
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK)  return -1;
    
    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)  return -1;
    
    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK)  return -1;
    
    // found a match!
    found = true;
    lcd.print(" Found");
        Serial.print("Found ID #"); Serial.print(finger.fingerID); 


    who = finger.fingerID;
    Serial.print("Found ID #"); Serial.print(finger.fingerID); 
    Serial.print(" with confidence of "); Serial.println(finger.confidence);
    
    return finger.fingerID; 
}

void startMethod()
{
    digitalWrite(on, RELAY_ON);
    digitalWrite(acc, RELAY_ON);
    digitalWrite(start, RELAY_ON);
    if (firstStart == true)
    {
        digitalWrite(starter, RELAY_ON);
        delay(900);
        digitalWrite(starter, RELAY_OFF);
        firstStart = false;
    }    
    digitalWrite(ledPin3, HIGH);
    lcd.setCursor(0, 1);
    lcd.print(" Car is started");  
}

void onMethod()
{
    digitalWrite(on, RELAY_ON);
    digitalWrite(acc, RELAY_ON);
    digitalWrite(start, RELAY_ON);
    digitalWrite(starter, RELAY_OFF);

    digitalWrite(ledPin2, HIGH);
    if (firstClearon == true)
    {
        lcd.clear(); // Clears the display 
        firstClearon = false;
    }    
    lcd.setCursor(0, 1);    
    lcd.print(" Car is on"); // Prints "Arduino" on the LCD 
    
  
    // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(inputPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings)
    // ...wrap around to the beginning:
    readIndex = 0;

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  Serial.println(average);
  while (average >= 1000)
  {
        digitalWrite(starter, RELAY_ON);
         // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(inputPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings)
    // ...wrap around to the beginning:
    readIndex = 0;

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  Serial.println(average);
  }
  delay(1);        // delay in between reads for stability
}


void accMethod()
{
    digitalWrite(ledPin, HIGH);
    digitalWrite(acc, RELAY_ON);
    if (firstClearacc == true)
    {
        lcd.clear(); // Clears the display 
        firstClearacc = false;
    }  
    lcd.setCursor(0, 1);
    lcd.print(" Accessies Mode"); // Prints "Arduino" on the LCD 
}


void offMethod()
{
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    
    digitalWrite(on, RELAY_OFF);
    digitalWrite(start, RELAY_OFF);
    digitalWrite(acc, RELAY_OFF);
    if (begins == true)
    {
        if (firstClear == true)
        {
            lcd.clear(); // Clears the display 
            firstClear = false;
        }
        lcd.setCursor(0,0);
        lcd.print("--Match Found---"); // Prints "Arduino" on the LCD 
        lcd.setCursor(0,1); // Sets the location at which subsequent text written to the LCD will be displayed 
        lcd.print("Welcome  ");
        lcd.print(myStrings[who]); 
        lcd.noAutoscroll();
      }
    else{
       lcd.print("Turning car off.."); // Prints "Arduino" on the LCD 
        
       
        begins = true;
        firstClear = true;
        firstClearacc  =true;
        firstClearon = true;
        firstClearst = true;
        firstStart = true;
        
        found = false;
    }
    
}


