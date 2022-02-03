#ifndef BASE_PROJECT_MULTI_COLOR_LED_H
#define BASE_PROJECT_MULTI_COLOR_LED_H

class MultiColorLed {
public:
    int fadeMode{}; // Fade mode : 1 dummy, 2 simple, 3 random
    MultiColorLed() = default;

    void init();

    void run() const;

protected:
    static constexpr int bluePin = 3; // Must be digital output PWN
    static constexpr int greenPin = 5; // Must be digital output PWN
    static constexpr int redPin = 6; // Must be digital output PWN
    static constexpr int baseFadeDelay = 50; // waiting time between color fades in ms
private:
    static void sendColors(int redValue, int greenValue, int blueValue, int fadeDelay = baseFadeDelay);

    static void dummyFade();

    static void simpleFade();

    static void randomMode();

    static void onDemandMode();
};

#endif
