float vel3 = 0, oldvel3 = 0, vel2 = 0, oldvel2 = 0, vel1 = 0, oldvel1 = 0, x = 0, xx;
bool forward1, forward2;
unsigned long long int check1, check2, check3, oldcheck1, oldcheck2, oldcheck3;
int PWR = 200, pwrL, pwrR, pwrB;

int SSLEFT = 0, SSRIGHT = 0, SSBACK = 0;

float Dkol = 1.45;
long a1=0,a2=0,b1=0,b2=0;
long inkL = 0, inkR = 0, inkB = 0;
unsigned long long int t = 0, T, t1 = 0, T1, t2 = 0, T2, TT, tt = 0, ST, TS= 0;

bool EncRA=false, EncRB=false; //Правый
bool oldEncRA=false, oldEncRB=false;

bool EncLA=false, EncLB=false; //Левый
bool oldEncLA=false, oldEncLB=false;

bool EncBA=false, EncBB=false; //Jopa
bool oldEncBA=false, oldEncBB=false;

void setup() {
  Serial.begin(115200);
  t = micros();
  for (int i = 2; i <= 10; i++) {
    pinMode(i, OUTPUT);
  }

  for (int i = 11; i < 18; i++) {
    pinMode(i, INPUT);
  }


  
  if(digitalRead(11)==LOW) {oldEncLA=true;} else {oldEncLA=false;}
  if(digitalRead(12)==LOW) {oldEncLB=true;} else {oldEncLB=false;}
  if(digitalRead(16)==LOW) {oldEncRA=true;} else {oldEncRA=false;}
  if(digitalRead(17)==LOW) {oldEncRB=true;} else {oldEncRB=false;}
  if(digitalRead(18)==LOW) {oldEncBA=true;} else {oldEncBA=false;}
  if(digitalRead(19)==LOW) {oldEncBB=true;} else {oldEncBB=false;}

  oldEncLB = digitalRead(17) == LOW;

  
  digitalWrite(2, 1);
  digitalWrite(4, 0);
  digitalWrite(5, 1);
  digitalWrite(6, 0);
  digitalWrite(7, 1);
  digitalWrite(8, 0);
  
}
void check(){
  digitalWrite(2, 1);
  digitalWrite(4, 0);
  digitalWrite(5, 1);
  digitalWrite(6, 0);
  digitalWrite(7, 1);
  digitalWrite(8, 0);

  digitalWrite(3, 1);
  digitalWrite(9, 1);
  digitalWrite(10, 1);

  TT = micros();
  
  if(digitalRead(11) == HIGH){
    EncLA = true;
  }
  else EncLA = false;
  
  if(digitalRead(12) == HIGH){
    EncLB = true;
  }
  else EncLB = false;
  
  if(digitalRead(16) == HIGH){
    EncRA = true;
  }
  else EncRA = false;
  
  if(digitalRead(17) == HIGH){
    EncRB = true;
  }
  else EncRB = false;
  if(digitalRead(18) == HIGH){
    EncBA = true;
  }
  else EncBA = false;
  
  if(digitalRead(19) == HIGH){
    EncBB = true;
  }
  else EncBB = false;
  //logical
  if((EncRA == true)&&(EncRB == true)){
    if((oldEncRA == false)&&(oldEncRB == true)) check1++;
    if((oldEncRA == true)&&(oldEncRB == false)) check1--;
  }
  if((EncLA == true)&&(EncLB == true)){
    if((oldEncLA == true)&&(oldEncLB == false)) check2++;
    if((oldEncLA == false)&&(oldEncLB == true)) check2--;
  }
  if((EncBA == true)&&(EncBB == true)){
    if((oldEncBA == true)&&(oldEncBB == false)) check3++;
    if((oldEncBA == false)&&(oldEncBB == true)) check3--;
  }
  oldEncRA = EncRA;
  oldEncRB = EncRB;
  oldEncLA = EncLA;
  oldEncLB = EncLB;
  oldEncBA = EncBA;
  oldEncBB = EncBB;


  /*if (TT-tt >= 500000){
    Serial.print("Back: ");
    Serial.print(check3);
    Serial.print(" ");
    Serial.print("Left: ");
    Serial.print(check2);
    Serial.print("Right: ");
    Serial.println(check1);
    tt = micros();
  }*/
  }





