#define StepUEN 9    //EN
#define StepUDir 8   //CW
#define StepUPul 10  //CLK
#define InBorad 11

void setup() {
  pinMode(InBorad,INPUT_PULLUP);
  pinMode(StepUPul,OUTPUT);digitalWrite(StepUPul,LOW);
  pinMode(StepUDir,OUTPUT);digitalWrite(StepUDir,LOW);
  pinMode(StepUEN,OUTPUT);digitalWrite(StepUEN,LOW);
}

void loop() {
  if(digitalRead(InBorad)==LOW){
    digitalWrite(StepUDir,HIGH);
    lap(200);
    delay(1000);
    digitalWrite(StepUDir,LOW);
    lap(200);
    delay(1000);
  }
  

}

void lap(int lapp){
  for(int a=0;a<lapp;a++){
    digitalWrite(StepUPul,HIGH);
    delayMicroseconds(800);
    digitalWrite(StepUPul,LOW);
    delayMicroseconds(800);
  }
}

