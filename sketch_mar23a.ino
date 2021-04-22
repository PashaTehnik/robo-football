int tmpAL, tmpBL, tmpAR, tmpBR, QuarterI;
bool StateAL, StateBL, StateAR, StateBR;

void setup() {
  Serial.begin(115200);
  pinMode(14,INPUT);
  pinMode(15,INPUT);
  pinMode(16,INPUT);
  pinMode(17,INPUT);

  

}

void loop() {
  tmpAL = digitalRead(14);
  tmpBL = digitalRead(15);
  tmpAR = digitalRead(16);
  tmpBR = digitalRead(17);
  if(StateAL){
    if(tmpAL == LOW){
      StateAL = false;
      QuarterI++;
    }
    else{
      if(tmpAL == HIGH){
        StateAL = true;
        QuarterI++;
      }
    }
  }
  
  
  Serial.println(QuarterI);

}
