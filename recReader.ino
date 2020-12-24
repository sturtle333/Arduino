
#include <SPI.h>
#include <SD.h>

File myFile;

void setup() {
  Serial.begin(9600);

  SD.begin(10);

  myFile = SD.open("rec.log");
  
  if (myFile) {
    Serial.println("rec.log:");

    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
  } 
  else {
    Serial.println("error opening rec.log");
  }
  //SD.remove("rec.log");
}

void loop() {
}
