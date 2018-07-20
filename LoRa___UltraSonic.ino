#include <SPI.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <LoRa.h>

#define SW1 8
#define SW2 7
#define SW3 4
//SPI LoRa
#define NSS_PIN    10
#define NRESET_PIN 9
#define DIO0_PIN   2
//UltraSonic
#define trigPin    5
#define echoPin    6

#define Lmin 80

unsigned long TimeDeviceRun;
unsigned long TimeCurrent = 0;
unsigned long Milli;
long duration;
int distance;
int MinuteTop,SecondTop,MillisecTop;
int Minute,Second,Millisec;
byte State = 0;             //1 = เริ่มนับ 2 = หยุดนับ  0 = รีเซ็ทและหยุดนับ
String Head;
String DataIn;
String Lead;
int DataL;

long TopPoint;
long Point;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600); lcd.begin();
  Serial.println("LoRa Receiver");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  
  LoRa.setPins(NSS_PIN, NRESET_PIN, DIO0_PIN);
  //LoRa.setTxPower(17);
  while(!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    delay(1500);
  }
  lcd.print("Open Device");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0); lcd.print("Best");    lcd.setCursor(8,0); TimeString(MinuteTop,SecondTop,MillisecTop);
  lcd.setCursor(0,1); lcd.print("Current"); lcd.setCursor(8,1); TimeString(Minute,Second,Millisec);
}


void loop() {

  DataL = CM();
  Serial.println(DataL);
  onReceive(LoRa.parsePacket());
  TimeDeviceRun = millis();
  if(digitalRead(SW1)==LOW){
    State = 0;
    sendLoRa("RxT:Send Test:TxR");
    delay(1000);
  }
  if(digitalRead(SW2)==LOW){
    State = 0;
    sendLoRa("RxT:Send Test:TxR");
    delay(1000);
  }
  if(digitalRead(SW3)==LOW){
    State = 0;
    sendLoRa("RxT:Send Test:TxR");
    delay(1000);
  }
  
  if((DataL <= Lmin)&&(State != 1)){
    State = 1;
    TimeCurrent = TimeDeviceRun;
  }
  if((DataIn == "STOP")&&(State == 1)){
    State = 2;
    delay(1000);
    DataIn = "";
    
  }
  if(DataIn == "Connect M1"){
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Connect To LaRo");
    delay(2000);
    lcd.clear();
    State = 3;
    DataIn = "";
    
  }
  Timer();
  delay(100);
  
}

int CM(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  return(distance);
}
String getValue(String data, char separator, int index){
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void TimeString(int mm,int s,int ms){

  lcd.print(mm/10);
  lcd.print(mm%10);
  lcd.print(":");
  lcd.print(s/10);
  lcd.print(s%10);
  lcd.print(":");
  lcd.print(ms/100);
  lcd.print(ms%10);
  lcd.print(" ");
}
void sendLoRa(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
}
void Timer(){
  
  if(State == 0){//Restart
    Millisec = 0;
    Second = 0;
    Minute = 0;
    lcd.setCursor(8,1); TimeString(Minute,Second,Millisec);
  }
  else if(State == 1){//นับเวลา
    Milli = TimeDeviceRun - TimeCurrent;
    Millisec = Milli-((Minute*60*1000)+(Second*1000));
    Second = (Milli/1000)-(Minute*60);
    Minute = (Second/60);
    Serial.print(Minute); Serial.print(":");Serial.print(Second);Serial.print(":");Serial.println(Millisec);  
    lcd.setCursor(8,1); TimeString(Minute,Second,Millisec);
    delay(10);
  }
  else if(State == 2){//เปรียบเทียบเวลาที่ดีที่สุด
    TopPoint = (MinuteTop*60*1000)+(SecondTop*1000)+MillisecTop;
    Point = (Minute*60*1000)+(Second*1000)+Millisec;
    if(TopPoint == 0){
      MinuteTop = Minute;
      SecondTop = Second;
      MillisecTop = Millisec;
    }
    if(Point<TopPoint){
      MinuteTop = Minute;
      SecondTop = Second;
      MillisecTop = Millisec;
    }
    
    lcd.setCursor(0,0); lcd.print("Best");    lcd.setCursor(8,0); TimeString(MinuteTop,SecondTop,MillisecTop);
    lcd.setCursor(0,1); lcd.print("Current"); lcd.setCursor(8,1); TimeString(Minute,Second,Millisec);
    delay(1000);
  }
  else if(State == 3){//หน้าจอปกติ
    lcd.setCursor(0,0); lcd.print("Best");    lcd.setCursor(8,0); TimeString(MinuteTop,SecondTop,MillisecTop);
    lcd.setCursor(0,1); lcd.print("Current"); lcd.setCursor(8,1); TimeString(Minute,Second,Millisec);
    State = 0;
  }
  else if(State == 4){//All Zero
    MinuteTop = 0,SecondTop = 0,MillisecTop = 0;
    Minute = 0,Second = 0,Millisec = 0;
    State = 3;
  }
  else if(State == 5){
    
  }
}
void onReceive(int packetSize) {
  if (packetSize != 0){
  // read packet header bytes:
  String incoming = "";
  while (LoRa.available()) {
    incoming += (char)LoRa.read();

  }
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Head = getValue(incoming,':',0);
  Lead = getValue(incoming,':',2);

  if((Head == "RxT") && (Lead == "TxR")){
    DataIn = getValue(incoming,':',1);
  }

  }
}
