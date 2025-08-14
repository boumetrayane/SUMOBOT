const int trigpin = 11;
const int echopin = 10;
const int motorAp1 = 8;
const int motorAp2 = 7;
const int motorA = 9;
const int motorBp1 = 4;
const int motorBp2 = 6;
const int motorB = 5;
const int ir_front = 2;
long duration;
int distance;
int front_ir;
int steady = 1;

void setup() {
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  pinMode(motorAp1, OUTPUT);
  pinMode(motorAp2, OUTPUT);
  pinMode(motorA, OUTPUT);
  pinMode(motorBp1, OUTPUT);
  pinMode(motorBp2, OUTPUT);
  pinMode(motorB, OUTPUT);
  pinMode(ir_front, INPUT);
  Serial.begin(9600);
}

void motors(int spa = 0, int spb = 0, int a1 = LOW, int a2 = LOW, int b1 = LOW, int b2 = LOW) {
  analogWrite(motorA, spa);
  analogWrite(motorB, spb);
  digitalWrite(motorAp1, a1);
  digitalWrite(motorAp2, a2);
  digitalWrite(motorBp1, b1);
  digitalWrite(motorBp2, b2);
}

void loop() {
  if (steady){
    steady = 0;
    delay(5000);
  }
  if (checkIR()) {
    int direction = random(3);
    if (direction == 0){
      motors(255, 255, LOW, HIGH, LOW, HIGH); // goes back
    } else if (direction == 1) {
      motors(200, 0, LOW, HIGH, LOW, LOW); // backward right
    } else {
      motors(0, 255, LOW, LOW, LOW, HIGH); // backward left
    }
    delay(700);
    if (random(2) == 0) {
      motors(255, 0, HIGH, LOW, LOW, LOW); // forward right
    } else {
      motors(0, 200, LOW, LOW, HIGH, LOW); // forward left
    }
    delay(300);
    return; // Immediately return to the top of the main loop
  }
  
  if (visible_enemy(30)) {
    
      stop(0);
      motors(255, 255, HIGH, LOW, HIGH, LOW); // forward
    
    
  } else {
    motors(); // stop the motors
    search("tornado");
  }
}

bool visible_enemy(int desired) {
  long totalDistance = 0;
  int validReadings = 0;

  // Take multiple readings for stability
  for (int i = 0; i < 1; i++) {
    // Trigger the ultrasonic sensor
    digitalWrite(trigpin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigpin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigpin, LOW);

    duration = pulseIn(echopin, HIGH);
    distance = duration * 0.034 / 2;

    // Ignore zero or out-of-range values
    if (distance > 0 && distance < 100) { // Adjust upper limit as needed
      totalDistance += distance;
      validReadings++;
    }

    delay(10); // Small delay between readings for stability
  }

  // Calculate the average distance if we got valid readings
  if (validReadings > 0) {
    distance = totalDistance / validReadings;
    Serial.println(distance);
  } else {
    Serial.println("No valid reading");
    return false; // No valid distance measured
  }

  // Return true if the averaged distance is within the desired range
  return distance <= desired;
}

void search(String strategy) {
  if (strategy == "tornado") {
    motors(200, 0, HIGH, LOW, LOW, LOW); // forward right
  } else if (strategy == "charge") {
    motors(200, 200, HIGH, LOW, HIGH, LOW); // forward
  } else if (strategy == "snake") {
    motors(200, 200, HIGH, LOW, HIGH, LOW); // forward
    delay(250);
    motors(200, 0, HIGH, LOW, LOW, LOW); // forward right
    delay(250);
    motors(0, 200, LOW, LOW, HIGH, LOW); // forward left
    delay(250);
  }
}

bool checkIR() {
  front_ir = digitalRead(ir_front);
  if (front_ir == 1) {
    return true; // Indicates that IR check has run and processed an event
  }
  return false; // No event processed
}

void stop(long stop_time) {
  motors(0, 0, LOW, LOW, LOW, LOW);
  delay(stop_time);
}
