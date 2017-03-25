/*
	# Warmth : light device for warmth
 - https://github.com/PROTOROOM/Warmth
 	- by SeungBum Kim & Hoonida Kim
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
int oldTemp = 0;

boolean triggered = false;
boolean isStarting = true;

long pTime = 0;
long cTime = 0;
int fade = 20;
int brightness = 0;
int pulseCount = 0;
int minValue = 0;
int defaultValue = 130;
int threshold = 5;

void setup() {                
  pinMode(analogOutput, OUTPUT);

  averageTemp(1);
  brightness = defaultValue;
}


// ##########################  MAIN  ##########################
void loop() {

  current = readSmooth();
  cTime = millis();


  if (triggered) {
    
    if (isStarting) {
      for (;brightness<256;brightness+=1) {
        writeValue(brightness);
        delay(10);
      }
      delay(1000);      
      isStarting = false;
    }

    for (int i=0; i<5; i++) {
      beatPulse2(40);
    }
    
    if (!isIncreasing()) {
      for (;brightness < defaultValue; brightness+=2) {
        writeValue(brightness);
        delay(20);
      }    
      triggered = false;
      isStarting = true;
    }

  } else {
    
    writeValue(brightness);
    
    if ((current - average) > threshold && isIncreasing()) {
      triggered = true;
      pulseCount = 0;
    }

  }

}


// ##########################  Commands  ##########################
boolean isIncreasing() {
  boolean result = false;
  if (cTime - pTime > 200) {
    pTime = cTime;
    if (current > oldTemp) result = true;
    oldTemp = current;
  }
  
  return result;
}


void beatPulse(int beat) {  
  if (brightness <= 0) {
    fade = 20;
  }
  if (brightness >= 255) {
    fade = -5;
    pulseCount++;
  }

  if (cTime - pTime > int(1000/beat)) {
    pTime = cTime;
    writeValue(brightness);
    brightness = brightness + fade;
  }
}


void beatPulse2(int beat) {
//  brightness = 0;
  for (;brightness<=255;brightness+=10) {
    writeValue(min(255, brightness));
    delay(1000/beat);
  }
  for (;brightness>minValue; brightness-=5) {
    writeValue(brightness);
    delay(1000/beat);
  }
}


void averageTemp(boolean withLight) {
  int t = 0;
  int n = 30;
  for (int i=0; i<n; i++) {
    t = t + analogRead(analogInput);

    if (withLight) {
      if (n%2 == 0) writeValue(100);
      else writeValue(0);
    }

    delay(100);
  }
  average = t / n;
}


int readSmooth() {
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(analogInput);
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  
  if (readIndex >= numReadings) readIndex = 0;
  return total / numReadings;
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
  } 
  else if (light < mid2) {
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



