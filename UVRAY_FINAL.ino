#include <SerialESP8266wifi.h>
#include <FirebaseArduino.h>


#define FIREBASE_HOST "Hostname"
#define FIREBASE_AUTH "AuthKey"

//Hardware pin definitions
int UVOUT =  A1; //Output from the sensor
int REF_3V3 = A0; //3.3V power on the Arduino board

void setup()
{
 Serial.begin(9600);
  //Serial.begin(115200);
  Serial.println();

  WiFi.begin("SSID", "Password");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

 pinMode(UVOUT, INPUT);
 pinMode(REF_3V3, INPUT);
 Serial.println("MP8511 example");
  //Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
 
void loop()
{
 int uvLevel = averageAnalogRead(UVOUT);
 //int uvLevel = analogRead(UVOUT);
 int refLevel = averageAnalogRead(REF_3V3);
 //Use the 3.3V power pin as a reference to get a very accurate output value from sensor
 float outputVoltage = 3.3 / refLevel * uvLevel;  
 
 outputVoltage = outputVoltage/10; 
 float uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0); 
 Serial.print("MP8511 output: ");
 Serial.print(uvLevel);
 Serial.print(" MP8511 voltage: ");
 Serial.print(outputVoltage);  
 Serial.println();
  
 Firebase.setFloat("reading", uvIntensity);
  // handle error
 if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
 delay(100);
}
//Takes an average of readings on a given pin
//Returns the average
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);  
}
//The Arduino Map function but for floats
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
