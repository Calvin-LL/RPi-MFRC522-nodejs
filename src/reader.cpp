#include <array>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

#include "MFRC522.h"
#include "napi.h"

using namespace std;
using namespace Napi;

string uint8ArrayToHexString(const uint8_t *v, const size_t s) {
  stringstream ss;

  ss << hex << setfill('0');

  for (size_t i = 0; i < s; i++) {
    ss << "-" << hex << setw(2) << static_cast<int>(v[i]);
  }

  string result = ss.str().erase(0, 1);  // remove first "-"

  for (auto &c : result)  // to uppercase
    c = toupper(c);

  return result;
}

class RfidLoopWorder : public AsyncProgressWorker<Array> {
 public:
  RfidLoopWorder(Array data, Function cb) : AsyncProgressWorker(cb) {
    for (size_t i = 0; i < data.Length(); i++) {
      Array pins = data[i].As<Array>();

      readers.push_back(MFRC522(pins[0], pins[1]));
    }
  }

 protected:
  void Execute(const ExecutionProgress &cb) override {
    size_t readerSize = readers.size();
    while (1) {
      Array uidStrings = Array::New(Env(), readerSize);

      for (size_t reader = 0; reader < readerSize; reader++) {
        // Look for new cards
        readers[reader].PCD_AntennaOn();

        if ((readers[reader].PICC_IsNewCardPresent() ||
             readers[reader].PICC_IsNewCardPresent()) &&
            readers[reader].PICC_ReadCardSerial()) {
          string uidString = uint8ArrayToHexString(readers[reader].uid.uidByte,
                                                   readers[reader].uid.size);

          uidStrings[reader] = String::New(Env(), uidString);
        } else {
          uidStrings[reader] = Env::Null();
        }
        readers[reader].PCD_AntennaOff();

        bcm2835_delay(50);
      }

      cb.Send(&uidStrings, 1);
    }
  }

  void OnProgress(const Array *data, size_t count) override {
    HandleScope scope(Env());
    Callback().Call({data});
  }

  void OnOK() override {}

 private:
  vector<MFRC522> readers;
};

void Method(const CallbackInfo &info) {
  Array data = info[0].As<Array>();
  Function cb = info[1].As<Function>();

  if (data.length() > 0) {
    RfidLoopWorder *worker = new RfidLoopWorder(data, cb);
    worker->Queue();
  }
}

Object Init(Env env, Object exports) {
  exports.Set(String::New(env, "onRfidChange"), Function::New(env, Method));
  return exports;
}

NODE_API_MODULE(hello, Init);