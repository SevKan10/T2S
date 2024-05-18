#define R1 13
#define R2 12
#define R3 14
#define R4 27
#define R_water 26
#define R_phenol 25
#define R_base 33
#define R_acid 32

void setup() {

  pinMode(R_acid, OUTPUT);
  pinMode(R_base, OUTPUT);
  pinMode(R_phenol, OUTPUT);
  pinMode(R_water, OUTPUT);
}

void loop() {
  //digitalWrite(R_acid,1);
  digitalWrite(R_base,1);
}
