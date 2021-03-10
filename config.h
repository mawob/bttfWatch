// => Hardware select
// #define LILYGO_WATCH_2019_WITH_TOUCH     //To use T-Watch2019 with touchscreen, please uncomment this line
// #define LILYGO_WATCH_2019_NO_TOUCH       //To use T-Watch2019 Not touchscreen , please uncomment this line
#define LILYGO_WATCH_2020_V1             //To use T-Watch2020 V1, please uncomment this line
// #define LILYGO_WATCH_2020_V2             //To use T-Watch2020 V2, please uncomment this line
// #define LILYGO_WATCH_2020_V3             //To use T-Watch2020 V3, please uncomment this line

// => Function select
#define LILYGO_WATCH_LVGL                   //To use LVGL, you need to enable the macro LVGL

#include <LilyGoWatch.h>


// ------------------------------------------------------------------------------
// --- global variables and parameters for the BTTF watch 
// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
// --- global variables that can be changed in the watch settings
// -   settings page #1 - watchface related
uint8_t modeWatchfaceChanges   = 1;    // watchface 0:fixed (can be swiped)   1:changes randomly  2:cycles through all in a row
uint8_t modeDisplay1224sec     = 0;   // display time 0: 12h  1: 12h+sec  2: 24h    <<< yes, easier to understand in settings
uint8_t modeDisplayStepCounter = 1;   // display step counter  0:off  1:on
uint8_t modeDisplayBattery     = 1;   // display battery percentage  0:off  1:on
// -   settings page #2 - display related
uint8_t modeDisplayStayOn      = 0;    // select the time the display stays on: 0,1,2 (default:0)
uint8_t modeDisplayNormalBrightness = 0;   // select the brightness during normal operation: 0,1,2 (default:0)
uint8_t modeDisplayDimAtNight  = 1;   // dim display between 10pm and 7am    0:off  1:on

// --- constants
//     these are the actual values thatcan be selected in the BTTF watch's settings menu
uint8_t displayBrightness[3] = {54, 76, 100};   // the three brightness values that are selected with: modeDisplayNormalBrightness
uint8_t displayStayOnTime[3] = {7, 12, 20};   // the three stay-on times that are selected with: modeDisplayStayOn

// --- the CPU frequencies for normal operation and for light sleep = "off") mode
//uint8_t cpuFrequencyNormal = 80;     // CPU frequency for normal operation
uint8_t cpuFrequencyNormal = 30;     // CPU frequency for normal operation  <<< the lowest value for which the graphics was still responsive
//uint8_t cpuFrequencyLow    = 10;     // CPU frequency in light sleep mode
uint8_t cpuFrequencyLow    = 3;     // CPU frequency in light sleep mode


// --- global variables for internal use
uint32_t lastActivityTime      = 0;   // time (in ms) when last user activity was noticed
uint32_t lightSleepTime        = 0;   // time (in ms) when system went to light sleep

#define MAX_WATCH_SCREENS  5      // max No of watchface screens
#define MAX_SET_SCREENS    5      // max No of settings screens
uint8_t activeScreen    = 1;      // No of active screen (initialize withj "1" => flux cap)
uint8_t lastActiveWatch = 1;      // store the No of last active watchface
