# RPi-MFRC522-nodejs

RPi-MFRC522 for nodejs

Read UIDs from your RFID cards with MFRC522 readers on Raspberry Pi. Works with multiple readers.

If you want to read more than the UID, change `./src/reader.cpp` according to https://github.com/CoolCyberBrain/RPi-MFRC522 and https://github.com/miguelbalboa/rfid (The Arduino rfid library)

## Install

(Do the following on your Raspberry Pi)

1. Endable SPI on Raspberry Pi

   https://www.raspberrypi-spy.co.uk/2014/08/enabling-the-spi-interface-on-the-raspberry-pi/

2. Install the bcm2835 library

   - Download the latest version of the bcm2835 library on http://www.airspayce.com/mikem/bcm2835/

   - Use `cd` to navigate to the folder where bcm2835-1.xx.tar.gz is downloaded

   - Run the following commands to install the library

   ```bash
   tar zxvf bcm2835-1.xx.tar.gz

   cd bcm2835-1.xx

   ./configure

   make

   sudo make check

   sudo make install
   ```

3. `npm install rpi-mfrc522-nodejs`

## Usage

```js
const reader = require("rpi-mfrc522-nodejs");

// readerPins are of the format [SS, RST] where SS and RST are the GPIO pins connected to the SDA and RST pins of the MFRC522s
// find more pin here: https://www.airspayce.com/mikem/bcm2835/group__constants.html#ga63c029bd6500167152db4e57736d0939
const readerPins = [
  [reader.RPI_V2_GPIO_P1_38, reader.RPI_V2_GPIO_P1_15],
  [reader.RPI_V2_GPIO_P1_40, reader.RPI_V2_GPIO_P1_18],
];

reader.onRfidChange(readerPins, data => {
  // data are of the format [UID, UID], if no card is present, null is returned instead of the UID
  // example: ["09:C4:B1:A3", null], reader 0 reads a card with uid 09:C4:B1:A3, no card read by reader 1
  console.log(data);
});
```

## Getting started

### Connecting your MFRC522 to your Raspberry Pi

1. Open https://pinout.xyz/pinout/spi in your browser as a reference.
2. Connect the 3.3v pin on the MFRC522 to any 3.3v pin on the Raspberry Pi.  
   Connect the RST pin on the MFRC522 to any GPIO pin on the Raspberry Pi.  
   Connect the GND pin on the MFRC522 to any ground pin on the Raspberry Pi.  
   Leave the IRQ pin unconnected.  
   Connect the MISO pin on the MFRC522 to the MISO pin (19) on the Raspberry Pi.  
   Connect the MOSI pin on the MFRC522 to the MOSI pin (21) on the Raspberry Pi.  
   Connect the SCK pin on the MFRC522 to the SCLK pin (23) on the Raspberry Pi.  
   Connect the SDA pin on the MFRC522 to any GPIO pin on the Raspberry Pi.  
   (GPIO pins can be any pins that are not ground or power)

### Try the example

(make sure nodejs is installed)

1. Download this repo by running `git clone https://github.com/CoolCyberBrain/RPi-MFRC522-nodejs.git`
2. Enter the folder by running `cd RPi-MFRC522-nodejs`
3. `npm install`
4. Run the example by running the following command (if you don't run in sudo the program will crash, need sudo to access the GPIO pins) `sudo node example.js`
