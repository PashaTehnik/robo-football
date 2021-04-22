float vel2 = 0, oldvel2 = 0, vel1 = 0, oldvel1 = 0, x = 0, xx;
bool forward1, forward2;
int check1, check2, oldcheck1, oldcheck2;
int PWR = 200;
float Dkol = 1.45;
long a1=0,a2=0,b1=0,b2=0;
long inkL = 0, inkR = 0;
unsigned long long int t = 0, T, t1 = 0, T1, TT, tt = 0, ST, TS= 0;

bool EncRA=false, EncRB=false; //Правый
bool oldEncRA=false, oldEncRB=false;

bool EncLA=false, EncLB=false; //Левый
bool oldEncLA=false, oldEncLB=false;

void setup() {
  Serial.begin(115200);
  t = micros();
  for (int i = 2; i <= 6; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(9, OUTPUT);

  for (int i = 14; i < 18; i++) {
    pinMode(i, INPUT);
  }

  digitalWrite(2, 1);
  digitalWrite(3, 0);
  digitalWrite(4, 0);
  digitalWrite(5, 1);
  
  if(digitalRead(14)==LOW) {oldEncRA=true;} else {oldEncRA=false;}
  if(digitalRead(15)==LOW) {oldEncRB=true;} else {oldEncRB=false;}
  if(digitalRead(16)==LOW) {oldEncLA=true;} else {oldEncLA=false;}
  //if(digitalRead(17)==LOW) {oldEncLB=true;} else {oldEncLB=false;}

  oldEncLB = digitalRead(17) == LOW;
  
}

void checkVel(){
  /*
  digitalWrite(2, 1);
  digitalWrite(3, 0);
  digitalWrite(4, 0);
  digitalWrite(5, 1);
  analogWrite(6, PWR);
  analogWrite(9, PWR);*/

  TT = micros();
  
  if(digitalRead(14) == HIGH){
    EncLA = true;
  }
  else EncLA = false;
  
  if(digitalRead(15) == HIGH){
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
  //logical
  if((EncRA == true)&&(EncRB == true)){
    if((oldEncRA == false)&&(oldEncRB == true)) check1++;
    if((oldEncRA == true)&&(oldEncRB == false)) check1--;
  }
  if((EncLA == true)&&(EncLB == true)){
    if((oldEncLA == true)&&(oldEncLB == false)) check2++;
    if((oldEncLA == false)&&(oldEncLB == true)) check2--;
  }
  oldEncRA = EncRA;
  oldEncRB = EncRB;
  oldEncLA = EncLA;
  oldEncLB = EncLB;
  
  if(check2 - oldcheck2>=11){
    t = micros() - T;
    vel2=(1000000/(t*4));
    oldcheck2 = check2;
    T = micros();
  }
  
  if(check1 - oldcheck1>=11){
    t1 = micros() - T1;
    vel1=(1000000/(t1*4));
    oldcheck1 = check1;
    T1 = micros();
  }
  /*
  if (TT-tt >= 500000){
    Serial.print("Left: ");
    Serial.print(vel2);
    Serial.print(" ");
    Serial.print("Right: ");
    Serial.println(vel1);
    tt = micros();
  }*/
  
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

void loop() {
  /*t = micros();
  if (Serial.available() > 0){
      Serial.read();
      Serial.print(inkL);Serial.print('|');Serial.println(inkR);
    }
  

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

  if(inkR>inkL) analogWrite(6,0);
  if(inkL>inkR) analogWrite(9,0);
  if(inkL==inkR){
    analogWrite(6,PWR);
    analogWrite(9,PWR);
  }
  
  
  T=millis();
  
  if (T>t){
  
  a1 = inkL;
  a1 = a1 - a2;
  
  b1 = inkR;
  b1 = b1 - b2;
  
  a2 = inkL;
  b2 = inkR;
  t = t + 1000;

/*if (mic - printT >= 500000) {
printT = micros();
Serial.println(inkL);
}
  //}*/
  //checkVel();
  //moveEnc(10000, 255);
  travel(10000, 25);
  //travelVel(10000, 25);
  //angle(180);
  
}  
