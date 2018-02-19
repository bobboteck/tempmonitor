#include <LowPower.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <SimpleDHT.h>
#include <stdlib.h>

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// for DHT11, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
const int pinDHT11 = 2;
SimpleDHT11 dht11;

int counter=0;
char indice=0;
int datiGrafico[12];

void u8g2_prepare(void) 
{
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  u8g2.begin();
  Serial.begin(9600);
  // Inizializza a zero l'array del grafico
  for(int i=0;i<12;i++)
  {
    datiGrafico[i]=0;
  }
}

void loop() 
{
  digitalWrite(LED_BUILTIN, HIGH);
  u8g2.clearBuffer();
  u8g2_prepare();
  // Mostra a video il Titolo
  u8g2.drawStr(0,0, "Monitor Ambiente");
  // Disegna le barre vuote
  u8g2.drawFrame(  4,32,8,32);
  u8g2.drawFrame( 14,32,8,32);
  u8g2.drawFrame( 24,32,8,32);
  u8g2.drawFrame( 34,32,8,32);
  u8g2.drawFrame( 44,32,8,32);
  u8g2.drawFrame( 54,32,8,32);
  u8g2.drawFrame( 64,32,8,32);
  u8g2.drawFrame( 74,32,8,32);
  u8g2.drawFrame( 84,32,8,32);
  u8g2.drawFrame( 94,32,8,32);
  u8g2.drawFrame(104,32,8,32);
  u8g2.drawFrame(114,32,8,32);
  
  // read without samples.
  byte temperatura = 0;
  byte umidita = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(pinDHT11, &temperatura, &umidita, NULL)) != SimpleDHTErrSuccess)
  {
    Serial.print("Read DHT11 failed, err="); 
    Serial.println(err);
    delay(1000);
  }
  else
  {
    char t[3];  //<<<< Array a 2 elementi??? FORSE
    itoa((int)temperatura, t, 10);

    char u[3];  //<<<< Array a 2 elementi??? NO
    itoa((int)umidita, u, 10);

    char simbolo[2]=" ";  // Se non viene dimensionato da 2, ma da 1 come mi aspetterei, mostra una riga intera di caratteri a seguire
    simbolo[0]=176;       // 176 === '°'

    // Stampa la prima riga con "T: xx°C U:yy%"
    u8g2.setFont(u8g2_font_9x15_mf);
    u8g2.drawStr( 0, 16, "T:");
    u8g2.drawStr(20, 16, t);
    u8g2.drawStr(40, 16, simbolo);
    u8g2.drawStr(50, 16, "C U:");
    u8g2.drawStr(90, 16, u);
    u8g2.drawStr(110, 16, "%");

    if(counter % 720 == 0) //720
    {
      Serial.print("Indice:"); Serial.print(indice); Serial.print(" Tempreatura"); Serial.println((int)temperatura);
      datiGrafico[indice]=(int)temperatura;
      indice++; //Potrebbe essere un char visto il numero massimo di elementi (12), ma la Serial.print() non stampa il char, verificare
    }

    for(int i=0;i<12;i++)
    {
      int tg=datiGrafico[i];
      if(tg>0)
      {
        u8g2.drawBox(4+(10*i),64-(tg-8),8,tg-8);
      }
    }
  }

  u8g2.sendBuffer();  

  counter++;
  Serial.println(counter);

  digitalWrite(LED_BUILTIN, LOW);

  //delay(8000);
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}
