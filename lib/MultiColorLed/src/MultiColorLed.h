#ifndef BASE_PROJECT_MULTI_COLOR_LED_H
#define BASE_PROJECT_MULTI_COLOR_LED_H

class MultiColorLed {
public:
    int fadeMode{}; // Fade mode : 1 dummy, 2 simple, 3 random, 4 on demand
    bool debugMode{}; // Mode used to print more data over serial com
    bool firstLoop = true; // Indicate if we are in the first loop

    explicit MultiColorLed(bool debugMode = false) : debugMode(debugMode) {};

    void init();

    void run();

protected:
    static constexpr int bluePin = 3; // Must be digital output PWN
    static constexpr int greenPin = 5; // Must be digital output PWN
    static constexpr int redPin = 6; // Must be digital output PWN
    static constexpr int baseFadeDelay = 50; // waiting time between color fades in ms
private:
    void sendColors(int redValue, int greenValue, int blueValue, int fadeDelay = baseFadeDelay) const;

    void dummyFade() const;

    void simpleFade() const;

    void randomMode() const;

    void onDemandMode();

    void algorithmMode();

    char * askValuesForUser(char const * message, char const * hint);

    static void clearReadBuffer();
};

#endif
