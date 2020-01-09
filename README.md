# RPi-MFRC522
Raspberry Pi C++ library for MFRC522 and other RFID RC522 based modules.

Read and write different types of Radio-Frequency IDentification (RFID) cards
on your Raspberry Pi using a RC522 based reader connected via the Serial Peripheral
Interface (SPI) interface.

Adapted from https://github.com/miguelbalboa/rfid
Inspired by https://github.com/paguz/RPi-RFID and many of its forks

This simply uses the same methods as https://github.com/miguelbalboa/rfid but mocks all Arduino only method with the bcm2835 library for raspberry pi.

Has the same methods, signatures, functions, and limitations as https://github.com/miguelbalboa/rfid (at least it should)

## Getting started
(Do the following on your Raspberry Pi)
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
   (GPIO pins are just pins that are not ground or power) 

### Endable SPI on Raspberry Pi
https://www.raspberrypi-spy.co.uk/2014/08/enabling-the-spi-interface-on-the-raspberry-pi/

### Install the bcm2835 library
1. Download the latest version of the bcm2835 library on http://www.airspayce.com/mikem/bcm2835/

2. Use ```cd``` to navigate to the folder where bcm2835-1.xx.tar.gz is downloaded

3. Run the following commands to install the library
```bash
tar zxvf bcm2835-1.xx.tar.gz

cd bcm2835-1.xx

./configure

make

sudo make check

sudo make install
```

### Try an example
1. Download this repo by running ```git clone https://github.com/CoolCyberBrain/RPi-MFRC522.git```
2. Enter the folder by running ```cd RPi-MFRC522```
3. Build the ReadNUID example by running the following command
```bash
g++ src/**/*.cpp src/*.cpp examples/ReadUidMultiReader/ReadUidMultiReader.cpp -I src -I src/RPi-Arduino-SPI -std=c++17 -lbcm2835
```
4. Run the example by running the following command (if you don't run in sudo the program will crash, need sudo to access the GPIO pins)
```bash
sudo ./a.out
```
