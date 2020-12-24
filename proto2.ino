#include <PCF8563.h>
#include <SPI.h>
#include <SD.h>

//상태LED
#define PIN_GREEN 4
#define PIN_YELLOW 3
#define PIN_RED 2

//전압입력 핀
#define PIN_A A3
#define PIN_B A2
#define PIN_C A1
#define PIN_D A0
float Volt_arr[4];

//버튼핀
#define PIN_BTN 5
bool wait_btn = false;

PCF8563 pcf;
File myFile;
Time nowTime;

uint32_t cnt_mil = 0;
uint32_t cnt_offset = 1000;
uint32_t cnt_stack = 0;

bool temp_stat = false;
bool record_mode = false;

void measureVoltages(){
    Volt_arr[0] = analogRead(PIN_A) * 25;
    Volt_arr[1] = analogRead(PIN_B) * 25;
    Volt_arr[2] = analogRead(PIN_C) * 25;
    Volt_arr[3] = analogRead(PIN_D) * 25;

    Volt_arr[0] /= 1024; 
    Volt_arr[1] /= 1024; 
    Volt_arr[2] /= 1024; 
    Volt_arr[3] /= 1024;
}

void printVoltages(){
    Serial.print(nowTime.year);
    Serial.print('/');
    Serial.print(nowTime.month);
    Serial.print('/');
    Serial.print(nowTime.day);
    Serial.print(' ');
    Serial.print(nowTime.hour);
    Serial.print(':');
    Serial.print(nowTime.minute);
    Serial.print(':');
    Serial.print(nowTime.second);
    Serial.print(" ; \t");
    Serial.print(Volt_arr[0]);
    Serial.print(", ");
    Serial.print(Volt_arr[1]);
    Serial.print(", ");
    Serial.print(Volt_arr[2]);
    Serial.print(", ");
    Serial.println(Volt_arr[3]);
}
void SDVoltages(){
    myFile.print(nowTime.year);
    myFile.print('/');
    myFile.print(nowTime.month);
    myFile.print('/');
    myFile.print(nowTime.day);
    myFile.print(' ');
    myFile.print(nowTime.hour);
    myFile.print(':');
    myFile.print(nowTime.minute);
    myFile.print(':');
    myFile.print(nowTime.second);
    myFile.print(" ; \t");
    myFile.print(Volt_arr[0]);
    myFile.print(", ");
    myFile.print(Volt_arr[1]);
    myFile.print(", ");
    myFile.print(Volt_arr[2]);
    myFile.print(", ");
    myFile.println(Volt_arr[3]);
}

void setup() {
  Serial.begin(9600);
  SD.begin(10);

  //LED 핀 선언
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_YELLOW, OUTPUT);
  pinMode(PIN_RED, OUTPUT);
  
  //전압측정 핀모드 선언
  pinMode(PIN_A, INPUT);
  pinMode(PIN_B, INPUT);
  pinMode(PIN_C, INPUT);
  pinMode(PIN_D, INPUT);

  //버튼 핀 선언
  pinMode(PIN_BTN, INPUT_PULLUP);

  digitalWrite(PIN_GREEN, HIGH);
  
  pcf.init();//initialize the clock

//  pcf.setYear(20);//set year
//  pcf.setMonth(12);//set month
//  pcf.setDay(16);//set dat
//  pcf.setHour(10);//set hour
//  pcf.setMinut(46);//set minut
//  pcf.setSecond(0);//set second

  pcf.startClock();//start the clock
  
  Time nowTime = pcf.getTime();
}

void loop() {
  nowTime = pcf.getTime();

  if(!digitalRead(PIN_BTN)){
    if(!wait_btn){
      wait_btn = true;
      record_mode = !record_mode;
      if(record_mode){
        myFile = SD.open("rec.log", FILE_WRITE);
      }
      else {
        myFile.close();
      }
      digitalWrite(PIN_RED, record_mode);
    }
  }
  else {
    wait_btn = false;
  }

  if (record_mode){
    if ( cnt_mil < millis()) { //정해진 시간마다 동작 실행
      digitalWrite(PIN_YELLOW, temp_stat = !temp_stat);
      cnt_mil += cnt_offset;
      if (cnt_stack > 5) {
        myFile.close();
        myFile = SD.open("rec.log", FILE_WRITE);
      }
      else cnt_stack += 1;

      measureVoltages();
      SDVoltages();
    }
  }
}
