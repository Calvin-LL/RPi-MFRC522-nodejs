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

string uint8ArrayToHexString(const uint8_t* v, const size_t s) {
  stringstream ss;

  ss << hex << setfill('0');

  for (size_t i = 0; i < s; i++) {
    ss << ":" << hex << setw(2) << static_cast<int>(v[i]);
  }

  string result = ss.str().erase(0, 1);  // remove first ":"

  for (auto& c : result)  // to uppercase
    c = toupper(c);

  return result;
}

Array vectorToNapiArray(Napi::Env env, const vector<string>& data) {
  size_t size = data.size();
  Array napiData = Array::New(env, size);

  for (size_t i = 0; i < size; i++) {
    if (data[i] == "")
      napiData[i] = env.Null();
    else
      napiData[i] = String::New(env, data[i]);
  }

  return napiData;
}

class RfidLoopWorder : public AsyncProgressWorker<vector<string>> {
 public:
  RfidLoopWorder(Array data, Function cb) : AsyncProgressWorker(cb) {
    // Process parameters
    for (size_t i = 0; i < data.Length(); i++) {
      Array pins = ((Value)data[i]).As<Array>();

      Value ssPinValue = pins[(uint32_t)0];
      Value rstPinValue = pins[(uint32_t)1];

      int ssPin = ssPinValue.As<Number>();
      int rstPin = rstPinValue.As<Number>();

      MFRC522 mfrc522 = MFRC522(ssPin, rstPin);

      mfrc522.PCD_Init();

      readers.push_back(mfrc522);
    }
  }

 protected:
  void Execute(const ExecutionProgress& cb) override {
    size_t readerSize = readers.size();

    // Execution loop
    while (1) {
      vector<string> uidStrings;

      for (size_t reader = 0; reader < readerSize; reader++) {
        // Look for new cards
        readers[reader].PCD_AntennaOn();

        if ((readers[reader].PICC_IsNewCardPresent() ||
             readers[reader].PICC_IsNewCardPresent()) &&
            readers[reader].PICC_ReadCardSerial()) {
          string uidString = uint8ArrayToHexString(readers[reader].uid.uidByte,
                                                   readers[reader].uid.size);

          uidStrings.push_back(uidString);
        } else {
          string empty;

          uidStrings.push_back(empty);
        }
        readers[reader].PCD_AntennaOff();

        bcm2835_delay(50);
      }

      if (prevUidStrings != uidStrings)
        cb.Send(&uidStrings, 1);  // Send data to OnProgress

      prevUidStrings = uidStrings;
    }
  }

  void OnProgress(const vector<string>* data, size_t count) override {
    Array napiData = vectorToNapiArray(Env(), *data);

    Callback().Call({napiData});  // Pass data to js
  }

  void OnOK() override {}

 private:
  vector<MFRC522> readers;
  vector<string> prevUidStrings;
};

void Method(const CallbackInfo& info) {
  Array data = info[0].As<Array>();
  Function cb = info[1].As<Function>();

  if (data.Length() > 0) {
    RfidLoopWorder* worker = new RfidLoopWorder(data, cb);
    worker->Queue();
  }
}

Object Init(Env env, Object exports) {
  exports.Set(String::New(env, "onRfidChange"), Function::New(env, Method));
  return exports;
}

NODE_API_MODULE(reader, Init);
