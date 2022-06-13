/*
//FLOAT EM STRING
char t[10]; //empty array
float x = 3.38;
dtostrf(x, 3, 2, t); //(floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, empty array);
Wire.write(t);  
*/

#include <Wire.h>

char t[10];
char p[10];
char w[10];
char strSend[30];
int require = -1;

byte statusLed    = 12;

float tempo = 42.10;
float power = 3100.222;
float water = 30.30;

void setup()
{
  Serial.begin(115200);
  Wire.begin(1); 
  digitalWrite(statusLed, HIGH);
  Wire.onReceive(receiveEvent); 
  Wire.onRequest(requestEvent);
}

void loop()
{
    
}

void receiveEvent(int howMany)
{
  if(Wire.available())
  {
    require = Wire.read();
  }
  Serial.println(require);
  Serial.print(".");
  delay(100);
  Serial.print(".");
  delay(100);
  Serial.print(".");
  delay(100);
  Serial.println("");
}

void requestEvent()
{
  Serial.println("Enviando dados do banho...");
    
    dtostrf(tempo, 6, 2, t);
    dtostrf(power, 7, 2, p);
    dtostrf(water, 6, 2, w);
    sprintf(strSend, "%s ; %s ; %s",t,p,w);
    Wire.write(strSend);    
    Serial.println(strSend);   
}
