/*
	# Warmth : the wedding device
	- by SeungBum Kim <picxenk@gmail.com>
	- Artistic License 2.0  http://choosealicense.com/licenses/artistic-2.0/
	
*/

int analogInput = A1;
int analogOutput = 1;

const int numReadings = 50;
int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;
int current = 0;
boolean triggered = false;

long pTime = 0;
long cTime = 0;
int fade = 20;
int brightness = 0;

void setup() {                
  pinMode(analogOutput, OUTPUT);

  total = 0;
  for (int i=0; i<numReadings; i++) {
    total = total + analogRead(analogInput);
    if (numReadings%2 == 0) writeValue(100);
    else writeValue(0);
    delay(100);
  }
  average = total / numReadings;
}


// ##########################  MAIN  ##########################
void loop() {
	
//  nTemp = analogRead(analogInput);
//  minTemp = min(minTemp, nTemp);
//  maxTemp = max(maxTemp, nTemp);

  current = analogRead(analogInput);
  cTime = millis();
  
  if (!triggered && abs(average - current) > 10) {
    triggered = true;
  }
  
  if (triggered) {
//    beatPulse(70);
    for (int i=0; i<5; i++) {
      writeValue(250);
      delay(300);
      writeValue(50);
      delay(300);
    }
    triggered = false;
  } else {
    writeValue(130);
  }
//  slowOn(0.01);
}


void beatPulse(int beat) {  
  if (brightness == 0) {
    fade = 20;
  }
  if (brightness == 240) {
    fade = -5;
  }

  if (cTime - pTime > int(1000/beat)) {
    pTime = cTime;
    writeValue(brightness);
    brightness = brightness + fade;
  }
}




// Analog Input
int readValue() {
	return map(analogRead(analogInput), 0, 1023, 0, 255);
}


float readTemp() {
  int reading = analogRead(analogInput);
  float voltage = reading * 3.0/1024.0;
//  float cTemp = (voltage - 0.5) * 100;
//  return cTemp;
  return voltage;
//  return map(cTemp, 20, 32, 0, 255);
}

int calibrateLightSensor(int light) {
    int mid = 600;
    int mid2 = 900;
    if ( light < mid) {
        light = int(round((40.0/mid)*light));
    } else if (light < mid2) {
        light = int(round((mid2-40)/(mid2-float(mid))* light) - 1680);
    }
    light = constrain(light, 0, 1023);    
    return light;
}


// Analog Output
void writeValue(int value) {
	analogWrite(analogOutput, value);
}

void slowOn(float waitSec) {
	for (int i=0; i<=255; i=i+5) {
		writeValue(i);
		wait(waitSec);
	}
}

void slowOff(float waitSec) {
	for (int i=255; i>=0; i=i-5) {
		writeValue(i);
		wait(waitSec);
	}	
}



// Common
void wait(float sec) {
	delay(sec*1000);
}


