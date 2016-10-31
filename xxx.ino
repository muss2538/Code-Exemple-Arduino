
int score = 0;
struct ExA
{
  int ledgreen,ledred,state;
}

ExA[5];

void setup() {
  

}

void loop() {

  inspect(1,2,3,1,4);

}

void inspect(byte a1,byte a2,byte a3,byte a4,byte a5){
  for(int i=1;i<=13;i++){
    digitalWrite(ExA[i].ledred,0);
    digitalWrite(ExA[i].ledgreen,1);
  }
  if(a1 == 1){
    score++;    ExA[0].state=0;
    digitalWrite(ExA[0].ledgreen,ExA[0].state);
    digitalWrite(ExA[0].ledred,!ExA[0].state);
  }
  if(a2 == 1){
    score++;    ExA[1].state=0;
    digitalWrite(ExA[1].ledgreen,ExA[1].state);
    digitalWrite(ExA[1].ledred,!ExA[1].state);
  }

  if(a3 == 1){
    score++;    ExA[2].state=0;
    digitalWrite(ExA[2].ledgreen,ExA[2].state);
    digitalWrite(ExA[2].ledred,!ExA[2].state);
  }

  if(a4 == 1){
    score++;    ExA[3].state=0;
    digitalWrite(ExA[3].ledgreen,ExA[3].state);
    digitalWrite(ExA[3].ledred,!ExA[3].state);
  }

  if(a5 == 1){
    score++;    ExA[4].state=0;
    digitalWrite(ExA[4].ledgreen,ExA[4].state);
    digitalWrite(ExA[4].ledred,!ExA[4].state);
  }
  
  
}

