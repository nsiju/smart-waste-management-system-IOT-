#include <Servo.h>
#include <SoftwareSerial.h>

// Pin Definitions
const int trigPin = 8;
const int echoPin = 9;
const int irSensorPin = 7;       // Infrared sensor pin
const int buttonPin = 6;         // Push button pin for manual lid control
const int servoPin = 5;          // Servo motor pin
const int ledPin = 13;           // LED pin for setup confirmation
Servo myServo;                   // Servo object

SoftwareSerial sim808(10, 11);   // RX, TX for SIM808 module

// Threshold distances for bin fill level (in cm)
const int binFullDistance = 10;  // Distance when the bin is considered full

// Variables
bool binIsFull = false;
bool lidOpen = false;
bool buttonPressed = false;      // To track button press
bool lastButtonState = HIGH;     // To track last button state for debounce

// Function to read button state
bool isButtonPressed() {
  return digitalRead(buttonPin) == LOW;
}

// Function to open the bin lid
void openLid() {
  myServo.write(90);  // Move servo to 90 degrees (lid open)
  lidOpen = true;
}

// Function to close the bin lid
void closeLid() {
  myServo.write(0);   // Move servo to 0 degrees (lid closed)
  lidOpen = false;
}

void setup() {
  // Initialize components
  Serial.begin(9600);
  sim808.begin(9600);
  
  pinMode(buttonPin, INPUT_PULLUP);    // Button input with internal pull-up resistor
  pinMode(ledPin, OUTPUT);             // LED pin as output

  myServo.attach(servoPin);            // Attach servo motor to pin

  // Keep LED continuously on to indicate system is functioning
  digitalWrite(ledPin, HIGH);

  // Initialize servo in closed position
  closeLid();
}

void loop() {
  // Read the current state of the button
  bool currentButtonState = isButtonPressed();

  // Check for button press (LOW state) and change from previous state
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    buttonPressed = !buttonPressed;  // Toggle buttonPressed state

    // Toggle the lid based on buttonPressed state
    if (buttonPressed) {
      openLid();  // Open the lid
    } else {
      closeLid(); // Close the lid
    }

    delay(500);  // Debounce delay
  }

  // Save the current state as the last state for the next loop
  lastButtonState = currentButtonState;

  delay(50); // Small delay to avoid bouncing issues
}
