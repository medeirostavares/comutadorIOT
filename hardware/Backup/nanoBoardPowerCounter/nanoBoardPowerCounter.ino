
#include <TM1637Display.h>
 
TM1637Display display(5,4); // define os pinos 5 com CLK e 4 com DIO

int tempo;
byte ini = 0;
uint32_t temporizador = 0;
 
void setup()
{
  // TEMPO EM SEGUNDOS:
  //300  = 05 min
  //600  = 15 min
  //900  = 15 min
  //1200 = 20 min
  
  tempo = 900;

  //INICIALIZA TEMPORIZADOR:
  ini = 1;
  
  //Serial.begin(9600); 
  display.setBrightness(7);
}
 
void loop() {
  //brilho(); // ajusta brilho dos leds do display
  

// Tempo em segundos
   if(tempo == 0) {
       ini = 0;
   }
  if (ini==0) {
    // Valor inicial do cronômetro    
   char s; 
   String n;   
   if(Serial.available()>0){     
      for( int i = 0; i<5; i++){       
         s = Serial.read(); 
         n = n + s; 
      }
    tempo = n.toInt();
   }
  }

// Ativa o cronometro regressivo
   if (ini == 1) {
    if (millis() - temporizador >= 1000) {
      tempo = tempo - 1;
      temporizador = millis();
    }    
   }

// Mostra o valor tempo no display  (minutos e segundos)
   // Zera e cria o array position[] para armazenar os valores do cronômetro
   uint8_t position[] = { 0, 0, 0, 0 };   
   // Calculo individual para posição no display
   position[0] = display.encodeDigit((tempo / 60) / 10); // Dezena de minuto
   position[1] = display.encodeDigit((tempo / 60) % 10); // Unidade de minuto
   position[2] = display.encodeDigit((tempo % 60) / 10); // Dezena de segundo
   position[3] = display.encodeDigit((tempo % 60) % 10); // Unidade de segundo
   // Coloca o separador ":"
   position[1] = position[1] + 128;    
   display.setSegments(position);
 
  //Mostra o valor inicial no display
  display.setSegments(position);
}
