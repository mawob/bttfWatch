
/*
 * Copyright (c) 2021 Markus Wobisch
 * 
 *  https://github.com/mawob/bttfWatch
 *  
 *  This file includes the subroutines used for (but not 
 *  necessaryly limited to) the bttfWatch 
 *  
 *  made for the T-Watch 2000 v1
 *  
 */


// ---------------------------------------------------------------
// --- Create the main task
// ---
// ---------------------------------------------------------------
void createTask()
{
    static uint32_t user_data = 10;
    lv_task_t * task = lv_task_create(myTask, 3, LV_TASK_PRIO_MID, &user_data);  
}

// ---------------------------------------------------------------
// --- My Task
// ---    
// ---------------------------------------------------------------
void myTask(lv_task_t * task)
{

   // --- update screen
   screenUpdate();
   
   // --- check for touch input
   uint8_t touch = checkTouch();
   if (touch != 0) {
      Serial.print(" touch result "); Serial.println(touch); 
   }
   // --- actions, based on swipes or touch
   if (touch != 0) { 
      if (touch < 10) handleSwipe(touch);
      else handleTouch(touch);
   }
   

   
}

// ---------------------------------------------------------------
// --- handle touch
// ---
// ---------------------------------------------------------------
void handleTouch(uint8_t touch) {

  uint8_t cx = touch%10;   //  0,1,2
  uint8_t cy = touch/10;   //  1,2,3,4,5

  // ---- stopwatch
  if (activeScreen == 51) {
     if (cy > 3) {
        if (cx == 0) operateStopwatch(1);        // start 
        else if (cx == 1) operateStopwatch(2);   // stop
        else if (cx == 2) operateStopwatch(3);   // reset
     }
  } else if (activeScreen == 102) {   
     settingsWatchface(cy-1);
  } else if (activeScreen == 103) {   
     settingsDisplay(cy-1);
  } else if (activeScreen == 104) { 
     uint8_t n = 99; 
     if (cy == 2 && cx > 0) {
       n = cx; 
     } else if (cy == 4 && cx > 0) {
       n = cx +2; 
     } else if (cy == 5) {
       n = 5;
     }
     settingsTime(n);
  } else if (activeScreen == 105) { 
     uint8_t n = 99;
     if (cy == 2) {
       n = cx+1; 
     } else if (cy == 4) {
       n = cx+4; 
     } else if (cy == 5) {
       n = 7;
     }
     settingsDate(n);
  }
}

// ---------------------------------------------------------------
// --- handle swipe
// ---
// --- different actions for watchfaces, stopwatch, settings
// ---------------------------------------------------------------
uint8_t handleSwipe(uint8_t swipe) {

   if ( activeScreen <= MAX_WATCH_SCREENS) {    // watch faces
      if (swipe == 1) activeScreen++;
      else if (swipe == 2) activeScreen--;

      // --- deal with boundaries
      if (activeScreen>MAX_WATCH_SCREENS) activeScreen = 1;
      else if (activeScreen == 0) activeScreen = MAX_WATCH_SCREENS;
      
      // --- remember last watchface, before going to settings/stopwatch
      lastActiveWatch = activeScreen;
      
      if (swipe == 3) activeScreen = 101;    // settings entry
      else if (swipe == 4) activeScreen = 51;    // 51 stop watch
     
   }
   
   else if ( activeScreen == 51 ) {     // in Stopwatch
      if (swipe == 3) activeScreen = 1;    // last watchface
   }
   
   else if ( activeScreen > 100 ) {     // in Settings
      if (swipe < 3) {                           // left or right
         if (swipe == 1) activeScreen++;
         else if (swipe == 2) activeScreen--;
         // --- deal with boundaries
         if (activeScreen>(MAX_SET_SCREENS+100)) activeScreen = 102;    // omit the 101 "entry" screen
         else if (activeScreen <= 101) activeScreen = MAX_SET_SCREENS+100;
      }
      
      if (swipe == 4) activeScreen = lastActiveWatch ;        // up: last watchface
   }
   
}


