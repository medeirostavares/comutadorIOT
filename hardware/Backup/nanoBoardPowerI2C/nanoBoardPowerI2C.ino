/*
//FLOAT EM STRING
char t[10]; //empty array
float x = 3.38;
dtostrf(x, 3, 2, t); //(floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, empty array);
Wire.write(t);  
*/

//PORTAS I2C: A4 E A5
// RODANDO NO CANAL 1

#include <Wire.h>

char t[10];
char p[10];
char c[10];
char strSend[30];
int require = -1;

byte statusLed    = 12;

//MINUTOS:
float tempo = 42.10;
//WATTS:
float powerPot = 3100.222;
//R$
float powerCons = 12.58;

void setup()
{
  Serial.begin(115200);
  //CANAL 1
  Wire.begin(1); 
  delay(2000);
  Serial.println("--- Aferidor Elétrico ---");
  Serial.println();
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
  Serial.println("Recebendo requisição...");
  Serial.println();
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
    dtostrf(powerPot, 7, 2, p);
    dtostrf(powerCons, 6, 2, c);
    sprintf(strSend, "%s ; %s ; %s",t,p,c);
    Wire.write(strSend);    
    Serial.println(strSend);   
}
