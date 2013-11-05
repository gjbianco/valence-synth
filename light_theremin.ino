/*
    valence synth
    
        by Guy Bianco

    originally based off of light_theremin.ino and other examples
*/

int sensorHigh = 0;
int sensorLow  = 1023;

// LED pin
const int ledPin = 13;

void setup() {
    // turn on LED to signal the start of calibration
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);

    // set serial (USB) communication
    Serial.begin(9600);

    // calibrate during first five seconds
    while(millis() < 5000){
      int sensorValue = analogRead(A0);

      // record max sensor value
      if(sensorValue > sensorHigh){
          sensorHigh = sensorValue;
      }  

      // record min sensor value
      if(sensorValue < sensorLow){
          sensorLow = sensorValue;
      }  
    }

    // signal end of calibration
    digitalWrite(ledPin, LOW); 
}

void loop() {
    // read input from A0
    int sensorValue = analogRead(A0); 

    // map sensor values to range of pitches
    // adjust values to conform to max and min
    // received from sensor
    int pitch = map(sensorValue,
                    sensorLow,
                    sensorHigh,
                    50,
                    4000);

    // modify the pitch in some manner
    pitch = pitchMod(pitch);

    // play the tone
    tone(8, pitch, 20);

    // send the pitch over serial (USB)
    Serial.println(pitch);

    // wait
    delay(10);
}

int pitchMod(int pitch) {
    int newPitch = pitch;
    newPitch += random(50, 500);
    newPitch %= 4000;
    return newPitch;
}