// ---------------------------------------------------------------
// --- check for touch
// ---
// ---   return:
// ---     0:nothing  1:swipeL  2:swipeR 3:swipeUp   4:swipeDown
// ---
// ---------------------------------------------------------------
uint8_t checkTouch() {

   uint8_t flag; 
   int16_t x, y, tx, ty;
   uint32_t startTime = 0;
   uint32_t duration = 0;
   
   flag = 0;
   if (ttgo->getTouch(x, y)) { // touch was registered
      startTime = millis();
      while (ttgo->getTouch(tx, ty)) {} // wait for touch to end

      lastActivityTime = millis();
      duration = lastActivityTime - startTime;  // duration of the touch/swipe - maybe use later

      // --- check for swipes
      uint8_t length = 120;   // --- min length to be recognized as swipe
      if ( (x-tx) > length ) flag = 1;       // --- left swipe
      else if ( (tx-x) > length ) flag = 2;  // --- right swipe
      else if ( (y-ty) > length ) flag = 3;  // up 
      else if ( (ty-y) > length ) flag = 4;  // down 

      // --- no swipe -> check for touch (in 3 x-ranges and 4 y-ranges)
      // --- ensure that a non-successful swipe is not misinterpreted as a touch
      if (abs(tx-x)<30 && abs(ty-y)<30) {         // no "tiny" swipe was registered
         if ( x<80 )               flag = 10;     // left column
         else if ( x>80 && x<160)  flag = 11;     // mid column
         else if ( x>160)          flag = 12;     // right column
         
         // vertical regions for touch:  22-74, 82-130, 138-186, >194
         if ( y<58)                 flag += 10;     // row 1
         else if ( y>=58 && y<121)  flag += 20;     // row 2
         else if ( y>=121 && y<184) flag += 30;     // row 3 
         else if ( y>=184)          flag += 40;     // row 4
      }
   }
   
   return flag;
}


// ---------------------------------------------------------------
// --- Update screen
// ---             input: screen No.
// ---------------------------------------------------------------
void screenUpdate() {
   static uint8_t lastScreen = 199;
   static uint32_t lastTimeTime = 0;
   static uint32_t lastTimeSteps = 0;
   static uint32_t lastTimeBattery = 0;

   // --- if the screen changes
   if (lastScreen != activeScreen) {
      lastScreen = activeScreen;
      screenInit(activeScreen); 
      if (activeScreen < 50) {   //< 50) {  // only for watchfaces - not for settings/stopwatch screens
         if (activeScreen != 2 && activeScreen != 5) displayDateTime(1);
         displaySteps(); 
         displayBattery();
      }
   }
   if (activeScreen == 1) {
      updateFlux();
   } else if (activeScreen == 2) {
      tcOperate(0);
      tcBlink(); 
   } else if (activeScreen == 3) {
      updateSpeedoAnalog();
   } else if (activeScreen == 4) {
      nextSid();
      updateSid();
   } else if (activeScreen == 5) {
      twinPinesMall(0);

   } else if (activeScreen == 51) {   // stopwatch
      operateStopwatch(0);
      
   } else if (activeScreen == 101) {   // main settings entry screen
      //settingsEntry(0);
   }
      
   // --- update time/date - only for watchfaces - not for settings/stopwatch screens
   if (activeScreen < 50) {
      if (activeScreen != 2 && activeScreen != 5 && (millis()-lastTimeTime) > 250) {
         lastTimeTime = millis();
         displayDateTime(0);
      }
      if ( (millis()-lastTimeBattery) > 60000) {
         lastTimeBattery = millis();
         displayBattery();
      }
      if ( (millis()-lastTimeSteps) > 1000) {
         lastTimeSteps = millis();
         displaySteps(); 
      }
   }
   
}


// ---------------------------------------------------------------
// --- Initialize/Draw a new screen
// ---             input: screen No.
// ---------------------------------------------------------------
void screenInit(uint8_t screen) {
    
    ttgo->tft->fillScreen(TFT_BLACK); 
    //ttgo->tft->fillScreen(TFT_BLUE);   // for testing
    if (screen == 1) {
      fluxCapEnclosure();
    } else if (screen == 2) {
      tcEnclosure();
      tcOperate(1); 
    } else if (screen == 3) {
      speedoEnclosure();
      agEnclosure();
    } else if (screen == 4) {
      sidEnclosure();
    } else if (screen == 5) {
      twinPinesMall(1);
       
    } else if (screen == 51) { // Stopwatch
      drawStopwatch();
       
    } else if (screen == 101) { // Settings Entry Screen
      settingsEntry(0);
    } else if (screen == 102) { // Settings Watchface
      settingsWatchface(0);
    } else if (screen == 103) { // Settings Display 
      settingsDisplay(0);
    } else if (screen == 104) { 
      settingsTime(0);
    } else if (screen == 105) {
      settingsDate(0);
    }
}


