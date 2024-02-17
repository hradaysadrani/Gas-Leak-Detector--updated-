// Code written by Hraday Sadrani, B.Tech. , Electrical Engineering Student @ NITS on 27/01/2024
#include <WiFi.h>   // As we have used ESP32. So to disable WiFi and Bluetooth antenna, we have used this library.
#define sensor 34 // Gas Sensor (MQ-2 or MQ-6 is connected on D34)
#include <ESP32Servo.h>


Servo myregulator;  // Declared a myregulator object.
int pos = 0;    // Initially, Regulator will be at ON position.
int regulatorPin = 25;  // Regulator Knob's (Servo Motor) signal pin is attached to D25.

void setup() {
  Serial.begin(115200);  // For Serial Monitor
  Serial2.begin(115200); // For A9G Board
  // Making Radio OFF for power saving
  WiFi.mode(WIFI_OFF);  // WiFi OFF
  btStop();   // Bluetooth OFF
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myregulator.setPeriodHertz(50); // Standard 50 hz servo
  myregulator.attach(regulatorPin, 500, 2400); // Attaches the regulator on D25 to the myregulator object.
  // using default min/max of 500us and 2400us
  myregulator.write(0);
  Serial.println("Please wait while GSM module turns ON..");
  delay(20000); // Safe delay required for GSM module to completely turn on.
  Serial.println("Ok, System is Ready to Use..");
}

void SendMessage() {
  Serial.println("Sending SMS Alert, Please Wait!!");
  Serial2.println("AT+CMGF=1\r");
  Serial.println("AT+CMGF=1\r");
  delay(2000);
  Serial2.println("AT+CMGS=\"+919321899124\"\r");
  Serial.println("AT+CMGS=\"+919321899124\"\r");
  delay(2000);
  Serial2.println("GAS LEAK DETECTED. REGULATOR HAS BEEN TURNED OFF!");
  Serial.println("GAS LEAK DETECTED. REGULATOR HAS BEEN TURNED OFF!");
  delay(200);
  Serial2.println((char)26);
  Serial.println((char)26);
  Serial.println("Done! Alert Sent Successfully");
  delay(10000);
}

void loop() {
  if (Serial2.available() > 0) {
    Serial.write(Serial2.read());
  }
  int value = analogRead(sensor);
  value = map(value, 0, 4095, 0, 100);
  Serial.print("Value: ");
  Serial.println(value);

  // The below if-else block will detect whether there is an gas leak or not. If yes, then it will automatically turn off the regulator and sends an alert message to the user.
  
  if(value > 70){
    Serial.println("Verifying is there any actual Gas Leak?");
    Serial.println("Please Wait....");
    delay(5000);
    // The delay given above will prevent instantly rectified accidental leaks.
    if(value > 65){
      SendMessage();
      myregulator.write(90); 
    }
    else{
      Serial.println("Seems that situation is normal.");
    }
  }
  delay(500);
}
