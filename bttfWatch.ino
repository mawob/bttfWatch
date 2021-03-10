/*
 * Copyright (c) 2021 Markus Wobisch
 * 
 *  https://github.com/mawob/
 *  
 * bttfWatch was written based on the concepts found in other codes, including
 *  - the SimpleWatch example by Lewis he: 
 *        https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library
 *  - aGoodWatch by Alex Goodtear: 
 *        https://github.com/AlexGoodyear/agoodWatch
 *  - the Arduino framework by Dan Geiger: 
 *        https://www.instructables.com/Lilygo-T-Watch-2020-Arduino-Framework/
 *  - the examples from diyproject.io:      
 *        https://diyprojects.io/lilygowatch-esp32-ttgo-t-watch-get-started-ide-arduino-platformio
 *        
 *  The code was made for the T-Watch 2000 v1  
 *  
 */
 
#include "config.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/queue.h"
#include <soc/rtc.h>
//#include "esp_wifi.h"
#include "esp_sleep.h"
//#include <WiFi.h>

#define G_EVENT_VBUS_PLUGIN         _BV(0)
#define G_EVENT_VBUS_REMOVE         _BV(1)
#define G_EVENT_CHARGE_DONE         _BV(2)

enum {
    Q_EVENT_BMA_INT,
    Q_EVENT_AXP_INT,
} ;


#define WATCH_FLAG_SLEEP_MODE   _BV(1)
#define WATCH_FLAG_SLEEP_EXIT   _BV(2)
#define WATCH_FLAG_BMA_IRQ      _BV(3)
#define WATCH_FLAG_AXP_IRQ      _BV(4)

QueueHandle_t g_event_queue_handle = NULL;
EventGroupHandle_t g_event_group = NULL;
EventGroupHandle_t isr_group = NULL;
bool lenergy = false;
TTGOClass *ttgo;


void low_energy()
{
    if (ttgo->bl->isOn()) {
        lightSleepTime = millis();            // store time when system goes to sleep 
        xEventGroupSetBits(isr_group, WATCH_FLAG_SLEEP_MODE);
        ttgo->closeBL();
        ttgo->stopLvglTick();
        ttgo->bma->enableStepCountInterrupt(false);
        ttgo->displaySleep();
        lenergy = true;
        setCpuFrequencyMhz(cpuFrequencyLow); 
        gpio_wakeup_enable ((gpio_num_t)AXP202_INT, GPIO_INTR_LOW_LEVEL);
        gpio_wakeup_enable ((gpio_num_t)BMA423_INT1, GPIO_INTR_HIGH_LEVEL);
        esp_sleep_enable_gpio_wakeup ();
        esp_light_sleep_start();

    } else {
        ttgo->startLvglTick();
        ttgo->displayWakeup();
        ttgo->rtc->syncToSystem();
        
        lv_disp_trig_activity(NULL);
        ttgo->openBL();
        ttgo->bma->enableStepCountInterrupt(true);

        // --- set brightness - if required: dim at night
        uint8_t howBright = displayBrightness[modeDisplayNormalBrightness];
        if (modeDisplayDimAtNight == 1) {
           RTC_Date tnow = ttgo->rtc->getDateTime();
           uint8_t hour = tnow.hour;
           if (hour < 7 || hour > 22) howBright = howBright/2 + 15;
        } 
        ttgo->setBrightness(howBright);
        
        lastActivityTime = millis();  
        // --- pick next watchface (fixed, random, cycle)
        if (activeScreen <= MAX_WATCH_SCREENS) {                // regular watchface (no settings or stopwatch)
           if (modeWatchfaceChanges == 1) {                       // in random mode 
              byte prevScreen = activeScreen;
              while (activeScreen == prevScreen) activeScreen = random(MAX_WATCH_SCREENS) + 1;
           } else if (modeWatchfaceChanges == 2) {                // in cycle mode  
              activeScreen++;
              if (activeScreen > MAX_WATCH_SCREENS) activeScreen = 1;
           }
        } else if (activeScreen == 101) {      // if it was the "settings entry" screen -> go back to last watchface 
           activeScreen = lastActiveWatch;  
        } else if (activeScreen > 101 && (millis()-lightSleepTime) > 60000) {  // if sleep was >1 min -> leave Settings
           activeScreen = lastActiveWatch;    
        }
    }
}




// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// ---
void setup()
{
    Serial.begin (115200);
    
    // --- Create a program that allows the required message objects and group flags
    g_event_queue_handle = xQueueCreate(20, sizeof(uint8_t));
    g_event_group = xEventGroupCreate();
    isr_group = xEventGroupCreate();

    ttgo = TTGOClass::getWatch();

    //  --- Initialize TWatch
    ttgo->begin();

    // Turn on the IRQ used
    ttgo->power->adc1Enable(AXP202_BATT_VOL_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1, AXP202_ON);
    ttgo->power->enableIRQ(AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_FINISHED_IRQ, AXP202_ON);
    ttgo->power->clearIRQ();
    
    // --- Turn off unused power
    ttgo->power->setPowerOutPut(AXP202_EXTEN, AXP202_OFF);
    ttgo->power->setPowerOutPut(AXP202_DCDC2, AXP202_OFF);
    ttgo->power->setPowerOutPut(AXP202_LDO3, AXP202_OFF);
    ttgo->power->setPowerOutPut(AXP202_LDO4, AXP202_OFF);

    // --- Initialize lvgl
    ttgo->lvgl_begin();

    // --- Initialize bma423
    ttgo->bma->begin();

    // --- Enable BMA423 interrupt
    ttgo->bma->attachInterrupt();

    
    // --- Connection interrupted to the specified pin
    pinMode(BMA423_INT1, INPUT);
    attachInterrupt(BMA423_INT1, [] {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        EventBits_t  bits = xEventGroupGetBitsFromISR(isr_group);
        if (bits & WATCH_FLAG_SLEEP_MODE)
        {
            // Use an XEvent when waking from low energy sleep mode.
            xEventGroupSetBitsFromISR(isr_group, WATCH_FLAG_SLEEP_EXIT | WATCH_FLAG_BMA_IRQ, &xHigherPriorityTaskWoken);
        } else
        {
            // Use the XQueue mechanism when we are already awake.
            uint8_t data = Q_EVENT_BMA_INT;
            xQueueSendFromISR(g_event_queue_handle, &data, &xHigherPriorityTaskWoken);
        }

        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR ();
        }
    }, RISING);
    

    // --- Connection interrupted to the specified pin
    pinMode(AXP202_INT, INPUT);
    attachInterrupt(AXP202_INT, [] {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        EventBits_t  bits = xEventGroupGetBitsFromISR(isr_group);
        if (bits & WATCH_FLAG_SLEEP_MODE)
        {
            // Use an XEvent when waking from low energy sleep mode.
            xEventGroupSetBitsFromISR(isr_group, WATCH_FLAG_SLEEP_EXIT | WATCH_FLAG_AXP_IRQ, &xHigherPriorityTaskWoken);
        } else
        {
            // Use the XQueue mechanism when we are already awake.
            uint8_t data = Q_EVENT_AXP_INT;
            xQueueSendFromISR(g_event_queue_handle, &data, &xHigherPriorityTaskWoken);
        }
        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR ();
        }
    }, FALLING);

    // --- Synchronize time to system time
    ttgo->rtc->syncToSystem();
    
    // --- Clear lvgl counter
    lv_disp_trig_activity(NULL);

    // --- when initialization complete - turn on the backlight
    ttgo->openBL();
    ttgo->setBrightness(displayBrightness[modeDisplayNormalBrightness]);
    setCpuFrequencyMhz(cpuFrequencyNormal);

    // --- use sec and day to initialize random generator
    RTC_Date tnow = ttgo->rtc->getDateTime();
    randomSeed(tnow.second+60*tnow.day);
    
    startScreen(); 
    delay(900);
    ttgo->tft->fillScreen(TFT_BLACK);
    delay(300);
    int16_t x,y,tx,ty;
    if (ttgo->getTouch(x,y)) {    // read touch values that occurred during start screen
       while (ttgo->getTouch(tx,ty)) {}; // if touch -> wait for touch to end
    }
    
    createTask();    // create the task to be performed by the LVGL task handler
    lv_task_handler(); 
    

    // --- Enable double tap wakeup.
    ttgo->bma->enableWakeupInterrupt(true);
    ttgo->setBrightness(displayBrightness[modeDisplayNormalBrightness]);
}


// -------------------------------------------------------------------
// -------------------------------------------------------------------
// -------------------------------------------------------------------
// ---
void loop()
{
    
    bool  rlst;
    uint8_t data;
    
    // An XEvent signifies that there has been a wakeup interrupt, bring the CPU out of low energy mode
    EventBits_t  bits = xEventGroupGetBits(isr_group);
    if (bits & WATCH_FLAG_SLEEP_EXIT) {
        if (lenergy) {
            lenergy = false;
	          setCpuFrequencyMhz(cpuFrequencyNormal);
        }

        low_energy();

        if (bits & WATCH_FLAG_BMA_IRQ) {
            do {
                rlst =  ttgo->bma->readInterrupt();
            } while (!rlst);
            xEventGroupClearBits(isr_group, WATCH_FLAG_BMA_IRQ);
        }
        if (bits & WATCH_FLAG_AXP_IRQ) {
            ttgo->power->readIRQ();
            ttgo->power->clearIRQ();
            xEventGroupClearBits(isr_group, WATCH_FLAG_AXP_IRQ);
        }
        xEventGroupClearBits(isr_group, WATCH_FLAG_SLEEP_EXIT);
        xEventGroupClearBits(isr_group, WATCH_FLAG_SLEEP_MODE);
    }

    if ((bits & WATCH_FLAG_SLEEP_MODE)) {
        //! No event processing after entering the information screen
        return;
    }
    
    //! Normal polling
    if (xQueueReceive(g_event_queue_handle, &data, 5 / portTICK_RATE_MS) == pdPASS) {
        switch (data) {
        case Q_EVENT_BMA_INT:
            do {
                rlst =  ttgo->bma->readInterrupt();
            } while (!rlst);

            if (ttgo->bma->isDoubleClick()) {
              ttgo->setBrightness(255);
            }
         
            break;
        case Q_EVENT_AXP_INT:
            ttgo->power->readIRQ();
            if (ttgo->power->isPEKShortPressIRQ()) {
                ttgo->power->clearIRQ();
                low_energy();
                return;
            }
            ttgo->power->clearIRQ();
            break;
      
        default:
            break;
        }

    }

    if (lv_disp_get_inactive_time(NULL) < displayStayOnTime[modeDisplayStayOn]*1000 ||
                   (millis()-lastActivityTime) < displayStayOnTime[modeDisplayStayOn]*1000) { 
        lv_task_handler();     
    } else {
        low_energy();
    }
    
    
   
      
}
