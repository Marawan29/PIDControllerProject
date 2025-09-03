// Fan tachometer pin
const byte tachPin = D1;
volatile unsigned int pulseCount = 0;
unsigned long lastRPMTime = 0;
unsigned int rpm = 0;
const byte OUTPUT_PIN = D5;
double dt, last_time;
double integral, previous, output = 0;
double kp, ki, kd;
int setpoint = 1900;
int max_rpm = 3600;

double pid(double error)
{
  double proportional = error;
  integral += error * dt;
  double derivative = (error - previous) / dt;
  previous = error;
  double output = (kp * proportional) + (ki * integral) + (kd * derivative);
  output = constrain(output, 0, max_rpm);
  int voltage_control = map(output, 0, max_rpm, 0, 255);
  return output;
}

void IRAM_ATTR countPulse() {
  pulseCount++;
}

void setup()
{
  Serial.begin(115200);
  kp = 2;
  ki = 9.5;
  kd = 0.08;
  last_time = 0;
  analogWrite(OUTPUT_PIN, 0);
  pinMode(tachPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(tachPin), countPulse, FALLING);
  for(int i = 0; i < 50; i++)
  {
    Serial.print(setpoint);
    Serial.print(",");
    Serial.println(0);
    delay(100);
  }
  delay(100);
}

void loop()
{
  double now = millis();
  dt = (now - last_time)/1000.00;
  last_time = now;

  if (now - lastRPMTime >= 100) { // every 0.1 second
    // 2 pulses per revolution
    rpm = (pulseCount / 2) * 60 * 10;
    pulseCount = 0;
    lastRPMTime = now;
    double actual = rpm;
    double error = setpoint - actual;
    output = pid(error);
    analogWrite(OUTPUT_PIN, output);

    // Setpoint VS Actual
    Serial.print(setpoint);
    Serial.print(",");
    Serial.println(actual);
    
  }
}
