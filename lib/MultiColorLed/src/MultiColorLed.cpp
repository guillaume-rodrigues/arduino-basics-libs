#include "MultiColorLed.h"
#include <Arduino.h>

/**
 * Dummy fade with three loops
 */
void MultiColorLed::dummyFade() {
    int fadeDelay = 1;
    // Iterate over possible red values
    for (int redValue = 0; redValue < 255; redValue += 10) {
        // Iterate over possible green values
        for (int greenValue = 0; greenValue < 255 - redValue; greenValue += 10) {
            // Iterate over possible blue values
            for (int blueValue = 0; blueValue < 255 - redValue - greenValue; blueValue += 10) {
                // Send the new value
                sendColors(redValue, greenValue, blueValue, fadeDelay);
            }
        }
    }
}

/**
 * Fade following the sequence red -> green -> blue -> red
 */
void MultiColorLed::simpleFade() {
    int step = 1;
    int nextColorIndex;
    int values[3] = {250, 0, 0}; // Red, Green, Blue
    // For the three color
    for (int colorIndex = 0; colorIndex < 3; ++colorIndex) {
        // Compute next index to be modulo 3
        nextColorIndex = (colorIndex + 1) % 3;
        // Increment next color and decrease the current color by step
        for (int currColorValue = 0; currColorValue < 250; currColorValue += step) {
            values[colorIndex] -= step;
            values[nextColorIndex] += step;
            sendColors(values[0], values[1], values[2]);
        }
    }
}

/**
 * Use random value every baseFadeDelay for r, g, b led
 */
void MultiColorLed::randomMode() {
    int colorTotals = 0;
    int currValue;
    int currRank;
    int colorValues[3] = {0, 0, 0};
    for (int i = 0; i < 3; ++i) {
        currValue = (int) random(255 - colorTotals);
        currRank = (int) random(0, 3);
        while (colorValues[currRank] != 0) {
            currRank = (int) random(0, 3);
        }
        colorTotals += currValue;
        colorValues[currRank] = currValue;
    }
    sendColors(colorValues[0], colorValues[1], colorValues[2], 100);
}


/**
 * Ask color value to the user via serial port
 */
void MultiColorLed::onDemandMode() {
    Serial.println("What color do you want ?");
    Serial.println("(Accepted values are r,g,b or #rrggbb in exa)");
    while (!Serial.available()) {
        // Wait for available value from serial com
    }
    // Read the answer of the user
    auto serialInput = Serial.readString();
    int colorValues[3] = {0, 0, 0};
    // Reading value for #rrggbb
    if (serialInput.charAt(0) == '#' && serialInput.length() == 7) {
        // Get the red value in exa
        colorValues[0] = (int) strtol(serialInput.substring(1, 3).c_str(), nullptr, 16);
        // Get the green value in exa
        colorValues[1] = (int) strtol(serialInput.substring(3, 5).c_str(), nullptr, 16);
        // Get the blue value in exa
        colorValues[2] = (int) strtol(serialInput.substring(5, 7).c_str(), nullptr, 16);
    } else { // Assume the value is r,v,b
        // Index on color number
        int currentValue = 0;
        // Temporary char array use to handle string conversion
        auto tmpCharArray = (char *) malloc(sizeof(char) * serialInput.length() + 1);
        // Contains the string value of each color
        char *colorValue = nullptr;
        // Copy value of string to char array
        strcpy(tmpCharArray, serialInput.c_str());
        // Get the next value separated by ,
        const char *inputColorPart = strtok_r(tmpCharArray, ",", &colorValue);
        // While there is a next value, or we've got the three required values
        while (inputColorPart != nullptr && currentValue < 3) {
            // Get the numeric value from the string
            colorValues[currentValue] = (int) strtol(inputColorPart, nullptr, 10);
            currentValue++;
            // Get the next value
            inputColorPart = strtok_r(nullptr, ",", &colorValue);
        }
        // Free memory
        free((void *) inputColorPart);
        free((void *) tmpCharArray);
    }
    sendColors(colorValues[0], colorValues[1], colorValues[2], 0);
}

/**
 * Send to defined pins the current values of three colors (red, blue, green)
 * @param redValue The value of red fade
 * @param greenValue The value of green fade
 * @param blueValue The value of blue fade
 * @param fadeDelay The value of fade delay
 */
void MultiColorLed::sendColors(int redValue, int greenValue, int blueValue, int fadeDelay) {
    // Write color values to pins
    analogWrite(redPin, redValue);
    analogWrite(greenPin, greenValue);
    analogWrite(bluePin, blueValue);
    // Wait for next fade
    delay(fadeDelay);
    // Debug
    auto result = (char *) malloc(100 * sizeof(char));
    sprintf(result, "Send red - green - blue : %d - %d - %d", redValue, greenValue, blueValue);
    Serial.println(result);
    free(result);
}

/**
 * Init the component with values. Start Serial connection
 */
void MultiColorLed::init() {
    char readSerialBuffer[1];
    Serial.begin(9600);
    while (!Serial) {
        // Wait for serial connection to begin
    }
    Serial.println("Welcome to multicolor led");
    Serial.println("Please enter a fade mode : ");
    Serial.println("\t1 - dummy mode");
    Serial.println("\t2 - simple mode");
    Serial.println("\t3 - random mode");
    Serial.println("\t4 - on demand mode");
    Serial.println("\tother - stop");
    while (!Serial.available()) {
        // Wait for available value from serial com
    }
    readSerialBuffer[0] = (char) Serial.read();
    fadeMode = (int) strtol(readSerialBuffer, nullptr, 10);
    // Initialise three pin in output mode
    pinMode(MultiColorLed::redPin, OUTPUT);
    pinMode(MultiColorLed::greenPin, OUTPUT);
    pinMode(MultiColorLed::bluePin, OUTPUT);
    MultiColorLed::sendColors(0, 0, 0);
    // Random seed with noise in analog pin 0
    randomSeed(analogRead(0));
}

/**
 * Run the component depending on fade mode
 */
void MultiColorLed::run() const {
    switch (fadeMode) {
        case 1:
            dummyFade();
            break;
        case 2:
            simpleFade();
            break;
        case 3:
            randomMode();
            break;
        case 4:
            onDemandMode();
            break;
        default:
            Serial.println("404 - Mode not found");
            delay(4294967295); // Wait for a long time (~50 days)
            break;
    }
}
