// Define IR sensor pins
#define IR_LEFT 2
#define IR_RIGHT 3

// Motor A (Right side motors)
#define ENA 6
#define IN1 7
#define IN2 8

// Motor B (Left side motors)
#define ENB 11
#define IN3 9
#define IN4 10

void setup() {
  // Set motor pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Set IR sensor pins as inputs
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);
}

void loop() {
  // Read IR sensor values
  bool leftSensor = digitalRead(IR_LEFT);
  bool rightSensor = digitalRead(IR_RIGHT);

  // Both sensors detect black line → go forward
  if (leftSensor == LOW && rightSensor == LOW) {
    moveForward();
  }
  // Right sensor is off the line → turn left
  else if (leftSensor == LOW && rightSensor == HIGH) {
    turnLeft();
  }
  // Left sensor is off the line → turn right
  else if (leftSensor == HIGH && rightSensor == LOW) {
    turnRight();
  }
  // Both sensors off the line → stop
  else {
    stopMotors();
  }
}

// Move both sides forward
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 150); // Speed for right motor
  analogWrite(ENB, 150); // Speed for left motor
}

// Turn right by stopping left motors
void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 150); // Right side moving
  analogWrite(ENB, 0);   // Left side stopped
}

// Turn left by stopping right motors
void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);   // Right side stopped
  analogWrite(ENB, 150); // Left side moving
}

// Stop all motors
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
