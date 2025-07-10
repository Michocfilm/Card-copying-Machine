#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SS_PIN 10   
#define RST_PIN 9   
MFRC522 mfrc522(SS_PIN, RST_PIN);  
Adafruit_SH1106 display(OLED_RESET);

void setup() {
  Wire.begin();
  Serial.begin(9600);  
  SPI.begin(); 
  mfrc522.PCD_Init();  
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay();
  Serial.println("Ready to write new UID to Mifare Classic 1K card");
  display.setTextSize(1);
  display.setTextColor(1); 
  display.setCursor(0, 0);
  display.print(F("Ready to new UID "));
  display.display();
  
}

void loop() {
  
  byte newUid[4];
  display.setTextSize(1);
  display.setTextColor(1); 
  display.setCursor(0, 0);
  display.print(F("Ready to new UID "));
  display.display();
  // ตรวจสอบว่ามีบัตรใกล้เคียงกับ MFRC522 หรือไม่
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // อ่าน uid ของบัตร
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // แสดง UID ของบัตร
  Serial.print("Card UID: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    display.setTextSize(1);
    display.setTextColor(1); 
    display.setCursor(i * 20, 15);
    display.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    display.print(mfrc522.uid.uidByte[i], HEX);
    display.display();
  }
  Serial.println();

  // คัดลอก uid
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    newUid[i] = mfrc522.uid.uidByte[i];
  }
  delay(1000);
  Serial.println("Waiting for a new card...");
  display.setTextSize(1);
  display.setTextColor(1); 
  display.setCursor(0, 30);
  display.print(F("Waiting a new card..."));
  display.display();
  while (!mfrc522.PICC_IsNewCardPresent()) {
    delay(500);
  }

  // เรียกใช้ฟังก์ชัน MIFARE_SetUid เพื่อเขียน uid ใหม่ลงในบัตร
  if (mfrc522.MIFARE_SetUid(newUid, sizeof(newUid), true)) {
    Serial.println("New UID successfully written to the card");
    display.setTextSize(1);
    display.setTextColor(1); // Use 1 for white
    display.setCursor(0, 45);
    display.print(F("-successfully"));
    display.display();
    delay(1500);
    display.clearDisplay();
    display.display();
  } else {
    Serial.println("Error writing new UID to the card");
    display.setTextSize(1);
    display.setTextColor(1); 
    display.setCursor(0, 45);
    display.print(F("-Error"));
    display.display();
    delay(1500);
    display.clearDisplay();
    display.display();
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  
}
