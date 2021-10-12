#include "BarcodeReader.h"

const unsigned int MAX_RETRIES = 5;

int currentTry = 0;
bool barcodeWasRead = false; 

BarcodeReader barcodeReader;

// setup

void setup() {
  Serial.println('Setup....');
  Serial.begin(115200);
  delay(2);
  
  Serial.println("Reading barcode data...");
  barcodeReader.initializeBarcodeReader();
  
}

// loop

void loop() {

  if (!barcodeWasRead && currentTry < MAX_RETRIES){
    Serial.print("Reading barcode data...");
    currentTry++;
    String barcodeData = barcodeReader.scanBarCode();
  
    if (barcodeData.length() > 0) {
      barcodeWasRead = true;
      Serial.print("received: ");
      Serial.println(barcodeData);
    } else {
      Serial.println("no data received.");
    }    
  } else {
      // Enter deep sleep mode
      Serial.println("Entering Deep Sleep");
      barcodeReader.beginDeepSleep();      

    // simulate wait for wakeup
    delay(10000);

    Serial.println("Wakeup Barcode Reader");
    barcodeReader.wakeUp();

    currentTry = 0;
    barcodeWasRead = false;
  }
}
