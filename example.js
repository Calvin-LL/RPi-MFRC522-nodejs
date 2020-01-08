const reader = require("./index");

reader.onRfidChange([], data => {
  console.log(data);
});
