/*
CÁLCULO DE CONSUMO DE ENERGIA ELÉTRICA:

TARIFAS:
ANEEL    - R$0.35806
ENERGISA - R$0.60080 + IMPOSTOS(~45%) = R$0.868610


RELAÇÃO JOULES x KWH:
1KWH = 1J/36000000

EQUAÇÃO:
E(R$) = ((MEDIAPOTENCIA(W) x TEMPO(S))/3600000) x TARIFA(R$)

OBS:
A MÉDIA DA POTÊNCIA AFERIDA -> USAR vetor_dinâmico[lista];
TEMPO -> USAR A FUNÇÃO millis();

NO ARDUINO NANO A LISTA CHEGA SOMENTE ATÉ 32 POSIÇÕES...

RODANDO NO CANAL 1
PORTA I2C: A4 e A5
AFERIÇÃO DE CORRENTE ACS712: A0
SINAL DE START VIA ESP: D10
SINAL DE STOP VIA ESP: D11
LED ON: D12
LED OFF: D9

*/

#include <Wire.h>

char t[10];
char p[10];
char c[10];
char strSend[30];
int require = -1;
//MINUTOS:
unsigned long tempo = 0;
//WATTS:
float powerPot = 0;
//R$
float powerCons = 0;

bool fim_afericao = false;

float vetCorrente[30];
unsigned long oldTime = 0;
float* list;
size_t count;
unsigned long tempo_minutos = 0;
size_t capacity;
//bool fim_afericao = false;
#define TARIFA 0.8686
bool concluir = false;
//float E = 0;
//float media_potencia = 0;
float potencia = 0;

#define FATORCORR -5

#define SENSE 0.00

//DEFINE O TEMPO DE PROCESSAMENTO MINUTOS = 60000 / SEGUNDOS = 1000
#define TIMEPROC 1000

#define ACS712 A0
#define STARTSIG 10
#define STOPSIG 11
#define LEDON 12
#define LEDOFF 9

void setup()
{
  Serial.begin(115200);

  //CANAL 1 - POWER
  Wire.begin(1); 
  delay(2000);
  Serial.println("--- Aferidor Elétrico ---");
  Serial.println("Aguardando comandos..."); 
  Serial.println(); 
  Wire.onReceive(receiveEvent); 
  Wire.onRequest(requestEvent);
  
  pinMode(ACS712, INPUT);
  pinMode(STOPSIG, INPUT);
  pinMode(LEDON, OUTPUT);
  pinMode(LEDOFF, OUTPUT);
  
  //FORÇA A INICIALIZAÇÃO DAS AFERIÇÕES:
  //digitalWrite(STARTSIG, HIGH);
  
  CreateList(1);//inicializa vetor dinâmico
}
 
void loop()
{
  if(digitalRead(STOPSIG))
  {
    //Serial.println("FINALIZAR AFERIÇÕES...");
    fim_afericao = true; 
    digitalWrite(LEDON, LOW); 
    oldTime = 0;
    //tempo_minutos = 0;
  }
  else
  {
    if(digitalRead(STARTSIG))
    {
        fim_afericao = false; 
        digitalWrite(LEDON, HIGH); 
        afereCorrente();
        concluir = false; 
        //validarPotencia();
    }
  }
  

  if(fim_afericao && (concluir == false))
  {
    delay(100);
    //powerPot = potencia;
    fechaConta();  
  }
}

// FINALIZA AS COLETAS E REALIZA CÁLCULO DE CONSUMO
void fechaConta()
{
  
  //tempo_minutos = 40;
  
  Serial.println();
  Serial.print("Tempo: ");
  Serial.print(tempo_minutos);
  Serial.print(" minutos");
  Serial.println();
  float E = 0;
  float media_potencia = 0;
  for (size_t index = 0; index < count; index++)
  {
    media_potencia = media_potencia + list[index];
    /*
    Serial.println();
    Serial.print("Soma Potência: ");
    Serial.print(media_potencia);
    Serial.println();
    */
  }
  media_potencia = media_potencia/tempo_minutos;
  //powerPot = media_potencia;
  //media_potencia = 3200;
  
  Serial.println();
  Serial.print("Media Potência: ");
  Serial.print(media_potencia);
  Serial.print(" W");
  Serial.println();

  powerPot = media_potencia;
  
  //CÁLCULO DE CONSUMO: tempo_minutos é dado em minutos e só conta se o sistema estiver consumindo...

  Serial.println();
  Serial.print("Tarifa: R$");
  Serial.print(TARIFA);
  Serial.println();
  if(tempo_minutos != 0)
  {
      E = (((media_potencia * (tempo_minutos*60))/3600000) * TARIFA); 
      powerCons = E;
  }
  //else E = 0.00;
  delete[] list;
  CreateList(1);//inicializa vetor dinâmico
  Serial.println();
  Serial.print("Consumo Total: R$");
  Serial.print(E);
  Serial.println();
  Serial.println();
  Serial.println("AFERIÇÃO DE CONSUMO CONCLUÍDA!");
  Serial.println();
  tempo = tempo_minutos;
  tempo_minutos = 0;
  concluir = true;
}