// ---------------------------------------------------------------
// --- display the date and time
//             input force  =0(nothing)  =1(force update)
// rethink blink colon - works only in seconds-mode
// ---------------------------------------------------------------

// dw add code to reset step count when the day changes
uint8_t yday = 99;    // dw yesterday, start at 99 so never = to a real day

void displayDateTime(uint8_t force)
{
   uint16_t colDark7segm = color565(13,13,0);    // color of a dark segment in 7-seg display
   static uint8_t previousSecond = 99;
   static uint8_t previousMinute = 99;
   static uint8_t previousHour = 99;
   static uint8_t blinkColon = 0;
   uint16_t colorColon = TFT_YELLOW; 
   String monthStr[12]={ "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
      
   uint8_t hh, mm, ss, mmonth, dday; // H, M, S variables
   uint16_t yyear; // Year is 16 bit int
   
   // - get the current date & time
   RTC_Date tnow = ttgo->rtc->getDateTime();
   // - extract quantities 
   ss = tnow.second; 
   mm = tnow.minute;
   if (force == 0) {
      if (modeDisplay1224sec == 1) {    // seconds are displayed
         if (ss == previousSecond) return;
         previousSecond = ss;
      } else {            // seconds are not displayed         
         if (mm == previousMinute) return;
         previousMinute = mm;
      }
   }
   hh = tnow.hour;
   dday = tnow.day;
   mmonth = tnow.month;
   yyear = tnow.year; 

   // dw reset step counter when day changes
   if (dday != yday){
      log_i("reset setcounter: %d != %d", dday, yday );
      yday = dday;  // set yesterday to today as flag that counter was reset
      ttgo->bma->resetStepCounter();
   }
   
   uint8_t font = 7;     // 7-segment font
   ttgo->tft->setTextSize(1);
   ttgo->tft->setTextColor(colDark7segm, TFT_BLACK); // color for dark segments 
    
   int xpos = 75;
   if (modeDisplay1224sec == 1) xpos = xpos-50; // shift if seconds are displayed
   int yp =192;
   
   // --- hours - in 12h or 24h display
   if (hh != previousHour || force == 1) {
      previousHour = hh;
   
      if (modeDisplay1224sec < 2) {       // --- 12h display or 12+sec
         if (modeDisplay1224sec == 1) ttgo->tft->drawString("18:88:88", xpos, yp, font); 
         else ttgo->tft->drawString("18:88", xpos, yp, font);
         uint8_t hhampm = hh;
         if (hhampm == 0) hhampm = 12;
         else if (hhampm >12) hhampm = hhampm-12; 
         if (hhampm < 10) xpos += ttgo->tft->drawChar('1', xpos, yp, font);
         ttgo->tft->setTextColor(TFT_YELLOW);
         xpos += ttgo->tft->drawNumber(hhampm, xpos, yp, font);
      
      } else {                      // --- 24h display
         xpos += 20;
         ttgo->tft->drawString("88:88", xpos, yp, font);
         if (hh < 10) xpos += ttgo->tft->drawChar('0', xpos, yp, font);
         ttgo->tft->setTextColor(TFT_YELLOW);
         xpos += ttgo->tft->drawNumber(hh, xpos, yp, font);
      }
      
      // --- colon
      ttgo->tft->setTextColor(TFT_YELLOW);
      xpos += ttgo->tft->drawChar(':', xpos, yp, font);

      // --- month, day
      xpos = 9;
      if (modeDisplay1224sec == 1) xpos = xpos-9; // shift if seconds are displayed
      uint8_t ypM = 195;
      uint8_t fontM = 4;
      ttgo->tft->setTextColor(TFT_GREENYELLOW);
      ttgo->tft->drawString(monthStr[mmonth-1], xpos, ypM, fontM); 
      // day
      ttgo->tft->setTextColor(TFT_GREENYELLOW);
      if (dday > 9) ttgo->tft->drawString(String(dday), xpos+9, ypM+24, fontM);
      else ttgo->tft->drawString(String(dday), xpos+19, ypM+24, fontM);
   }

   
   if (modeDisplay1224sec == 0) xpos = 151;
   else if (modeDisplay1224sec == 1) xpos = 101;
   else if (modeDisplay1224sec == 2) xpos = 171;

   // --- clear min/sec displays before re-writing  
   ttgo->tft->setTextColor(colDark7segm, TFT_BLACK); // color for dark segments 
   if (modeDisplay1224sec == 1) ttgo->tft->drawString("18:88:88", xpos, yp, font); 
   else if (modeDisplay1224sec == 0) ttgo->tft->drawString("18:88", xpos, yp, font);
   else ttgo->tft->drawString("88:88", xpos, yp, font);
   
   ttgo->tft->setTextColor(TFT_YELLOW);
         
   // --- minutes
 
   ttgo->tft->setTextColor(TFT_YELLOW);
   if (mm < 10) xpos += ttgo->tft->drawChar('0', xpos, yp, font);
   xpos += ttgo->tft->drawNumber(mm, xpos, yp, font);
   // --- seconds - only in one of the two 12h modes
   if (modeDisplay1224sec == 1) {
      ttgo->tft->setTextColor(colorColon);
      xpos += ttgo->tft->drawChar(':', xpos, yp, font);
      ttgo->tft->setTextColor(TFT_YELLOW);
      if (ss < 10) xpos += ttgo->tft->drawChar('0', xpos, yp, font);
      ttgo->tft->drawNumber(ss, xpos, yp, font);
   }

   // --- AM/PM only in 12h display, and only if no seconds are displayed
   font = 4;
   yp = 195;
   if (modeDisplay1224sec == 0) {
      ttgo->tft->setTextColor(TFT_YELLOW);
      if (hh<12) ttgo->tft->drawString("A", 217+2, yp+1, font);
      else ttgo->tft->drawString("P", 217+2, yp+1, font);
      ttgo->tft->drawString("M", 217, yp+22, font);
   }

}


// ---------------------------------------------------------------
// --- display the step counter
//
// ---------------------------------------------------------------
void displaySteps() {
   uint8_t font = 2;
    
   if (modeDisplayStepCounter == 1) {   // display the step counter 
      ttgo->tft->setTextColor(TFT_GREEN);
      uint16_t stepCounter = ttgo->bma->getCounter();
      ttgo->tft->drawString(String(stepCounter), 2, 2, font);
   }
  
}

// ---------------------------------------------------------------
// --- display the battery percentage
//
// ---------------------------------------------------------------
void displayBattery() {
   uint8_t font = 2;
   
   if (modeDisplayBattery == 1) {   // display the battery percentage
      ttgo->tft->setTextColor(TFT_GREEN);
      //ttgo->power->adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, true);
      uint8_t perc = ttgo->power->getBattPercentage();
      ttgo->tft->drawString(String(perc) + "%", 205, 2, font);
      // Turn off the battery adc
      //ttgo->power->adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, false);
   }
}


// ---------------------------------------------------------------
// --- determine leap years
// ---------------------------------------------------------------
bool isLeapYear(uint16_t year) {
   bool isLeap = FALSE;
   if (year%4 == 0) isLeap = TRUE;
   if (year%100 == 0) isLeap = FALSE;
   if (year%400 == 0) isLeap = TRUE;
   return isLeap;
}

// ---------------------------------------------------------------
// --- determine last day in February
// ---------------------------------------------------------------
uint8_t lastDayOfMonth(uint8_t month, uint16_t year) {
   uint8_t lastDay = 0;
   uint8_t maxDay[12]={31,28,31,30,31,30,31, 31,30,31,30,31};
   lastDay = maxDay[month-1];
   if (month == 2 && isLeapYear(year)) lastDay = 29;
   return lastDay;
}

// ---------------------------------------------------------------
// --- convert RGB to 565 color
// ---------------------------------------------------------------
uint16_t color565(uint8_t red, uint8_t green, uint8_t blue)
{
    uint16_t b = (blue >> 3) & 0x1f;
    uint16_t g = ((green >> 2) & 0x3f) << 5;
    uint16_t r = ((red >> 3) & 0x1f) << 11;
    return (uint16_t) (r | g | b);
}
