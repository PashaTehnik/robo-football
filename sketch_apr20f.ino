byte vhod, gg = 0;
int dvizh, n, p;

void setup() {
  Serial.begin(115200);
}

void loop() {
  while (Serial.available() > 0) {
    delay(10);
    vhod = Serial.read();
    gg = gg*10 + (vhod - 48);
    
    for (int i = 7; i >= 0; i--) {
      Serial.print(gg >> i & 1);
    }
    Serial.println();
  }
  dvizh = (192&gg)>>6;
  n= (32&gg)>>5;
  p = (31&gg);
  if (dvizh == 0b00|| dvizh ==0b01) {
    if (n == 0) {
      digitalWrite(2, 1);
      digitalWrite(3, 0);
      analogWrite(6, p * 8);
    }
    else {
      digitalWrite(2, 0);
      digitalWrite(3, 1);
      analogWrite(6, p * 8);
    }
  }
  else {
    Serial.print(dvizh, BIN);
    if (n == 0) {
      digitalWrite(4, 0);
      digitalWrite(5, 1);
      analogWrite(9, p * 8);
    }
    else {
      digitalWrite(4, 1);
      digitalWrite(5, 0);
      analogWrite(9, p * 8);
    }
  }
  gg = 0b00000000;
  dvizh = 0;
  n = 0;
  p = 0;
}
