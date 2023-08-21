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
#define Buzzer A5
String Modelo="";
int ModNum=0;
boolean resetStatus;

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
MFRC522::MIFARE_Key key;  //llamada a protocolo para MFRC522


void setup() {
  wdt_disable();
  lcd.begin(16, 2);
  Serial.begin(9600);
  Serial.print("Iniciando Rompe frecuencias 3");
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  delay(100);
  digitalWrite(relay, LOW);
  lcd.print("Electronics dev");//mientras carga lo pirmero en mostrar
  delay(1000);
  lcd.clear();
  SPI.begin(); // inicia la comunicacion spi
  mfrc522.PCD_Init(); // Inicia la comunicacion con rc522
  pinMode(O1, OUTPUT);
  pinMode(O2, OUTPUT);
  pinMode(O3, OUTPUT);
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
     digitalWrite(Buzzer, HIGH);
     delay(100);
     digitalWrite(Buzzer, LOW);
     delay(100);
     digitalWrite(Buzzer, HIGH);
     delay(100);
     digitalWrite(Buzzer, LOW);
     lcd.clear();
     lcd.write("Error ");
     delay(5000);
     reset();
   }
   status = mfrc522.MIFARE_Read(block, buffer1, &len);
   if (status != MFRC522::STATUS_OK) {
     Serial.print(F("Reading failed: "));
     Serial.println(mfrc522.GetStatusCodeName(status));
     digitalWrite(Buzzer, HIGH);
     delay(100);
     digitalWrite(Buzzer, LOW);
     delay(100);
     digitalWrite(Buzzer, HIGH);
     delay(100);
     digitalWrite(Buzzer, LOW);
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
     digitalWrite(Buzzer, HIGH);
     delay(100);
     digitalWrite(Buzzer, LOW);
     delay(100);
     digitalWrite(Buzzer, HIGH);
     delay(100);
     digitalWrite(Buzzer, LOW);
     lcd.clear();
     lcd.write("Error ");
     delay(5000);
     reset();
   }
   status = mfrc522.MIFARE_Read(block, buffer2, &len);
   if (status != MFRC522::STATUS_OK) {
     Serial.print(F("Reading failed: "));
     digitalWrite(Buzzer, HIGH);
     delay(100);
     digitalWrite(Buzzer, LOW);
     delay(100);
     digitalWrite(Buzzer, HIGH);
     delay(100);
     digitalWrite(Buzzer, LOW);
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
    case -25727:
      lcd.setCursor(0, 1);
      lcd.print("D1501");
      digitalWrite(O6,HIGH);
      break;
    case -25627:
      lcd.setCursor(0, 1);
      lcd.print("D1511");
      digitalWrite(O5,HIGH);
      break;
    case -25622:
      lcd.setCursor(0, 1);
      lcd.print("D1516");
      digitalWrite(O5,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case -15727:
      lcd.setCursor(0, 1);
      lcd.print("D1601");
      digitalWrite(O4,HIGH);
      break;
    case -15726:
      lcd.setCursor(0, 1);
      lcd.print("D1602");
      digitalWrite(O4,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case -15724:
      lcd.setCursor(0, 1);
      lcd.print("D1604");
      digitalWrite(O4,HIGH);
      digitalWrite(O5,HIGH);
      break;
    case -15622:
      lcd.setCursor(0, 1);
      lcd.print("D1616");
      digitalWrite(O4,HIGH);
      digitalWrite(O5,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case -5727:
      lcd.setCursor(0, 1);
      lcd.print("D1701");
      digitalWrite(O3,HIGH);
      break;
    case -5523:
      lcd.setCursor(0, 1);
      lcd.print("D1725");
      digitalWrite(O3,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case -5522:
      lcd.setCursor(0, 1);
      lcd.print("D1726");
      digitalWrite(O3,HIGH);
      digitalWrite(O5,HIGH);
      break;
    case 14273:
      lcd.setCursor(0, 1);
      lcd.print("D1901");
      digitalWrite(O3,HIGH);
      digitalWrite(O5,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case 14274:
      lcd.setCursor(0, 1);
      lcd.print("D1902");
      digitalWrite(O3,HIGH);
      digitalWrite(O4,HIGH);
      break;
    case 14275:
      lcd.setCursor(0, 1);
      lcd.print("D1903");
      digitalWrite(O3,HIGH);
      digitalWrite(O4,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case 14276:
      lcd.setCursor(0, 1);
      lcd.print("D1904");
      digitalWrite(O3,HIGH);
      digitalWrite(O4,HIGH);
      digitalWrite(O5,HIGH);
      break;
    case 14277:
      lcd.setCursor(0, 1);
      lcd.print("D1905");
      digitalWrite(O3,HIGH);
      digitalWrite(O4,HIGH);
      digitalWrite(O5,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case 14278:
      lcd.setCursor(0, 1);
      lcd.print("D1906");
      digitalWrite(O2,HIGH);
      break;
    case 14279:
      lcd.setCursor(0, 1);
      lcd.print("D1907");
      digitalWrite(O2,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case 6769:
      lcd.setCursor(0, 1);
      lcd.print("D2001");
      digitalWrite(O2,HIGH);
      digitalWrite(O5,HIGH);
      break;
    case 6770:
      lcd.setCursor(0, 1);
      lcd.print("D2002");
      digitalWrite(O2,HIGH);
      digitalWrite(O5,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case -31598:
      lcd.setCursor(0, 1);
      lcd.print("F1602");
      digitalWrite(O2,HIGH);
      digitalWrite(O4,HIGH);
      break;
    case -31597:
      lcd.setCursor(0, 1);
      lcd.print("F1603");
      digitalWrite(O2,HIGH);
      digitalWrite(O4,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case -31595:
      lcd.setCursor(0, 1);
      lcd.print("F1605");
      digitalWrite(O2,HIGH);
      digitalWrite(O4,HIGH);
      digitalWrite(O5,HIGH);
      break;
    case -31396:
      lcd.setCursor(0, 1);
      lcd.print("F1624");
      digitalWrite(O2,HIGH);
      digitalWrite(O4,HIGH);
      digitalWrite(O5,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case -31393:
      lcd.setCursor(0, 1);
      lcd.print("F1627");
      digitalWrite(O2,HIGH);
      digitalWrite(O3,HIGH);
      break;
    case -21597:
      lcd.setCursor(0, 1);
      lcd.print("F1703");
      digitalWrite(O2,HIGH);
      digitalWrite(O3,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case -21596:
      lcd.setCursor(0, 1);
      lcd.print("F1704");
      digitalWrite(O2,HIGH);
      digitalWrite(O3,HIGH);
      digitalWrite(O5,HIGH);
      break;
    case -9103:
      lcd.setCursor(0, 1);
      lcd.print("F2001");
      digitalWrite(O2,HIGH);
      digitalWrite(O3,HIGH);
      digitalWrite(O5,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case 897:
      lcd.setCursor(0, 1);
      lcd.print("F2101");
      digitalWrite(O2,HIGH);
      digitalWrite(O3,HIGH);
      digitalWrite(O4,HIGH);
      break;
    case 898:
      lcd.setCursor(0, 1);
      lcd.print("F2102");
      digitalWrite(O2,HIGH);
      digitalWrite(O3,HIGH);
      digitalWrite(O4,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case 899:
      lcd.setCursor(0, 1);
      lcd.print("F2103");
      digitalWrite(O2,HIGH);
      digitalWrite(O3,HIGH);
      digitalWrite(O4,HIGH);
      digitalWrite(O5,HIGH);
      break;
    case 900:
      lcd.setCursor(0, 1);
      lcd.print("F2104");
      digitalWrite(O2,HIGH);
      digitalWrite(O3,HIGH);
      digitalWrite(O4,HIGH);
      digitalWrite(O5,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case 19922:
      lcd.setCursor(0, 1);
      lcd.print("G3202");
      digitalWrite(O1,HIGH);
      break;
    case 20022:
      lcd.setCursor(0, 1);
      lcd.print("G3212");
      digitalWrite(O1,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case 20028:
      lcd.setCursor(0, 1);
      lcd.print("G3218");
      digitalWrite(O1,HIGH);
      digitalWrite(O5,HIGH);
      break;
    case -15609:
      lcd.setCursor(0, 1);
      lcd.print("G3507");
      digitalWrite(O1,HIGH);
      digitalWrite(O5,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case -5615:
      lcd.setCursor(0, 1);
      lcd.print("G3601");
      digitalWrite(O1,HIGH);
      digitalWrite(O4,HIGH);
      break;
    case -5614:
      lcd.setCursor(0, 1);
      lcd.print("G3602");
      digitalWrite(O1,HIGH);
      digitalWrite(O4,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case -5613:
      lcd.setCursor(0, 1);
      lcd.print("G3603");
      digitalWrite(O1,HIGH);
      digitalWrite(O4,HIGH);
      digitalWrite(O5,HIGH);
      break;
    case -5612:
      lcd.setCursor(0, 1);
      lcd.print("G3604");
      digitalWrite(O1,HIGH);
      digitalWrite(O4,HIGH);
      digitalWrite(O5,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case -5511:
      lcd.setCursor(0, 1);
      lcd.print("G3615");
      digitalWrite(O1,HIGH);
      digitalWrite(O3,HIGH);
      break;
    case -28654:
      lcd.setCursor(0, 1);
      lcd.print("G4202");
      digitalWrite(O1,HIGH);
      digitalWrite(O3,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case -28653:
      lcd.setCursor(0, 1);
      lcd.print("G4203");
      digitalWrite(O1,HIGH);
      digitalWrite(O3,HIGH);
      digitalWrite(O5,HIGH);
      break;
    case -28652:
      lcd.setCursor(0, 1);
      lcd.print("G4204");
      digitalWrite(O1,HIGH);
      digitalWrite(O3,HIGH);
      digitalWrite(O5,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case -28649:
      lcd.setCursor(0, 1);
      lcd.print("G4207");
      digitalWrite(O1,HIGH);
      digitalWrite(O3,HIGH);
      digitalWrite(O4,HIGH);
      break;
    case -28553:
      lcd.setCursor(0, 1);
      lcd.print("G4213");
      digitalWrite(O1,HIGH);
      digitalWrite(O3,HIGH);
      digitalWrite(O4,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case -25087:
      lcd.setCursor(0, 1);
      lcd.print("E0101");
      digitalWrite(O1,HIGH);
      digitalWrite(O3,HIGH);
      digitalWrite(O4,HIGH);
      digitalWrite(O5,HIGH);
      break;
    case -25086:
      lcd.setCursor(0, 1);
      lcd.print("E0102");
      digitalWrite(O1,HIGH);
      digitalWrite(O3,HIGH);
      digitalWrite(O4,HIGH);
      digitalWrite(O5,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case -15087:
      lcd.setCursor(0, 1);
      lcd.print("E0201");
      digitalWrite(O1,HIGH);
      digitalWrite(O2,HIGH);
      break;
    case -15086:
      lcd.setCursor(0, 1);
      lcd.print("E0202");
      digitalWrite(O1,HIGH);
      digitalWrite(O2,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case -15085:
      lcd.setCursor(0, 1);
      lcd.print("E0203");
      digitalWrite(O1,HIGH);
      digitalWrite(O2,HIGH);
      digitalWrite(O5,HIGH);
      break;
    case 28675:
      lcd.setCursor(0, 1);
      lcd.print("W0103");
      digitalWrite(O1,HIGH);
      digitalWrite(O2,HIGH);
      digitalWrite(O5,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case 28678:
      lcd.setCursor(0, 1);
      lcd.print("W0106");
      digitalWrite(O1,HIGH);
      digitalWrite(O2,HIGH);
      digitalWrite(O4,HIGH);
      break;
    case 11073:
      lcd.setCursor(0, 1);
      lcd.print("V0501");
      digitalWrite(O1,HIGH);
      digitalWrite(O2,HIGH);
      digitalWrite(O4,HIGH);
      digitalWrite(O6,HIGH);
      break;
    case 11074:
      lcd.setCursor(0, 1);
      lcd.print("V0502");
      digitalWrite(O1,HIGH);
      digitalWrite(O2,HIGH);
      digitalWrite(O4,HIGH);
      digitalWrite(O5,HIGH);
      break;
    case 31073:
      lcd.setCursor(0, 1);
      lcd.print("V0701");
      digitalWrite(O1,HIGH);
      digitalWrite(O2,HIGH);
      digitalWrite(O4,HIGH);
      digitalWrite(O5,HIGH);
      digitalWrite(O6,HIGH);
      break;

    default:
      digitalWrite(Buzzer, HIGH);
      delay(100);
      digitalWrite(Buzzer, LOW);
      delay(100);
      digitalWrite(Buzzer, HIGH);
      delay(100);
      digitalWrite(Buzzer, LOW);
      lcd.setCursor(0, 1);
      lcd.print("Desconocido ");
      delay(5000);
      reset();

   }
   digitalWrite(Buzzer, HIGH);
   delay(100);
   digitalWrite(Buzzer, LOW);
   delay(86400000);
   delay(86400000);
   mfrc522.PICC_HaltA();
   mfrc522.PCD_StopCrypto1();
   reset();
 }
 void reset(){ // funcion de reset desde programacion
   wdt_enable(WDTO_15MS);
   while(1){};
  }
