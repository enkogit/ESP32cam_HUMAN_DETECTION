#include "EloquentTinyML.h"
#include "eloquent.h"
#include "eloquent_tinyml/tensorflow/person_detection.h"
// replace 'm5wide' with your own model
// possible values are 'aithinker', 'eye', 'm5stack', 'm5wide', 'wrover'
#include "eloquent/vision/camera/aithinker.h"

const uint16_t imageWidth = 160;
const uint16_t imageHeight = 120;

#define FLASH_LED_PIN 4
#define OUT_PIN 12 // add the output pin for sending a LOW signal
bool flashState = LOW;

Eloquent::TinyML::TensorFlow::PersonDetection<imageWidth, imageHeight> personDetector;

void setup() {
    Serial.begin(115200);
    delay(3000);
      // Set LED Flash as output
    pinMode(FLASH_LED_PIN, OUTPUT);
    digitalWrite(FLASH_LED_PIN, flashState);
    // Set output pin as output
    pinMode(OUT_PIN, OUTPUT);
    digitalWrite(OUT_PIN, HIGH); // set HIGH initially

    // configure camera
    camera.grayscale();
    camera.qqvga();

    while (!camera.begin())
        Serial.println("Cannot init camera");

    // configure a threshold for "robust" person detection
    // if no threshold is set, "person" would be detected everytime
    // person_score > not_person_score, even if just by 1
    // by trial and error, considering that scores range from 0 to 255,
    // a threshold of 190-200 dramatically reduces the number of false positives
    personDetector.setDetectionAbsoluteThreshold(190);
    personDetector.begin();

    // abort if an error occurred on the detector
    while (!personDetector.isOk()) {
        Serial.print("Detector init error: ");
        Serial.println(personDetector.getErrorMessage());
    }
}

void loop() {
    if (!camera.capture()) {
        Serial.println("Camera capture error");
        delay(1000);
        return;
    }

    bool isPersonInFrame = personDetector.detectPerson(camera.buffer);

    if (!personDetector.isOk()) {
        Serial.print("Person detector detection error: ");
        Serial.println(personDetector.getErrorMessage());
        delay(1000);
        return;
    }

    Serial.println(isPersonInFrame ? "Person detected" : "No person detected");
    if (isPersonInFrame) {
            digitalWrite(FLASH_LED_PIN, HIGH);
            digitalWrite(OUT_PIN, LOW); // set the output pin to LOW
    
        } else {
            digitalWrite(FLASH_LED_PIN, LOW);
            digitalWrite(OUT_PIN, HIGH); // set the output pin to HIGH
        }
    Serial.print("\t > It took ");
    Serial.print(personDetector.getElapsedTime());
    Serial.println("ms to detect");
    Serial.print("\t > Person score: ");
    Serial.println(personDetector.getPersonScore());
    Serial.print("\t > Not person score: ");
    Serial.println(personDetector.getNotPersonScore());
}

