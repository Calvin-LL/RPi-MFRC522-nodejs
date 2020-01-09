{
  "targets": [
    {
      "target_name": "reader",
      "cflags!": [
        "-fno-exceptions"
      ],
      "cflags_cc!": [
        "-fno-exceptions"
      ],
      "cflags_cc": [
        "-std=c++17"
      ],
      "sources": [
        "./src/reader.cpp",

        "./src/RPi-MFRC522/src/MFRC522.cpp",
        "./src/RPi-MFRC522/src/MFRC522Extended.cpp",


        "./src/RPi-MFRC522/src/RPi-Arduino-SPI/Arduino.cpp",
         "./src/RPi-MFRC522/src/RPi-Arduino-SPI/Serial.cpp",
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "./src/RPi-MFRC522/src",
        "./src/RPi-MFRC522/src/RPi-Arduino-SPI"
      ],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      "libraries": [
        "-lbcm2835"
      ]
    }
  ]
}
