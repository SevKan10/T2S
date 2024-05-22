#define R1 13
#define R2 12
#define R3 14
#define R_sti 27
#define R_water 26
#define R_phenol 25
#define R_base 33
#define R_acid 32

void setup() {

  pinMode(R_acid, OUTPUT);
  pinMode(R_base, OUTPUT);
  pinMode(R_phenol, OUTPUT);
  pinMode(R_water, OUTPUT);
  pinMode(R_sti, OUTPUT);
}

void loop() {
  digitalWrite(R_acid,1);
  delay(250);
  digitalWrite(R_acid,0);
  delay(2000);
  
  // digitalWrite(R_base,1);
  // delay(250);
  // digitalWrite(R_base,0);
  // delay(2000);
  
  digitalWrite(R_phenol,1);
  delay(250);
  digitalWrite(R_phenol,0);
  delay(2000); 
}
