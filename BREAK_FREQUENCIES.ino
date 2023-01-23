//librerias
#include <avr/wdt.h>
#include <MFRC522.h>
#include<LiquidCrystal.h>
#include <SPI.h>
//pinout LCd RFID y definicion de los pines de salidas
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
#define SS_PIN 10
#define RST_PIN 9
#define O1 A0
#define O2 A1
#define O3 A2
#define O4 A3
#define O5 A4
#define O6 8
String Modelo="";
int ModNum=0;
boolean resetStatus;

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
MFRC522::MIFARE_Key key;  //llamada a protocolo para MFRC522


void setup() {
  wdt_disable();
  lcd.begin(16, 2);
  Serial.begin(9600);
  Serial.print("Iniciando Rompe frecuencias 1");
  lcd.print("Electronics dev");//mientras carga lo pirmero en mostrar
  delay(2000);
  lcd.clear();
  SPI.begin(); // inicia la comunicacion spi
  mfrc522.PCD_Init(); // Inicia la comunicacion con rc522
  pinMode(O1, OUTPUT);//led rojo
  pinMode(O2, OUTPUT);//relay
  pinMode(O3, OUTPUT);//buzzer
  pinMode(O4, OUTPUT);
  pinMode(O5, OUTPUT);
  pinMode(O6, OUTPUT);
}
void loop() {
  digitalWrite(O1,LOW);
  digitalWrite(O2,LOW);     //inician los reles en 0vdc
  digitalWrite(O3,LOW);
  digitalWrite(O4,LOW);
  digitalWrite(O5,LOW);
  digitalWrite(O6,LOW);
  lcd.setCursor(0, 0);
  lcd.print("Rassini Frenos ");//PRIMER LINEA MENSAJE ESTATICO
  lcd.setCursor(0, 1);
  lcd.print("Freq Natural");//SEGUNDA LINEA MENSAJE ESTATICO
  MFRC522::MIFARE_Key key;

   for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
   byte block;
   byte len;
   MFRC522::StatusCode status;
   if ( !mfrc522.PICC_IsNewCardPresent()) {
     return;
   }
   if ( !mfrc522.PICC_ReadCardSerial()) {
     return;
   }
   byte buffer1[18];
   block = 4;
   len = 18;
   status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid));

   if (status != MFRC522::STATUS_OK) {
     Serial.print(F("Authentication failed: "));
     Serial.println(mfrc522.GetStatusCodeName(status));
     lcd.clear();
     lcd.write("Error ");
     delay(5000);
     reset();
   }
   status = mfrc522.MIFARE_Read(block, buffer1, &len);
   if (status != MFRC522::STATUS_OK) {
     Serial.print(F("Reading failed: "));
     Serial.println(mfrc522.GetStatusCodeName(status));
     lcd.clear();
     lcd.write("Error ");
     delay(5000);
     reset();
   }
   for (uint8_t i = 0; i < 16; i++)   //primer buffer de memorias... casi no se usa nunca
   {
     if (buffer1[i] != 32)
     {
       Serial.write(buffer1[i]);
       Serial.print("buffer1");
     }
   }
   byte buffer2[18];
   block = 1;
   status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid));
   lcd.clear();
  // inicio de if  para los errores
   if (status != MFRC522::STATUS_OK) {
     Serial.print(F("Authentication failed: "));
     Serial.println(mfrc522.GetStatusCodeName(status));
     lcd.clear();
     lcd.write("Error ");
     delay(5000);
     reset();
   }
   status = mfrc522.MIFARE_Read(block, buffer2, &len);
   if (status != MFRC522::STATUS_OK) {
     Serial.print(F("Reading failed: "));
     lcd.clear();
     lcd.write("Error ");
     delay(5000);
     Serial.println(mfrc522.GetStatusCodeName(status));
     reset();
   }
   //final de erroes
   lcd.setCursor(0, 0);
   lcd.print("Modelo:");
   for (uint8_t i = 0; i < 5; i++) {   // array de lectura de memorias
     Serial.write(buffer2[i]);
   }
   for(int x=0;x<5;x++){     //contruccionde array en String
       Modelo+=buffer2[x];
   }
   ModNum=Modelo.toInt(); // convertir de string a int
   Serial.print(ModNum);
   switch (ModNum) {   //inico de switch case para contruir la lectura
    case 12545:
      lcd.setCursor(0, 1);
      lcd.print("VO501");
      digitalWrite(O1,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case 12544:
      lcd.setCursor(0, 1);
      lcd.print("VO500");
      digitalWrite(O1,HIGH);
      digitalWrite(O2,HIGH);
      digitalWrite(O3,HIGH);
      digitalWrite(O4,HIGH);
      digitalWrite(O5,HIGH);
      digitalWrite(O6,HIGH);
      break;
    default:
      lcd.setCursor(0, 1);
      lcd.print("Desconocido ");
   }
   delay(86400000);
   mfrc522.PICC_HaltA();
   mfrc522.PCD_StopCrypto1();
   reset();
 }
 void reset(){ // funcion de reset desde programacion
   wdt_enable(WDTO_15MS);
   while(1){};
  }
