/*
 * Copyright (c) 2021 Markus Wobisch
 * 
 *  https://github.com/mawob/bttfWatch
 *  
 *  This file includes the code pieces for the settings screens
 *  used for (but not necessaryly limited to) the bttfWatch
 *  
 *  made for the T-Watch 2000 v1
 *  
 */

 
/*
     Subroutines for the Settings Screens of the BTTF watch 

     --- These routines are included:
     settingsEntry(uint8_t flag)
     announceUpdate(uint8_t flag)
     settingsWatchface()
     settingsDisplay()
     settingsTime(uint8_t flag) {
     settingsDate(uint8_t flag) {
     drawStopWatch()
     OperateStopWatch(n)
     screenDrawTitle(String title)
     drawTextButton(uint8_t row, String text1,String text2, uint8_t frame) 
     drawTriangle(uint8_t x, uint8_t y, uint8_t direction ,uint16_t color, uint8_t frame) {
     drawGrid() 

*/


// ---------------------------------------------------------------
// --- Settings Entry Screen 
// ---------------------------------------------------------------
void settingsEntry(uint8_t flag) {
  
   uint16_t col = color565(30,60,165);
   uint16_t colt = color565(240,240,100);
  
   ttgo->tft->fillRect(20,  70, 200, 32, TFT_RED);
   ttgo->tft->fillRect(40, 120, 160, 32, TFT_RED);
   int font = 4;
   ttgo->tft->setTextColor(TFT_WHITE);
   ttgo->tft->drawString("ENTER  THE", 49, 75, font); 
   ttgo->tft->drawString("SETTINGS",  60, 125, font); 
   
   drawTriangle(30, 210, 4, col, 0);
   drawTriangle(210, 210, 2, col, 0);
   ttgo->tft->fillRect(64, 193, 112, 35, col);
 
   //uint8_t font = 4;
   ttgo->tft->setTextColor(colt); 
   ttgo->tft->drawString("S W I P E", 70, 200, font);
   
}

// ---------------------------------------------------------------
// --- Announce time or date updates - Screen 
// ---------------------------------------------------------------
void announceUpdate(uint8_t flag) {

   ttgo->tft->fillScreen(TFT_BLACK);
   uint8_t x = 40;
   uint8_t y = 75;
   
   ttgo->tft->fillRect(x+20,  y-5, 122, 32, TFT_RED);
   ttgo->tft->fillRect(x+14-16, y+60-5, 164, 32, TFT_RED);
   int font = 4;
   ttgo->tft->setTextColor(TFT_WHITE);

   if (flag == 1) ttgo->tft->drawString("TIME", x+50, y, font); 
   else if (flag == 2) ttgo->tft->drawString("DATE", x+50, y, font); 
   ttgo->tft->drawString("IS  UPDATED", x+6, y+60, font); 

   activeScreen = lastActiveWatch;
   delay(750);
   
}


// ---------------------------------------------------------------
// --- Settings Watchface - Screen
// ---------------------------------------------------------------
void settingsWatchface(uint8_t flag) {

   String text1[3] = {"   fixed"," random","   cycle"};   // three options
   String text2[3] = {"     12h","12h+sec","     24h"};  // three options
   String text3[2] = {"       off","       on"};  // two options
   String text4[2] = {"       off","       on"};  // two options
    
   if (flag == 0) {
      ttgo->tft->fillScreen(TFT_BLACK); 
      drawTextButton(1,"watchface",      text1[modeWatchfaceChanges],0);
      drawTextButton(2,"format",       text2[modeDisplay1224sec],0);
      drawTextButton(3,"step count", text3[modeDisplayStepCounter],0);
      drawTextButton(4,"battery %",  text4[modeDisplayBattery],0);
   } else if (flag == 1) {
      modeWatchfaceChanges++;
      if (modeWatchfaceChanges == 3) modeWatchfaceChanges = 0 ;
      drawTextButton(1,"watchface",      text1[modeWatchfaceChanges],1);
   } else if (flag == 2) {
      modeDisplay1224sec++;
      if (modeDisplay1224sec == 3) modeDisplay1224sec = 0;
      drawTextButton(2,"format",       text2[modeDisplay1224sec],1);
   }  else if (flag == 3) {
      modeDisplayStepCounter++;
      if (modeDisplayStepCounter == 2) modeDisplayStepCounter = 0;
      drawTextButton(3,"step count", text3[modeDisplayStepCounter],1);
   }  else if (flag == 4) {
      modeDisplayBattery++;
      if (modeDisplayBattery == 2) modeDisplayBattery = 0;
      drawTextButton(4,"battery %",  text4[modeDisplayBattery],1);
   } 
   
}


