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
        "<!@(node -p \"require('fs').readdirSync('./src').map(f=>'src/'+f).join(' ')\")",
        "<!@(node -p \"require('fs').readdirSync('./src/RPi-MFRC522/src').map(f=>'src/'+f).join(' ')\")",
        "<!@(node -p \"require('fs').readdirSync('./src/RPi-MFRC522/src/RPi-Arduino-SPI').map(f=>'src/'+f).join(' ')\")"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "libraries": [
        "-lbcm2835"
      ]
    }
  ]
}
