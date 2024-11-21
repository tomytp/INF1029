#include <TimerOne.h>
#include <MultiFuncShield.h>

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

GeneratorModeValues currentState;
int MIN_NUM = 0, MAX_NUM = 10;
RangeStatusValues MIN_SET = RANGE_NOT_SET, MAX_SET = RANGE_NOT_SET;

inline void executeStopped() {
  byte btn = MFS.getButton();
  if (!btn) return;

  switch (btn) {
    case BUTTON_1_LONG_RELEASE:
      if (MIN_SET == RANGE_SET && MAX_SET == RANGE_SET) {
        currentState = GENERATOR_STARTED;
        return;
      }
      break;

    case BUTTON_2_SHORT_RELEASE:
      MFS.write(MAX_NUM);
      break;

    case BUTTON_2_LONG_RELEASE:
      currentState = SETTING_RANGE_MAX_NUM_STARTED;
      break;

    case BUTTON_3_SHORT_RELEASE:
      MFS.write(MIN_NUM);
      break;

    case BUTTON_3_LONG_RELEASE:
      currentState = SETTING_RANGE_MIN_NUM_STARTED;
      break;
  }
}

inline void executeStarted() {

}

inline void executeMaxNum() {
  byte btn = MFS.getButton();
  if (!btn) return;

  switch (btn) {
    case BUTTON_1_SHORT_RELEASE:
  }
}

inline void executeMinNum() {

}

void setup()
{
  Serial.begin(9600);
  currentState = GENERATOR_STOPPED;
  Timer1.initialize();
  MFS.initialize(&Timer1);
}

void loop() {
  switch (currentState)
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