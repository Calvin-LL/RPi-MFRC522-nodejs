const reader = require("./index");
// use const reader = require("rpi-mfrc522-nodejs"); in your code

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
