// Fan tachometer pin
const byte tachPin = D1;
volatile unsigned int pulseCount = 0;
unsigned long lastRPMTime = 0;
unsigned int rpm = 0;
const byte OUTPUT_PIN = D5;
double last_time;
double previous, output = 0;
int setpoint = 1900;
int max_rpm = 3600;
int current_step = 0;
int step_period = 0;
int step_amm[] = {3000, 2000, 2500, 3000, 2000, 1800, 3200, 1800, 3000, 2000};

void IRAM_ATTR countPulse() {
  pulseCount++;
}

void setup()
{
  Serial.begin(115200);
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
    if(step_period % 50 == 0) {
       step_period += 1;
      setpoint = step_amm[current_step];
      output = map(setpoint, 0, 3000, 0, 170);
      current_step += 1;
    } else {
            step_period += 1;
            setpoint = step_amm[current_step];
            output = map(setpoint, 0, 3000, 0, 170);
    }
    if(current_step == 10) {
      current_step = 0;
    }

    analogWrite(OUTPUT_PIN, output);

    // Setpoint VS Actual
    Serial.print(setpoint);
    Serial.print(",");
    Serial.println(actual);
    
  }
}
