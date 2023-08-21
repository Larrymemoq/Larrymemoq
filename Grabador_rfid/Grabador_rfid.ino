#include <SPI.h>
#include <MFRC522.h>
#include<LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
#define RST_PIN         9           
#define SS_PIN          10          
#define relay A5

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance


void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);     
  Serial.print("Grabador rfid1");   
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  delay(100);
  digitalWrite(relay, LOW);
  lcd.print("Electronics dev");//mientras carga lo pirmero en mostrar
  delay(1000);
  lcd.clear();
  SPI.begin();               
  mfrc522.PCD_Init();        
  Serial.println(F("listo para Grabar "));
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Grabador de ");//PRIMER LINEA MENSAJE ESTATICO
  lcd.setCursor(0, 1);
  lcd.print("Modelos freq-nat");//SEGUNDA LINEA MENSAJE ESTATICO

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print(F("Card UID:"));    //Dump UID
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Grabando no ");
    lcd.setCursor(0, 1);
    lcd.print("retire token");
    digitalWrite(relay, HIGH);
    delay(100);
    digitalWrite(relay, LOW);

  }
  Serial.print(F(" PICC type: "));   // Dump PICC type
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  byte buffer[34];
  byte block;
  MFRC522::StatusCode status;
  byte len;

  Serial.setTimeout(20000L) ;     // wait until 20 seconds for input from serial
  // Ask personal data: Family name
  Serial.println(F("Type Family name, ending with #"));
  len = Serial.readBytesUntil('#', (char *) buffer, 30) ; // read family name from serial
  
  for (byte i = len; i < 30; i++) buffer[i] = ' ';     // pad with spaces

  block = 1;
  //Serial.println(F("Authenticating using key A..."));
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    lcd.clear();
    digitalWrite(relay, HIGH);
    delay(100);
    digitalWrite(relay, LOW);
    delay(100);    
    digitalWrite(relay, HIGH);
    delay(100);
    digitalWrite(relay, LOW);
    lcd.write("Error ");
    delay(2000);
    return;
  }
  else Serial.println(F("PCD_Authenticate() success: "));

  // Write block
  status = mfrc522.MIFARE_Write(block, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));    
    digitalWrite(relay, HIGH);
    delay(100);
    digitalWrite(relay, LOW);
    delay(100);    
    digitalWrite(relay, HIGH);
    delay(100);
    digitalWrite(relay, LOW);
    lcd.clear();
    lcd.write("Error ");
    delay(2000);
    return;
  }
  else Serial.println(F("MIFARE_Write() success: "));

  block = 2;
  //Serial.println(F("Authenticating using key A..."));
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    digitalWrite(relay, HIGH);
    delay(100);
    digitalWrite(relay, LOW);
    delay(100);    
    digitalWrite(relay, HIGH);
    delay(100);
    digitalWrite(relay, LOW);
    lcd.clear();
    lcd.write("Error ");
    delay(2000);
    return;
  }

  // Write block
  status = mfrc522.MIFARE_Write(block, &buffer[16], 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    digitalWrite(relay, HIGH);
    delay(100);
    digitalWrite(relay, LOW);
    delay(100);    
    digitalWrite(relay, HIGH);
    delay(100);
    digitalWrite(relay, LOW);
    lcd.clear();
    lcd.write("Error ");
    delay(2000);
    return;
  }
  else Serial.println(F("MIFARE_Write() success: "));

  // Ask personal data: First name
  Serial.println(F("Type First name, ending with #"));
  len = Serial.readBytesUntil('#', (char *) buffer, 20) ; // read first name from serial
  for (byte i = len; i < 20; i++) buffer[i] = ' ';     // pad with spaces

  block = 4;
  //Serial.println(F("Authenticating using key A..."));
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    digitalWrite(relay, HIGH);
    delay(100);
    digitalWrite(relay, LOW);
    delay(100);    
    digitalWrite(relay, HIGH);
    delay(100);
    digitalWrite(relay, LOW);
    lcd.clear();
    lcd.write("Error ");
    delay(2000);
    return;
  }

  // Write block
  status = mfrc522.MIFARE_Write(block, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    digitalWrite(relay, HIGH);
    delay(100);
    digitalWrite(relay, LOW);
    delay(100);    
    digitalWrite(relay, HIGH);
    delay(100);
    digitalWrite(relay, LOW);
    lcd.clear();
    lcd.write("Error ");
    delay(2000);
    return;
  }
  else Serial.println(F("MIFARE_Write() success: "));

  block = 5;
  //Serial.println(F("Authenticating using key A..."));
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    digitalWrite(relay, HIGH);
    delay(100);
    digitalWrite(relay, LOW);
    delay(100);    
    digitalWrite(relay, HIGH);
    delay(100);
    digitalWrite(relay, LOW);
    lcd.clear();
    lcd.write("Error ");
    delay(2000);
    return;
  }

  // Write block
  status = mfrc522.MIFARE_Write(block, &buffer[16], 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    digitalWrite(relay, HIGH);
    delay(100);
    digitalWrite(relay, LOW);
    delay(100);    
    digitalWrite(relay, HIGH);
    delay(100);
    digitalWrite(relay, LOW);
    lcd.clear();
    lcd.write("Error ");
    delay(2000);
    return;
  }
  else Serial.println(F("MIFARE_Write() success: "));

  digitalWrite(relay, HIGH);
  delay(200);
  digitalWrite(relay, LOW);
  Serial.println(" Listo ");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Listo puede");
  lcd.setCursor(0, 1);
  lcd.print("retirar token :)");
  delay(50000);
  mfrc522.PICC_HaltA(); // Halt PICC
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
}