// ---------------------------------------------------------------
// --- Settings Display - Screen 
// ---------------------------------------------------------------
void settingsDisplay(uint8_t flag) {
   String text1[3] = {"     7 sec","   12 sec","   20 sec"};  // three options -> match the actual values in configure.h
   String text2[3] = {"       low","      med","      high"};  // three options
   String text3[2] = {"   no-dim","       dim"};              // two options

   if (flag == 0) {
      ttgo->tft->fillScreen(TFT_BLACK); 
      drawTextButton(2,"stay-on",    text1[modeDisplayStayOn],0);
      drawTextButton(3,"brightness", text2[modeDisplayNormalBrightness],0);
      drawTextButton(4,"at night",   text3[modeDisplayDimAtNight],0);
   } else if (flag == 2) {
      modeDisplayStayOn++;
      if (modeDisplayStayOn == 3) modeDisplayStayOn = 0 ; 
      drawTextButton(2,"stay-on",    text1[modeDisplayStayOn],1);
   } else if (flag == 3) {
      modeDisplayNormalBrightness++;
      if (modeDisplayNormalBrightness == 3) modeDisplayNormalBrightness = 0;
      ttgo->setBrightness(displayBrightness[modeDisplayNormalBrightness]);
      drawTextButton(3,"brightness", text2[modeDisplayNormalBrightness],1);
   }  else if (flag == 4) {
      modeDisplayDimAtNight++;   
      if (modeDisplayDimAtNight == 2) modeDisplayDimAtNight = 0;
      drawTextButton(4,"at night",   text3[modeDisplayDimAtNight],1);
   } 
}


      
// ---------------------------------------------------------------
// --- Set Time Screen 
// ---------------------------------------------------------------
void settingsTime(uint8_t flag) {
   static uint8_t  localMin, localHour;
   uint8_t  localDay, localMonth;
   uint16_t localYear;
   uint16_t col = color565(30,60,165);
   uint8_t xpos = 89; 
   uint8_t yp;
   uint8_t font = 7;
    
   if (flag == 0) {
      ttgo->tft->fillScreen(TFT_BLACK);
      //drawGrid(); // for testing 
      
      // --- up arrows 
      yp = 18; 
      drawTriangle(120,yp,1,col, 1);
      drawTriangle(200,yp,1,col, 1);
      // --- down arrows
      yp = 168;
      drawTriangle(120,yp,3,col, 1);
      drawTriangle(200,yp,3,col, 1); 

      font = 4;
      ttgo->tft->setTextColor(color565(240,240,100)); 
      ttgo->tft->setTextSize(2);
      yp = 17;
      ttgo->tft->drawString("+", 110, yp, font);
      ttgo->tft->drawString("+", 190, yp, font);
      yp = 122;
      ttgo->tft->drawString("-", 112, yp, font);
      ttgo->tft->drawString("-", 192, yp, font);
   
      ttgo->tft->fillRoundRect(18, 198, 204, 41, 16, color565(255,0,0));
      ttgo->tft->fillRoundRect(20, 200, 200, 37, 14, color565(30,60,165));
      ttgo->tft->setTextSize(1);
      ttgo->tft->setTextColor(color565(240,240,100));
      ttgo->tft->drawString("S E T   T I M E", 48, 208, font);
     
 
      // - get the current date & time
      RTC_Date tnow = ttgo->rtc->getDateTime();
      localMin   = tnow.minute;
      localHour  = tnow.hour;
      
   } else if (flag == 1) {   // +
      localHour += 1;
      if (localHour == 23) localHour = 0;
   } else if (flag == 2) {
      localMin += 1;
      if (localMin == 60) localMin = 0;
   } else if (flag == 3) {   // -
       if (localHour > 0) localHour--;
       else localHour = 23;
   } else if (flag == 4) {
       if (localMin > 0) localMin--;
       else localMin = 59;  
   } else if (flag == 5) {   // save
       // set time - use current year/month/day
       RTC_Date tnow = ttgo->rtc->getDateTime();
       localDay   = tnow.day;
       localMonth = tnow.month;
       localYear  = tnow.year; 
       ttgo->rtc->setDateTime(localYear, localMonth, localDay, localHour, localMin, 0);
       announceUpdate(1);
   } 
   
   yp = 69;
   // --- draw time
   font = 7;
   ttgo->tft->setTextSize(1);
   ttgo->tft->setTextColor(color565(23,23,0)); 
   ttgo->tft->drawString("88:88", xpos, yp, font);
   if (localHour < 10) xpos += ttgo->tft->drawChar('0', xpos, yp, font);
   ttgo->tft->setTextColor(TFT_YELLOW);
   xpos += ttgo->tft->drawNumber(localHour, xpos, yp, font);
   xpos += ttgo->tft->drawChar(':', xpos, yp, font); 
   if (localMin < 10) xpos += ttgo->tft->drawChar('0', xpos, yp, font);
   xpos += ttgo->tft->drawNumber(localMin, xpos, yp, font);

}