void afereCorrente()
{
  double maior_Valor = 0;
  double valor_Corrente = 0;  
  float tensao = 220;
  //float potencia = 0;
 
  for(int i = 0; i < 29; i++)
  {
    vetCorrente[i] = analogRead(ACS712);
    delayMicroseconds(600);
  }  
 
  for(int i = 0; i < 29; i++)
  {
    if(maior_Valor < vetCorrente[i])
    {
      maior_Valor = vetCorrente[i];
    }    
  }  
  Serial.println(maior_Valor); // VALOR DECIMAL 10bit
  maior_Valor = (maior_Valor+FATORCORR) * 0.004882812;
  valor_Corrente = maior_Valor - 2.5;
  valor_Corrente = valor_Corrente * 1000;
  valor_Corrente = valor_Corrente / 66;  //sensibilidade : 66mV/A para ACS712 30A / 185mV/A para ACS712 5A
  valor_Corrente = valor_Corrente / 1.41421356;
  if(valor_Corrente <= SENSE)
  {
    valor_Corrente = 0;  
  }
  Serial.println();
  Serial.print("Corrente = ");
  Serial.print(valor_Corrente);
  Serial.println(" A");
 
  potencia = valor_Corrente * tensao;
 
  Serial.print("Potencia = ");
  Serial.print(potencia);
  Serial.println(" W");
  
  Serial.print(".");
  delay(100);
  Serial.print(".");
  delay(100);
  Serial.print(".");
  delay(100);
 
  Serial.println("");  
  
  validarPotencia();
}

void validarPotencia()
{
  // Processa a cada 1 minuto, não entra potencia zero...
  //if(((millis() - oldTime) > 1000) && (potencia != 0))    
  if((millis() - oldTime) > TIMEPROC)    
  { 
    Serial.println("Salvando potência no vetor!");
    AddItem(potencia); 
    printList();
    oldTime = millis();
    //concluir = false;
    tempo_minutos++;
  }
}

// Crear una nueva lista
void CreateList(size_t _capacity)
{
  list = new float[_capacity];
  capacity = _capacity;
  count = 0;
}
 
// Añadir elemento al final de la lista
void AddItem(float item)
{
  ++count;
    
  if (count < capacity)
  {
    size_t newSize = capacity + 1;
    resize(newSize);
  } 
 
  list[count - 1] = item;
}
 
/* Eliminar ultimo elemento de la lista
void RemoveTail()
{
  --count;
}
 
// Reducir capacidad de la lista al numero de elementos
void Trim()
{
  resize(count);
}
*/ 
// Reescalar lista
void resize(size_t newCapacity)
{
  float* newList = new float[newCapacity];
  memmove(newList, list, count  * sizeof(float));
  delete[] list;
  capacity = newCapacity;
  list = newList;
}

// Muestra la lista por Serial para debug
void printList()
{
  /*
  Serial.println();
  Serial.print("Capacidade da Lista:");
  Serial.print(capacity);
  Serial.println();
  */
  
  Serial.print("  Número de Coletas:");
  Serial.print(count);

  Serial.println();
 
  Serial.print("  Itens: ");
  for (size_t index = 0; index < count; index++)
  {
    Serial.print(list[index]);
    Serial.print(" / ");
  }
  Serial.println();
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
  Serial.println("Enviando dados de consumo...");
  
  dtostrf(tempo, 5, 2, t);
  dtostrf(powerPot, 5, 2, p);
  dtostrf(powerCons, 5, 2, c);

  Serial.println(t);
  Serial.println(p);
  Serial.println(c);
  
  sprintf(strSend, "%s ; %s ; %s ; " ,t,p,c);
  Wire.write(strSend);    
  Serial.println(strSend);   
}