void checkVel(){
  //TT = micros();
  
  if(digitalRead(11) == HIGH){
    EncLA = true;
  }
  else EncLA = false;
  
  if(digitalRead(12) == HIGH){
    EncLB = true;
  }
  else EncLB = false;
  
  if(digitalRead(16) == HIGH){
    EncRA = true;
  }
  else EncRA = false;
  
  if(digitalRead(17) == HIGH){
    EncRB = true;
  }
  else EncRB = false;
  if(digitalRead(18) == HIGH){
    EncBA = true;
  }
  else EncBA = false;
  
  if(digitalRead(19) == HIGH){
    EncBB = true;
  }
  else EncBB = false;
  //logical
  if((EncRA == true)&&(EncRB == true)){
    if((oldEncRA == false)&&(oldEncRB == true)) check1++;
    if((oldEncRA == true)&&(oldEncRB == false)) check1++;
  }
  if((EncLA == true)&&(EncLB == true)){
    if((oldEncLA == true)&&(oldEncLB == false)) check2++;
    if((oldEncLA == false)&&(oldEncLB == true)) check2++;
  }
  if((EncBA == true)&&(EncBB == true)){
    if((oldEncBA == true)&&(oldEncBB == false)) check3++;
    if((oldEncBA == false)&&(oldEncBB == true)) check3++;
  }
  oldEncRA = EncRA;
  oldEncRB = EncRB;
  oldEncLA = EncLA;
  oldEncLB = EncLB;
  oldEncBA = EncBA;
  oldEncBB = EncBB;

  if(check3 - oldcheck3>=11){
    t2 = micros() - T2;
    vel3=(1000000/(t2*4));
    oldcheck3 = check3;
    T2 = micros();
  }else if ((check3 - oldcheck3<11) && (micros() - T2 > 250000)) vel3 = 0;
  
  if(check2 - oldcheck2>=11){
    t = micros() - T;
    vel2=(1000000/(t*4));
    oldcheck2 = check2;
    T = micros();
  }else if ((check2 - oldcheck2<11) && (micros() - T > 250000)) vel2 = 0;
  
  if(check1 - oldcheck1>=11){
    t1 = micros() - T1;
    vel1=(1000000/(t1*4));
    oldcheck1 = check1;
    T1 = micros();
  }else if ((check1 - oldcheck1<11) && (micros() - T1 > 250000)) vel1 = 0;
  
  
  /*
  if (TT-tt >= 500000){
    Serial.print("Back: ");
    Serial.print(vel3);
    Serial.print(" ");
    Serial.print("Left: ");
    Serial.print(vel2);
    Serial.print(" ");
    Serial.print("Right: ");
    Serial.println(vel1);
    tt = micros();
  }
  */
}

void travel (int Lmm, int Vel){
    checkVel();
    delay(500);
    analogWrite(6, 80);
    analogWrite(9, 80);
    delay(100);
    long EncDef = check1;
    long dAlfa = abs(Lmm*360/(PI*Dkol));
    Serial.println(dAlfa);
    while((abs(check1) - EncDef) < (dAlfa - 60)){
      checkVel();
      
      if (vel1>vel2){ analogWrite(9,0);}
      else if (vel2>vel1){ analogWrite(6,0);}
      else /*(oldcheck1-check1==check2-check2)*/{
        if (vel1>Vel){ analogWrite(9,0);
        }
        else {
          analogWrite(9,255);
          analogWrite(6,255);
          } 
        if (vel2>Vel){ analogWrite(6,0);
        }
        else{
          analogWrite(9,255);
          analogWrite(6,255);
          }
      }
    }

    
    while((abs(check1) - EncDef) < (dAlfa - 3)){
      checkVel();
      
      if (vel1>vel2){ analogWrite(9,0);}
      else if (vel2>vel1){ analogWrite(6,0);}
      else /*(oldcheck1-check1==check2-check2)*/{
        if (vel1>Vel){ analogWrite(9,0);}
        else {
          analogWrite(9,80);
          analogWrite(6,80);
          } 
        if (vel2>Vel){ analogWrite(6,0);
        }
        else{
          analogWrite(9,80);
          analogWrite(6,80);
          }
      }
    }
    analogWrite(6, 255);
    analogWrite(9, 255);
    analogWrite(2, 1);
    analogWrite(3, 1);
    analogWrite(4, 1);
    analogWrite(5, 1);
    delay(5000);
  
}

