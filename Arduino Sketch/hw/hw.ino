#include <Servo.h>
#include <SPI.h>
#include <WiFi.h>
#include "rgb_lcd.h"


rgb_lcd lcd;

int colorR = 0;
int colorG = 255;
int colorB = 255;

int servoPin = 5;
int buttonPin = 4;
int speakerPin = 3;
int triggerPin = 6;

char ssid[] = "Sonnet2";  

Servo groveServo;

int shaft;

boolean unlock = false;

boolean reject = false;

int length = 26; // the number of notes
char notes[] = "eeeee eeeee ee eeeee eeeee";
//char notes[] = "abcdefgabcdefgabcdefgabcde";
int beats[] = {1,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,2,1,1,1,1,1  };
int tempo = 80;

int status = WL_IDLE_STATUS;

WiFiClient client;

char server[] = "internetofdoor.azurewebsites.net";

unsigned long lastConnectionTime = 0;           // last time you connected to the server, in milliseconds
boolean lastConnected = false;                  // state of the connection last time through the main loop
const unsigned long postingInterval = 10*1000;  // delay between updates, in milliseconds

void setup() {
  
   Serial.begin(9600); 
  
   pinMode(speakerPin, OUTPUT);
   pinMode(buttonPin, INPUT);
   pinMode(servoPin, OUTPUT);
   pinMode(triggerPin, INPUT);
   groveServo.attach(servoPin);
  
   lcd.begin(16, 2);
    
   lcd.setRGB(colorR, colorG, colorB);
   
   lcd.print("Servo setup.");

   
   groveServo.write(5);
   
   
   lcd.setRGB(255, 0, 0);
   
   lcd.clear();
   
   lcd.print(ssid);
   while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid);

    // wait 10 seconds for connection:
    delay(1000);
  } 
  
  printWifiStatus();
  
  lcd.setRGB(0, 0, 0);
  lcd.clear();
  lcd.print("fuck you.");
}

void loop() {
  while ( status != WL_CONNECTED) { 
    lcd.setRGB(255, 0, 0);
    lcd.clear(); 
    lcd.print(ssid);
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid);

    // wait 10 seconds for connection:
    delay(10000);
  } 
  
  lcd.setRGB(0, 255, 255);
  lcd.clear();
  lcd.print("Hello.");
  
  if (digitalRead(buttonPin)){
    if (client.connect(server, 80)) {
      Serial.println("connected to server");
      // Make a HTTP request:
      client.println("GET /door HTTP/1.1");
      client.println("Host: internetofdoor.azurewebsites.net");
      client.println("Connection: close");
      client.println();
    }
    
    lcd.clear();
    lcd.setRGB(0, 0, 255);
    lcd.print("Ringing Doorbell.");
    for (int i = 0; i < length; i++) 
    {
        if (notes[i] == ' ')
        {
            delay(beats[i] * tempo); // rest
        }
        else
        {
            playNote(notes[i], beats[i] * tempo);
        }

        // pause between notes
        delay(tempo / 2);
    }
    lcd.clear();
    lcd.setRGB(0, 255, 255);
    lcd.print("Hello.");
  }
  if(digitalRead(triggerPin)){
    lcd.clear();
    lcd.setRGB(0, 255, 0);
    lcd.print("Unlocking Door.");
    lcd.setCursor(0, 1);
    groveServo.write(170);
    
    delay(1000);
    lcd.clear();
    lcd.print("4 Seconds left");
    
    delay(1000);
    lcd.clear();
    lcd.print("3 Seconds left");
    
    delay(1000);
    lcd.clear();
    lcd.print("2 Seconds left");
    
    delay(1000);
    lcd.clear();
    lcd.print("1 Second left");
    
    delay(1000);
    lcd.clear();
    
    groveServo.write(5);
    
    lcd.clear();
    lcd.print("Locking Door");
    delay(1000);
    
    lcd.clear();
    lcd.setRGB(0, 255, 255);
    lcd.print("Hello.");
  }
  
  if(reject){
     lcd.clear();
     lcd.setRGB(255, 0, 0);
     lcd.print("Go Away.");
     delay(2000);
     lcd.clear();
     lcd.setRGB(0, 255, 255);
     lcd.print("Hello.");
  }
  delay(100);
  
}

void playTone(int tone, int duration) {
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
        digitalWrite(speakerPin, HIGH);
        delayMicroseconds(tone);
        digitalWrite(speakerPin, LOW);
        delayMicroseconds(tone);
    }
}

void playNote(char note, int duration) {
    char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
    int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

    // play the tone corresponding to the note name
    for (int i = 0; i < 8; i++) {
        if (names[i] == note) {
            playTone(tones[i], duration);
        }
    }
}


// this method makes a HTTP connection to the server:
void httpRequest() {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    client.println("GET / HTTP/1.1");
    client.println("Host: internetofdoor.azurewebsites.net");
    client.println("Connection: close");
    client.println();
    

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


