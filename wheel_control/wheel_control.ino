#define enA 9
#define in1 7
#define in2 6

int rotDirection = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  // Set initial rotation direction
  digitalWrite(in1, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
  int speed = 100;
  analogWrite(enA, speed);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  delay(500);
  

}
