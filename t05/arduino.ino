#define RANGE_MAX_NUMBER 9999
#define RANGE_MIN_NUMBER 0000

enum GeneratorModeValues {
    GENERATOR_STOPPED,
    GENERATOR_STARTED,
    SETTING_RANGE_MAX_NUM_STARTED,
    SETTING_RANGE_MIN_NUM_STARTED
};

enum RangeStatusValues {
    RANGE_NOT_SET,
    RANGE_SET
};

enum ledModeValues {
    LED_ALL_OFF,
    LED_1_ON,
    LED_2_ON,
    LED_3_ON,
    LED_4_ON
};

enum AnimationModeValues {
    ANIMATION_STOPPED,
    ANIMATION_STARTED,
    ANIMATION_STAGE1,
    ANIMATION_STAGE2,
    ANIMATION_STAGE3,
    ANIMATION_STAGE4,
    ANIMATION_INTERRUPTED
};

GeneratorModeValues currentMode;

inline void executeStopped() {

}

inline void executeStarted() {

}

inline void executeMaxNum() {

}

inline void executeMinNum() {

}

void setup() {
    currentMode = GENERATOR_STOPPED;
}

void loop() {
    switch (currentMode)
    {
    case GENERATOR_STOPPED:
        executeStopped();
        break;
    case GENERATOR_STARTED:
        executeStarted();
        break;
    case SETTING_RANGE_MAX_NUM_STARTED:
        executeMaxNum();
        break;
    case SETTING_RANGE_MIN_NUM_STARTED:
        executeMinNum();
        break;
    }
}