void travelVel(int Lmm, int Vel){
     checkVel();
    delay(500);
    analogWrite(6, 80);
    analogWrite(9, 80);
    delay(100);
    ST = micros();
    long EncDef = check1;
    long dAlfa = abs(Lmm*360/(PI*Dkol));
    Serial.println(dAlfa);
    while(x*0.01 < (dAlfa - 60)){
      checkVel();   
      TS = micros();
      xx = vel1 * (TS-ST);
      x =x + xx;
      if (vel1>vel2){ analogWrite(9,0);}
      else if (vel2>vel1){ analogWrite(6,0);}
      else /*(oldcheck1-check1==check2-check2)*/{
        if (vel1>Vel){ analogWrite(9,0);
        }
        else {
          analogWrite(9,255);
          analogWrite(6,255);
          } 
        if (vel2>Vel){ analogWrite(6,0);
        }
        else{
          analogWrite(9,255);
          analogWrite(6,255);
          }
      }
    }

    
    while(x*0.01 < (dAlfa - 3)){
      checkVel();
      TS = micros();
      xx = vel1 * (TS-ST);
      x =x + xx;
      if (vel1>vel2){ analogWrite(9,0);}
      else if (vel2>vel1){ analogWrite(6,0);}
      else /*(oldcheck1-check1==check2-check2)*/{
        if (vel1>Vel){ analogWrite(9,0);}
        else {
          analogWrite(9,80);
          analogWrite(6,80);
          } 
        if (vel2>Vel){ analogWrite(6,0);
        }
        else{
          analogWrite(9,80);
          analogWrite(6,80);
          }
      }
    }
    analogWrite(6, 255);
    analogWrite(9, 255);
    analogWrite(2, 1);
    analogWrite(3, 1);
    analogWrite(4, 1);
    analogWrite(5, 1);
    delay(5000);
  
}

void CWLEFT(){
  digitalWrite(2, 0);
  digitalWrite(4, 1);
}
void CCWLEFT(){
  digitalWrite(2, 1);
  digitalWrite(4, 0);
}
void CWRIGHT(){
  digitalWrite(5, 1);
  digitalWrite(6, 0);
}
void CCWRIGHT(){
  digitalWrite(5, 0);
  digitalWrite(6, 1);
}
void CWBACK(){
  digitalWrite(7, 1);
  digitalWrite(8, 0);
}
void CCWBACK(){
  digitalWrite(7, 0);
  digitalWrite(8, 1);
}

void SetVel(int targetLeft, int targetRight, int targetBack){
    checkVel();
    if (targetLeft>20) pwrL = 250;
    else if (targetLeft>15) pwrL = 240;
    else if (targetLeft>10) pwrL = 220;
    else if (targetLeft>5) pwrL = 180;
    else pwrL = 150;
    
    if (targetRight>20) pwrR = 250;
    else if (targetRight>15) pwrR = 240;
    else if (targetRight>10) pwrR = 220;
    else if (targetRight>5) pwrR = 180;
    else pwrR = 150;
    
    if (targetBack>20) pwrB = 250;
    else if (targetBack>15) pwrB = 240;
    else if (targetBack>10) pwrB = 220;
    else if (targetBack>5) pwrB = 180;
    else pwrB = 150;
       if (vel1>=targetRight) analogWrite(9,0);
        else analogWrite(9,pwrR); 
       if (vel2>=targetLeft) analogWrite(3,0);
        else analogWrite(3,pwrL);
       if (vel3>=targetBack) analogWrite(10,0);
        else analogWrite(10,pwrB);
    

}


void angle(int Angle){
  
  long EncDef = inkR;
  
  while(((360*(inkR-EncDef))/(Dkol*11)) < (Angle - 5)){
      if(digitalRead(16)==LOW) EncRA=true; else EncRA=false;
      if(digitalRead(17)==LOW) EncRB=true; else EncRB=false;
      if(digitalRead(14)==LOW) EncLA=true; else EncLA=false;
      if(digitalRead(15)==LOW) EncLB=true; else EncLB=false;
      
      Serial.println((360*(inkR-EncDef))/(Dkol*11)); 
    
      if(EncRA==true && EncRB==true){
        if(oldEncRA==false && oldEncRB==true){
          inkL=inkL+1;
        }
      
        if(oldEncRA==true && oldEncRB==false){
          inkL=inkL-1;
        }
      }
      
      if(EncLA==true && EncLB==true){
        if(oldEncLA==true && oldEncLB==false){
          inkR=inkR+1;
        }
        if(oldEncLA==false && oldEncLB==true){
          inkR=inkR-1;
        }
      }
       
      
      oldEncRA=EncRA;
      oldEncRB=EncRB;
      oldEncLA=EncLA;
      oldEncLB=EncLB;
    
      if(inkR>inkL) analogWrite(6,0);
      if(inkL>inkR) analogWrite(9,0);
      if(inkL==inkR){
        analogWrite(6,PWR);
        analogWrite(9,PWR);
      }
    }
    while(((360*(inkR-EncDef))/(120)) < (Angle - 2)){
      if(digitalRead(16)==LOW) EncRA=true; else EncRA=false;
      if(digitalRead(17)==LOW) EncRB=true; else EncRB=false;
      if(digitalRead(14)==LOW) EncLA=true; else EncLA=false;
      if(digitalRead(15)==LOW) EncLB=true; else EncLB=false;
    
    
      if(EncRA==true && EncRB==true){
        if(oldEncRA==false && oldEncRB==true){
          inkL=inkL+1;
        }
      
        if(oldEncRA==true && oldEncRB==false){
          inkL=inkL-1;
        }
      }
      
      if(EncLA==true && EncLB==true){
        if(oldEncLA==true && oldEncLB==false){
          inkR=inkR+1;
        }
        if(oldEncLA==false && oldEncLB==true){
          inkR=inkR-1;
        }
      }
       
      
      oldEncRA=EncRA;
      oldEncRB=EncRB;
      oldEncLA=EncLA;
      oldEncLB=EncLB;

      analogWrite(6, 80);
      analogWrite(9, 80);
    }
    analogWrite(6, 255);
    analogWrite(9, 255);
    analogWrite(2, 1);
    analogWrite(3, 1);
    analogWrite(4, 1);
    analogWrite(5, 1);
    delay(5000);
}
  


