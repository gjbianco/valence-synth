/*
    valence synth
    
        by Guy Bianco

    originally based off of light_theremin.ino and other examples
*/

int sensorHigh = 0;
int sensorLow  = 1023;

const int ledPin    = 13;
const int buttonPin = 2;

const int GRAN     = 100;       // number of grains per second
const int DELAY    = 1000/GRAN; // delay to achieve above Gps
const int LOOP_LEN = 3;         // loop length

int counter  = 0;            // global loop counter
int looper[GRAN*LOOP_LEN];   // global sound loop

int buttonState = 0;

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT);

    // set serial (USB) communication
    Serial.begin(9600);

    calibrate(); // calibrate the light sensor
}

void loop() {
    // read inputs
    int sensorValue = analogRead(A0); 
    buttonState = digitalRead(buttonPin); 

    // map sensor values to range of pitches
    // adjust values to conform to max and min
    // received from sensor
    int pitch = map(sensorValue,
                    sensorLow,
                    sensorHigh,
                    50,
                    4000);

    // if button/key is pressed, modify current part of loop
    if(buttonState)
        looper[counter] = pitch;

    // play the tone
    tone(8, looper[counter], 20);

    // increment global counter
    counter++;
    // "fit" counter back into looper's size
    counter %= GRAN*LOOP_LEN;

    // send the pitch over serial (USB)
    Serial.println(pitch);

    // wait calculated amount of time
    delay(DELAY);
}

int pitchMod(int pitch) {
    int newPitch = pitch;
    newPitch += random(50, 500);
    newPitch %= 4000;
    return newPitch;
}

void calibrate() {
    // turn on LED to signal the start of calibration
    digitalWrite(ledPin, HIGH);

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