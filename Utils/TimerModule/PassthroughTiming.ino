// constants won't change. They're used here to 
// set pin numbers:
const int projectorPin = 12;     // the number of the pushbutton pin
const int cameraPin =  7;      // the number of the LED pin

void setup() {
  // initialize the LED pin as an output:
  pinMode(cameraPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(projectorPin, INPUT);     
}

void loop()
{
  digitalWrite(cameraPin, digitalRead(projectorPin));
}
