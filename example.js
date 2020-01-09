const reader = require("./index");

reader.onRfidChange(
  [
    [reader.RPI_V2_GPIO_P1_38, reader.RPI_V2_GPIO_P1_15],
    [reader.RPI_V2_GPIO_P1_40, reader.RPI_V2_GPIO_P1_18],
  ],
  data => {
    console.log(data);
  }
);
