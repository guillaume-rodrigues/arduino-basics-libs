#include "Core.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void Core::ParseColorFromString(char *input, int colorValues[3]) {
    // Reading value for #rrggbb
    if (input[0] == '#' && strlen(input) == 7) {
        char tmpString[3] = {input[1], input[2], '\0'};
        // Get the red value in exa
        colorValues[0] = (int) strtol(tmpString, nullptr, 16);
        tmpString[0] = input[3];
        tmpString[1] = input[4];
        // Get the green value in exa
        colorValues[1] = (int) strtol(tmpString, nullptr, 16);
        tmpString[0] = input[5];
        tmpString[1] = input[6];
        // Get the blue value in exa
        colorValues[2] = (int) strtol(tmpString, nullptr, 16);
    } else { // Assume the value is r,v,b
        // Index on color number
        int currentValue = 0;
        // Contains the string value of each color
        char *colorValue = nullptr;
        // Get the next value separated by ,
        const char *inputColorPart = strtok_r(input, ",", &colorValue);
        // While there is a next value, or we've got the three required values
        while (inputColorPart != nullptr && currentValue < 3) {
            // Get the numeric value from the string
            colorValues[currentValue] = (int) strtol(inputColorPart, nullptr, 10) % 256;
            currentValue++;
            // Get the next value
            inputColorPart = strtok_r(nullptr, ",", &colorValue);
        }
        // Free memory
        free((void *) inputColorPart);
    }
}

char *Core::ParseAlgorithmFromString(char const *input) {
    unsigned int resultSize = strlen(input) / 2 + 1;
    auto result = (char *) malloc(sizeof(char) * resultSize);
    sprintf(result, "%s", "");
    int currentIndex = 0;
    for (unsigned int inputIndex = 0; inputIndex < strlen(input); ++inputIndex) {
        if (input[inputIndex] != ',' && currentIndex < resultSize) {
            result[currentIndex] = input[inputIndex];
            currentIndex++;
        }
    }
    result[currentIndex] = '\0';
    return result;
}

int *Core::RandomColor() {
    auto colorValues = (int *) malloc(sizeof(int) * 3);
    colorValues[0] = 0;
    colorValues[1] = 0;
    colorValues[2] = 0;
    int currValue;
    int currRank;
    for (int i = 0; i < 3; ++i) {
        currValue = rand() % 256;
        currRank = rand() % 3;
        while (colorValues[currRank] != 0) {
            currRank = rand() % 3;
        }
        colorValues[currRank] = currValue;
    }
    return colorValues;
}