int reada2,reada3;

char ReadDataChoice(){
  reada2 = analogRead(A2);
  if((reada2 >= 565)&&(reada2 <= 568))return 'A';
  if((reada2 >= 565)&&(reada2 <= 568))return 'B';
  if((reada2 >= 565)&&(reada2 <= 568))return 'C';
  if((reada2 >= 565)&&(reada2 <= 568))return 'D';
}
char ReadDataControl(){
  reada3 = analogRead(A3);
  if((reada2 >= 565)&&(reada2 <= 568))return '';
  if((reada2 >= 565)&&(reada2 <= 568))return 'B';
  if((reada2 >= 565)&&(reada2 <= 568))return 'C';
  if((reada2 >= 565)&&(reada2 <= 568))return 'D';
}

void setup(){
  Serial.begin(9600);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  for(int pind=8;pind<=15;pind++){
    pinMode(pind,OUTPUT);}
}

void loop(){
  
}
