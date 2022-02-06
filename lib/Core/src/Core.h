#ifndef ARDUINO_BASICS_PROJECT_CORE_LIB_H
#define ARDUINO_BASICS_PROJECT_CORE_LIB_H

class Core {
public:
    static void ParseColorFromString(char * input, int colorValues[3]);
    static char * ParseAlgorithmFromString(char const * input);
    static int * RandomColor();
};


#endif //ARDUINO_BASICS_PROJECT_CORE_LIB_H