void moveEnc(int Lmm, byte PWR){
    analogWrite(6, 80);
    analogWrite(9, 80);
    delay(200);
    long EncDef = inkR;
    long dAlfa = abs(Lmm*360/(PI*Dkol));
    while((abs(inkR)-EncDef) < (dAlfa - 60)){
      if(digitalRead(16)==LOW) EncRA=true; else EncRA=false;
      if(digitalRead(17)==LOW) EncRB=true; else EncRB=false;
      if(digitalRead(14)==LOW) EncLA=true; else EncLA=false;
      if(digitalRead(15)==LOW) EncLB=true; else EncLB=false;
    
      if(EncRA==true && EncRB==true){
        if(oldEncRA==false && oldEncRB==true){
          inkL=inkL+1;
        }
      
        if(oldEncRA==true && oldEncRB==false){
          inkL=inkL-1;
        }
      }
      
      if(EncLA==true && EncLB==true){
        if(oldEncLA==true && oldEncLB==false){
          inkR=inkR+1;
        }
        if(oldEncLA==false && oldEncLB==true){
          inkR=inkR-1;
        }
      }
       
      
      oldEncRA=EncRA;
      oldEncRB=EncRB;
      oldEncLA=EncLA;
      oldEncLB=EncLB;
    
      if(inkR>inkL) analogWrite(9,0);
      if(inkL>inkR) analogWrite(6,0);
      if(inkL==inkR){
        analogWrite(6,PWR);
        analogWrite(9,PWR);
      }
    }
    while(abs(inkR - EncDef) < (dAlfa - 3)){
      if(digitalRead(16)==LOW) EncRA=true; else EncRA=false;
      if(digitalRead(17)==LOW) EncRB=true; else EncRB=false;
      if(digitalRead(14)==LOW) EncLA=true; else EncLA=false;
      if(digitalRead(12)==LOW) EncLB=true; else EncLB=false;
    
    
      if(EncRA==true && EncRB==true){
        if(oldEncRA==false && oldEncRB==true){
          inkL=inkL+1;
        }
      
        if(oldEncRA==true && oldEncRB==false){
          inkL=inkL-1;
        }
      }
      
      if(EncLA==true && EncLB==true){
        if(oldEncLA==true && oldEncLB==false){
          inkR=inkR+1;
        }
        if(oldEncLA==false && oldEncLB==true){
          inkR=inkR-1;
        }
      }
       
      
      oldEncRA=EncRA;
      oldEncRB=EncRB;
      oldEncLA=EncLA;
      oldEncLB=EncLB;

      analogWrite(6, 80);
      analogWrite(9, 80);
    }
    analogWrite(6, 255);
    analogWrite(9, 255);
    analogWrite(2, 1);
    analogWrite(3, 1);
    analogWrite(4, 1);
    analogWrite(5, 1);
    delay(5000);
  }

// incomingByte & 32)>>5

void sread(){
  int incomingByte = 0;
  while (Serial.available()>0){
    incomingByte = Serial.read();
    switch((incomingByte & 192)>>6){ //dviz
      case (0):
      {
        if (((incomingByte & 32)>>5) == 1) CCWLEFT(); else CWLEFT();
        SSLEFT = (incomingByte & 31);
        break;
        }
      case (1):
      {
        if (((incomingByte & 32)>>5) == 1) CCWRIGHT(); else CWRIGHT();
        SSRIGHT = (incomingByte & 31);
        break;
        }
      case (2):
      {
        if (((incomingByte & 32)>>5) == 1) CCWBACK(); else CWBACK();
        SSBACK = (incomingByte & 31);
        break;
        }
      
      }
        Serial.print(incomingByte, BIN);
    }
  }

void loop() {
  
  //check();
  //checkVel();
  sread();
  SetVel(SSLEFT, SSRIGHT, SSBACK);
  //moveEnc(10000, 255);
  //travel(10000, 25);
  //travelVel(10000, 25);
  //angle(180);
  
}  
