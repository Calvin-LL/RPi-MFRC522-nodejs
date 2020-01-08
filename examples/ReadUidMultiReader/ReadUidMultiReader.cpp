// Adapted from
// https://github.com/miguelbalboa/rfid/blob/master/examples/ReadUidMultiReader/ReadUidMultiReader.ino

#include <cstdio>

#include "MFRC522.h"

#define RST_1_PIN RPI_V2_GPIO_P1_15
#define RST_2_PIN RPI_V2_GPIO_P1_18
#define SS_1_PIN RPI_V2_GPIO_P1_38
#define SS_2_PIN RPI_V2_GPIO_P1_40

#define NR_OF_READERS 2

uint8_t ssPins[] = {SS_1_PIN, SS_2_PIN};
uint8_t rstPins[] = {RST_1_PIN, RST_2_PIN};

/**
 * Helper routine to dump a byte array as hex values.
 */
void printHex(uint8_t *buffer, size_t bufferSize) {
  for (size_t i = 0; i < bufferSize; i++) {
    printf(" %02X", buffer[i]);
  }
}

int main() {
  MFRC522 mfrc522[NR_OF_READERS];  // Create MFRC522 instance.

  for (size_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader],
                             rstPins[reader]);  // Init each MFRC522 card
    printf("Reader ");
    printf("%u", reader);
    printf(": ");
    mfrc522[reader].PCD_DumpVersionToSerial();
  }

  while (1) {
    for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
      // Look for new cards
      mfrc522[reader].PCD_AntennaOn();

      if ((mfrc522[reader].PICC_IsNewCardPresent() ||
           mfrc522[reader].PICC_IsNewCardPresent()) &&
          mfrc522[reader].PICC_ReadCardSerial()) {
        printf("Reader ");
        printf("%u", reader);
        // Show some details of the PICC (that is: the tag/card)
        printf(": Card UID:");
        printHex(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
        printf("\n");
        printf("PICC type: ");
        MFRC522::PICC_Type piccType =
            mfrc522[reader].PICC_GetType(mfrc522[reader].uid.sak);
        printf(mfrc522[reader].PICC_GetTypeName(piccType).c_str());
        printf("\n");
      } else {
        printf("Reader %u: none\n", reader);
      }
      mfrc522[reader].PCD_AntennaOff();

      bcm2835_delay(250);
    }
  }

  return 0;
}
