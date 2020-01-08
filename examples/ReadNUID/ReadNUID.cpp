// Adapted from
// https://github.com/miguelbalboa/rfid/blob/master/examples/ReadNUID/ReadNUID.ino

#include <cstdio>

#include "MFRC522.h"

// The GPIO pin that's connected to the MFRC522's reset pin
#define RST_PIN RPI_V2_GPIO_P1_15
// The GPIO pin that's connected to the MFRC522's SDA pin,
// sometimes labeled SS or CE or CS.
// Doesn't have to be one of the CE pins on the pi
#define SS_PIN RPI_V2_GPIO_P1_38

/**
 * Helper routine to dump a byte array as hex values.
 */
void printHex(uint8_t *buffer, size_t bufferSize) {
  for (size_t i = 0; i < bufferSize; i++) {
    printf(" %02X", buffer[i]);
  }
}

/**
 * Helper routine to dump a byte array as dec values.
 */
void printDec(uint8_t *buffer, size_t bufferSize) {
  for (size_t i = 0; i < bufferSize; i++) {
    printf(" %03u", buffer[i]);
  }
}

int main() {
  MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class
  rfid.PCD_Init();

  // Init array that will store new NUID
  uint8_t nuidPICC[4];

  while (1) {
    // Reset the loop if no new card present on the sensor/reader. This saves
    // the entire process when idle.
    if (!rfid.PICC_IsNewCardPresent()) continue;

    // Verify if the NUID has been readed
    if (!rfid.PICC_ReadCardSerial()) continue;

    printf("PICC type: ");
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    printf(rfid.PICC_GetTypeName(piccType).c_str());
    printf("\n");

    // Check is the PICC of Classic MIFARE type
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
        piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
        piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
      printf("Your tag is not of type MIFARE Classic.\n");
      continue;
    }

    if (rfid.uid.uidByte[0] != nuidPICC[0] ||
        rfid.uid.uidByte[1] != nuidPICC[1] ||
        rfid.uid.uidByte[2] != nuidPICC[2] ||
        rfid.uid.uidByte[3] != nuidPICC[3]) {
      printf("A new card has been detected.\n");

      // Store NUID into nuidPICC array
      for (size_t i = 0; i < 4; i++) {
        nuidPICC[i] = rfid.uid.uidByte[i];
      }

      printf("The NUID tag is:\n");
      printf("In hex: ");
      printHex(rfid.uid.uidByte, rfid.uid.size);
      printf("\nIn dec: ");
      printDec(rfid.uid.uidByte, rfid.uid.size);
      printf("\n");
    } else
      printf("Card read previously.\n");

    // Halt PICC
    rfid.PICC_HaltA();

    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();
  }

  return 0;
}
