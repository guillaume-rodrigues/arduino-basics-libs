#include <Arduino.h>
#include "MultiColorLed.h"
#include "../../Core/src/Core.h"

/**
 * Dummy fade with three loops
 */
void MultiColorLed::dummyFade() const {
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
void MultiColorLed::simpleFade() const {
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
void MultiColorLed::randomMode() const {
    auto colorValues = Core::RandomColor();
    sendColors(colorValues[0], colorValues[1], colorValues[2], 100);
    free((void *) colorValues);
}

/**
 * Ask color value to the user via serial port
 */
void MultiColorLed::onDemandMode() {
    char *serialInput = MultiColorLed::askValuesForUser("What color do you want ?",
                                                        "(Accepted values are r,g,b or #rrggbb in exa)");
    int colorValues[3] = {0, 0, 0};
    Core::ParseColorFromString(serialInput, colorValues);
    sendColors(colorValues[0], colorValues[1], colorValues[2], 0);
    free((void *) serialInput);
}

/**
 * Ask to the user the algorithm of color to be used in led
 */
void MultiColorLed::algorithmMode() {
    // Fade delay in milliseconds
    int fadeDelay = 1000;
    // Ask the next color algorithm to user
    char *serialInput = MultiColorLed::askValuesForUser("What algorithm do you want ?",
                                                        "(Accepted values are string of r g b separated by comma)");
    // Nice print asked colors
    Serial.print("Asked : \t\t");
    Serial.println(serialInput);
    // Parse asked color to color sequence
    auto colorSequence = Core::ParseAlgorithmFromString(serialInput);
    // Print current color
    Serial.print("Current : \t");
    for (unsigned int sequenceIndex = 0; sequenceIndex < strlen(colorSequence); ++sequenceIndex) {
        switch (colorSequence[sequenceIndex]) {
            case 'r':
                sendColors(255, 0, 0, 0);
                Serial.print("r ");
                break;
            case 'g':
                sendColors(0, 255, 0, 0);
                Serial.print("g ");
                break;
            case 'b':
                sendColors(0, 0, 255, 0);
                Serial.print("b ");
                break;
            default:
                break;
        }
        delay(fadeDelay);
    }
    Serial.println("\nOther ?");
    free((void *) serialInput);
    free((void *) colorSequence);
}

/**
 * Ask to the user a value from Serial com
 * @param message The message to be print to the user in Serial com
 * @param hint The hint for values to be print to the user in Serial com
 * @return The provided message from Serial com
 */
char *MultiColorLed::askValuesForUser(char const *message, char const *hint) {
    MultiColorLed::clearReadBuffer();
    if (firstLoop) {
        Serial.println(message);
        Serial.println(hint);
        firstLoop = false;
    }
    while (!Serial.available()) {
        // Wait for available value from serial com
    }
    delay(100); // Wait for data to be sent
    int availableByte = Serial.available();
    // Read the answer of the user
    auto serialInput = (char *) malloc(sizeof(char) * availableByte + 1);
    // Clear content
    sprintf(serialInput, "%s", "");
    // For each available bytes
    for (int inputIndex = 0; inputIndex < availableByte; ++inputIndex) {
        serialInput[inputIndex] = (char) Serial.read(); // Read the first byte
    }
    serialInput[availableByte] = '\0';
    return serialInput;
}

/**
 * Send to defined pins the current values of three colors (red, blue, green)
 * @param redValue The value of red fade
 * @param greenValue The value of green fade
 * @param blueValue The value of blue fade
 * @param fadeDelay The value of fade delay
 */
void MultiColorLed::sendColors(int redValue, int greenValue, int blueValue, int fadeDelay) const {
    // Write color values to pins
    analogWrite(redPin, redValue);
    analogWrite(greenPin, greenValue);
    analogWrite(bluePin, blueValue);
    // Wait for next fade
    delay(fadeDelay);
    if (debugMode) {
        auto result = (char *) malloc(100 * sizeof(char));
        sprintf(result, "Send red - green - blue : %d - %d - %d", redValue, greenValue, blueValue);
        Serial.println(result);
        free(result);
    }
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
    Serial.println("\t5 - algorithm mode");
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
    srand(analogRead(0));
}

/**
 * Run the component depending on fade mode
 */
void MultiColorLed::run() {
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
        case 5:
            algorithmMode();
            break;
        default:
            Serial.println("404 - Mode not found");
            delay(4294967295); // Wait for a long time (~50 days)
            break;
    }
}

/**
 * Clear serial read buffer, in order to remove unexpected char
 */
void MultiColorLed::clearReadBuffer() {
    while (Serial.available() > 0) {
        Serial.read();
    }
}