// ---------------------------------------------------------------
// --- Set Date Screen 
//       prevent from setting impossible days 
//       (take into account months&leap years)
// ---------------------------------------------------------------
void settingsDate(uint8_t flag) {
   String monthStr[12]={ "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
   uint8_t  localSec, localMin, localHour;
   static uint8_t  localDay, localMonth;
   static uint16_t localYear;
   uint8_t xpos = 15; 
   uint8_t yp;
   uint8_t font = 4;
     
   if (flag == 0) {
      ttgo->tft->fillScreen(TFT_BLACK); 
      //drawGrid();  // for testing 
      uint16_t col = color565(30,60,165);
      yp = 20;
      drawTriangle(40,yp,1,col, 1);
      drawTriangle(120,yp,1,col,1);
      drawTriangle(200,yp,1,col,1);
      yp = 156;
      drawTriangle(40,yp,3,col, 1);
      drawTriangle(120,yp,3,col,1);
      drawTriangle(200,yp,3,col,1); 
      ttgo->tft->setTextColor(color565(240,240,100)); 
      ttgo->tft->setTextSize(2);
      yp = 19;
      ttgo->tft->drawString("+", 30, yp, font);
      ttgo->tft->drawString("+", 110, yp, font);
      ttgo->tft->drawString("+", 190, yp, font);
      yp = 110;
      ttgo->tft->drawString("-", 32, yp, font);
      ttgo->tft->drawString("-", 112, yp, font);
      ttgo->tft->drawString("-", 192, yp, font);
   
      ttgo->tft->fillRoundRect(18, 198, 204, 41, 16, color565(255,0,0));
      ttgo->tft->fillRoundRect(20, 200, 200, 37, 14, color565(30,60,165));
      ttgo->tft->setTextSize(1);
      ttgo->tft->setTextColor(color565(240,240,100));
      ttgo->tft->drawString("S E T   D A T E", 44, 208, font);
      
      // - get the current date & time
      RTC_Date tnow = ttgo->rtc->getDateTime();
      localDay   = tnow.day;
      localMonth = tnow.month;
      localYear  = tnow.year; 
      
   } else if (flag == 1) {   // +
      if (localMonth < 12) localMonth++;
      else localMonth = 1;
      if (localDay > lastDayOfMonth(localMonth,localYear)) localDay = lastDayOfMonth(localMonth,localYear);
   } else if (flag == 2) {
      if (localDay < lastDayOfMonth(localMonth,localYear)) localDay++;
      else localDay = 1;
   } else if (flag == 3) {
      if (localYear < 9999) localYear++;
      else localYear = 0;
      if (localDay > lastDayOfMonth(localMonth,localYear)) localDay = lastDayOfMonth(localMonth,localYear);
   } else if (flag == 4) {   // -
      if (localMonth > 1) localMonth--;
      else localMonth = 12;
      if (localDay > lastDayOfMonth(localMonth,localYear)) localDay = lastDayOfMonth(localMonth,localYear);
   } else if (flag == 5) {
      if (localDay > 1) localDay--;
      else localDay = lastDayOfMonth(localMonth,localYear);
   } else if (flag == 6) {
      if (localYear > 0) localYear--;
      else localYear = 9999;
      if (localDay > lastDayOfMonth(localMonth,localYear)) localDay = lastDayOfMonth(localMonth,localYear);
   } else if (flag == 7) {   // save
      // set date&time - with current hour,min,sec
      RTC_Date tnow = ttgo->rtc->getDateTime();
      localHour = tnow.hour;
      localMin = tnow.minute;
      localSec = tnow.second; 
      ttgo->rtc->setDateTime(localYear, localMonth, localDay, localHour, localMin, localSec);
      announceUpdate(2);
   } 
 
   
   yp = 79; 
   // need a black box to clear previous entry
   ttgo->tft->fillRoundRect(10, yp-9, 225, 39, 4, color565(45,15,10));
   ttgo->tft->setTextSize(1);
   ttgo->tft->setTextColor(color565(255,255,0)); 
   ttgo->tft->drawString(monthStr[localMonth-1], xpos+6, yp, font);
   if (localDay < 10) ttgo->tft->drawNumber(localDay, xpos+98, yp, font);
   else ttgo->tft->drawNumber(localDay, xpos+91, yp, font);
   ttgo->tft->drawNumber(localYear, xpos+157, yp, font);

}



// ---------------------------------------------------------------
// --- StopWatch Screen - draw
// ---------------------------------------------------------------
void drawStopwatch() {
   ttgo->tft->fillScreen(TFT_BLACK);
   //ttgo->tft->fillScreen(TFT_BLUE);
  
   uint16_t colOut = color565(70,70,70);
   uint16_t colMid = color565(20,20,20);
   uint16_t colIn = color565(0,0,0);
   uint16_t colBut = color565(10,10,10);
   
   uint8_t triang = 50;
   ttgo->tft->fillTriangle(0,triang, triang,0, triang,triang, colOut);
   ttgo->tft->fillTriangle(239,triang, 239-triang,0, 239-triang,triang, colOut);
   ttgo->tft->fillRect(triang,  0, 240-triang-triang, 240, colOut);
   ttgo->tft->fillRect(0,  triang+1, 240, 240-triang-triang, colOut);
   ttgo->tft->fillTriangle(0,239-triang, triang,239, triang,239-triang, colOut);
   ttgo->tft->fillTriangle(239,239-triang, 239-triang,239, 239-triang,239-triang, colOut);
   
   uint8_t t1 = 19;
   uint8_t d1 = 15;
   ttgo->tft->fillTriangle(d1,50 , d1+t1,50, d1+t1,50-t1, colMid);
   ttgo->tft->fillTriangle(239-d1,50 , 239-d1-t1,50, 239-d1-t1,50-t1, colMid); 
   ttgo->tft->fillRect(d1+t1, 50-t1, 240-2*d1-2*t1, t1, colMid);
   ttgo->tft->fillRect(d1, 50, 240-2*d1, 20, colMid);
   ttgo->tft->fillRoundRect(d1, 60, 240-2*d1, 90, 10, colMid);

   ttgo->tft->fillRoundRect(80, 220, 80, 20, 5, colMid);

   // - buttons
   uint8_t w=60;
   uint8_t h=34;
   uint8_t ypos=166;
   ttgo->tft->fillRoundRect(240/2-w/2, ypos, w, h, 7, colBut);
   ttgo->tft->fillRoundRect(240/2-w/2-w-20, ypos, w, h, 7, colBut);
   ttgo->tft->fillRoundRect(240/2-w/2+w+20, ypos, w, h, 7, colBut);
   uint8_t font = 4;
   ttgo->tft->setTextColor(color565(200,170,170));
   ttgo->tft->drawString("Start", 14, ypos+6, font);
   ttgo->tft->drawString("Stop", 93, ypos+6, font);
   ttgo->tft->drawString("Rst", 180, ypos+6, font);

   // - time display
   uint8_t dist = 25;
   ttgo->tft->fillRoundRect(dist, 66, 240-2*dist, 55, 2, colIn);
   font = 7;
   ttgo->tft->setTextColor(color565(20,20,0)); 
   ttgo->tft->drawString("88:88:8", 28, 69, font);
   ttgo->tft->setTextColor(color565(255,255,0));
   ttgo->tft->drawString("00:00:0", 28, 69, font);
   
}

// ---------------------------------------------------------------
// --- StopWatch operate
//
//    input:   flag   0:continue  >0:button press start/stop/reset
//    3 modes: 0:stopped  1:running   2:running-but-display-is-not-updated
// ---------------------------------------------------------------
void operateStopwatch(uint8_t flag) { 
  
   static uint8_t mode = 0;     // 0:stopped   1:running(=update elapsed time)  2:running(but fix display time)
   static uint32_t startTime = 0; 
   static uint32_t elapsedTime = 0; 
   static uint8_t prevTenth = 99;
   static uint8_t prevSec = 99;
   static uint8_t prevMin = 99;
   
   uint8_t x = 28; 
   uint8_t y = 69;
   
   if (flag == 1) {             // start
      if (mode == 0) startTime = millis();
      mode = 1;
   } else if (flag == 2) {      // stop
      if (mode != 0) {
        elapsedTime = (millis()-startTime)/100;
        mode = 2;
      }
   } else if (flag == 3) {      // reset
      elapsedTime = 0;
      mode = 0;
   }

   if (mode == 1) {      // running
      elapsedTime = (millis()-startTime)/100;
   }
   uint8_t tenth = elapsedTime%10; 
   uint8_t sec = (elapsedTime/10)%60; 
   uint8_t min = (elapsedTime/600)%60;
  
   // --- display min, sec, thenths
   int font = 7;
   uint16_t col1 = color565(20,20,0);
   uint16_t col2 = color565(255,255,0);
   uint8_t xpos; 
   
   if (min != prevMin) {
      prevMin = min;
      xpos = x;
      ttgo->tft->setTextColor(col1);
      ttgo->tft->drawString("88", xpos, y, font);
      ttgo->tft->setTextColor(col2); 
      if (min < 10) xpos += ttgo->tft->drawChar('0', xpos, y, font);
      ttgo->tft->drawNumber(min, xpos, y, font);
   }

   if (sec != prevSec) { 
      prevSec = sec; 
      xpos = 104;
      ttgo->tft->setTextColor(col1);
      ttgo->tft->drawString("88", xpos, y, font);
      ttgo->tft->setTextColor(col2); 
      if (sec < 10) xpos += ttgo->tft->drawChar('0', xpos, y, font);
      ttgo->tft->drawNumber(sec, xpos, y, font);
   }
    
   if (tenth != prevTenth) {
      prevTenth = tenth;
      xpos = 180;
      ttgo->tft->setTextColor(col1);
      ttgo->tft->drawString("8", xpos, y, font);
      ttgo->tft->setTextColor(col2); 
      ttgo->tft->drawNumber(tenth, xpos, y, font);
   }
   
}



// ---------------------------------------------------------------
// ---------------------------------------------------------------
// ---------------------------------------------------------------
// --- Helpers
// ---------------------------------------------------------------
// ---------------------------------------------------------------
// ---------------------------------------------------------------


// ---------------------------------------------------------------
// --- draw text button
//          frame   0:nothing   1:draw a frame around the button for a short time   
// ---------------------------------------------------------------
void drawTextButton(uint8_t row, String text1,String text2, uint8_t frame) {

   uint8_t x;
   uint8_t y;
   uint8_t font;

   // --- button value - right field
   x = 13;
   y = 63*(row-1);
   
   // --- short flicker - if button was pressed
   if (frame == 1) {
      ttgo->tft->drawRoundRect(x-9, y+6, 233, 43, 18, color565(255,255,0));
      ttgo->tft->drawRoundRect(x-10, y+5, 235, 45, 19, color565(215,215,0));
      ttgo->tft->drawRoundRect(x-11, y+4, 235, 47, 20, color565(255,255,0));
      delay(75); 
   }

   // frame
   ttgo->tft->drawRoundRect(x-11, y+4, 237, 47, 20, color565(155,0,00));  // 3 dot frame
   ttgo->tft->fillRoundRect(x-10, y+5, 235, 45, 19, color565(255,0,00));  // 2 dot frame
   
   ttgo->tft->fillRoundRect(x+90, y+7, 133, 41, 17, color565(30,60,165));   // right: dark
   ttgo->tft->fillRoundRect(x-8, y+7, 115, 41, 17, color565(180,210,240));  // left: bright 
   // --- color transition
   for (uint8_t i=0; i<76; i++) ttgo->tft->drawLine(x+90+i,y+7,x+90+i,y+47,color565(180-+i*2,210-i*2,240-i));
   
   font = 4;
   ttgo->tft->setTextColor(TFT_BLACK); 
   ttgo->tft->drawString(text1, x+3, y+17, font); 
   ttgo->tft->setTextColor(color565(240,240,100));
   ttgo->tft->drawString(text2, x+120, y+17, font); 


}


// ---------------------------------------------------------------
// --- draw triangle
//           direction 1:up 2:right 3:down 4:left
// ---------------------------------------------------------------
void drawTriangle(uint8_t x, uint8_t y, uint8_t direction ,uint16_t color, uint8_t frame) {

   //uint16_t col2 = color565(255,255,0);
   uint16_t col2 = color565(255,0,0);
   uint8_t l = 34;
   uint8_t h = 34;
   uint8_t frWidth = 2;
   if (direction == 1) {
     if (frame == 1) ttgo->tft->fillTriangle(x,y-frWidth-1, x+h/2+frWidth,y+l+frWidth, x-l/2-frWidth,y+l+frWidth, col2);
     ttgo->tft->fillTriangle(x,y, x+h/2,y+l, x-l/2,y+l, color);
     //if (frame == 1) ttgo->tft->drawTriangle(x,y-1, x+h/2+1,y+l+1, x-l/2-1,y+l+1, col2);
   } else if (direction == 2) {
      if (frame == 1) ttgo->tft->fillTriangle(x+frWidth+1,y, x-l-frWidth,y+h/2+frWidth, x-l-frWidth,y-h/2-frWidth, col2);
      ttgo->tft->fillTriangle(x,y, x-l,y+h/2, x-l,y-h/2, color);
   } else if (direction == 3) { 
      if (frame == 1) ttgo->tft->fillTriangle(x,y+frWidth+1, x-h/2-frWidth,y-l-frWidth, x+h/2+frWidth,y-l-frWidth, col2);
      ttgo->tft->fillTriangle(x,y, x-h/2,y-l, x+h/2,y-l, color);
   } else if (direction == 4) {  
      if (frame == 1) ttgo->tft->fillTriangle(x-frWidth-1,y, x+l+frWidth,y-h/2-frWidth, x+l+frWidth,y+h/2+frWidth, col2);
      ttgo->tft->fillTriangle(x,y, x+l,y-h/2, x+l,y+h/2, color);
   }
}


// ---------------------------------------------------------------
// --- draw screen grid
//     for testing: visualize the 12 touch regions for placing buttons
//
void drawGrid() {
   ttgo->tft->drawLine(80,28,80,239,color565(255,0,0));
   ttgo->tft->drawLine(160,28,160,239,color565(255,0,0));
   
   ttgo->tft->drawLine(0,58,239,58,color565(255,100,0));
   ttgo->tft->drawLine(0,121,239,121,color565(255,100,0));
   ttgo->tft->drawLine(0,184,239,184,color565(255,100,0));

}
