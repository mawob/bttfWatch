/*
 * Copyright (c) 2021 Markus Wobisch
 * 
 *  https://github.com/mawob/bttfWatch
 *  
 *  bttfWatch:
 *  This file includes the bttf-specific code pieces that display the watchfaces
 *  
 *  made for the T-Watch 2000 v1
 *  
 */

// --- define colors
uint16_t colFluxElbow = color565(90,90,90);
uint16_t colFluxElbow2 = color565(120,120,130);

uint16_t colTcCase = color565(38,38,38);
uint16_t colTcOpenings = color565(0,0,0);
uint16_t colTcLabelsRed = color565(245,0,0);
uint16_t colTcLabelsBlack = color565(0,0,0);
uint16_t colTcLabelsFont = color565(180,160,140);
uint16_t colTcDigits[3] = {color565(235,0,0),
                         color565(0,240,0),
                         color565(200,200,0)};
uint16_t colTcLeds[3] = {color565(150,150,0),
                         color565(0,150,0),
                         color565(150,150,0)};

uint16_t colSpeedoCase = color565(220,170,65);

uint16_t colAgCase = color565(63,71,77);
uint16_t colAgScrew = color565(100,100,105);
uint16_t colAgBright = color565(205,150,105);         // warm white
uint16_t colAgOff = color565(80,0,0);     
uint16_t colAgEmpty = color565(243,0,0);     



// --- movie-related dates + actor's birthdays
const byte noDates = 14;
unsigned int myDates[noDates][3] = {      // all month-day, year, hour-min
  {1026,1985,121},       // 8 relevant dates from the movie
  {1026,1985,122},     
  {1105,1955,600},
  {1112,1955,1838},
  {704,1776,812},
  {1225,0,1603},
  {1021,2015,1928},
  {101,1885,0},
  {609,1961,1201},       // Michael J Fox 
  {1022,1938,1125},      // Christopher Lloyd 
  {415,1959,1811},       // Tom Wilson
  {705,1966,1143},       // Claudia Wells 
  {208,1953,803},        // Mary Steenburgen
  {921,1929,1527}        // Elsa Raven
};



// ---------------------------------------------------------------
// --- Start Screen
// ---------------------------------------------------------------
void startScreen() {
   ttgo->tft->fillScreen(TFT_BLACK);
   uint8_t x = 30;
   uint8_t y = 85;
   uint8_t r;
   
   uint16_t col;
   col = TFT_RED;
   
   uint8_t font = 4;
   ttgo->tft->setTextColor(TFT_WHITE);
 
   r = random(3);
   if (r == 0) {
      ttgo->tft->fillRect(x-20,  y-5, 202, 32, col);
      ttgo->tft->fillRect(x+14-16, y+51-6, 164, 32, col);
      ttgo->tft->drawString("SHIELD  EYES", x, y, font); 
      ttgo->tft->drawString("FROM  LIGHT", x+4, y+50, font);
   } else if (r == 1) {
      ttgo->tft->fillRect(x-5,  y-5, 175, 32, col);
      ttgo->tft->fillRect(x-22, y+51-6, 211, 32, col);
      ttgo->tft->drawString("SAVE  THE", x+20, y, font); 
      ttgo->tft->drawString("CLOCK  TOWER", x-10, y+50, font);
   } else if (r == 2) {
      ttgo->tft->fillRect(x-30,  y-25, 231, 32, col);
      ttgo->tft->fillRect(x-10, y+51-26, 188, 32, col);
      ttgo->tft->fillRect(x-27, y+102-27, 230, 32, col);
      ttgo->tft->drawString("PLEASE  EXCUSE", x-15, y-20, font); 
      ttgo->tft->drawString("THE  CRUDITY", x, y+30, font);
      ttgo->tft->drawString("OF  THIS  MODEL", x-10, y+80, font);
   }
   
}


// ---------------------------------------------------------------
// --- code for Flux Capacitor
// ---------------------------------------------------------------

void fluxCapEnclosure() {
  const byte x = 45;
  const byte y = 20;  
  const byte w = 142;
  const byte h = 170;
  const byte th = 19;

  //ttgo->tft->fillScreen(TFT_BLACK);

  // elbows
  const byte elbowwidth = 22;
  ttgo->tft->fillRoundRect(x+w*0.5-elbowwidth*0.5, y-20, elbowwidth, 40, 11, colFluxElbow);   // elbow top
  ttgo->tft->fillRoundRect(x+w*0.5-3, y-16, 6, 3, 2, colFluxElbow2);   // elbow reflection 
  ttgo->tft->fillRoundRect(x+w-20, y+h*0.5-12, 40, elbowwidth, 11, colFluxElbow);   // elbow
  ttgo->tft->fillRoundRect(x+w+5, y+h*0.5-7, 8, 3, 2, colFluxElbow2);   // elbow reflection 
  
  // enclosure
  uint16_t colFluxEnc1 = color565(45, 45, 65);
  uint16_t colFluxEnc2 = color565(35, 35, 53);
  uint16_t colFluxEnc3 = color565(8, 8, 8);
  uint16_t colFluxEnc4 = color565(52, 52, 73);
  ttgo->tft->fillRoundRect(x, y, w, h, 8, colFluxEnc1);
  ttgo->tft->fillRoundRect(x+th, y+th, w-2*th, h-2*th, 22, TFT_BLACK);
  ttgo->tft->fillRoundRect(x+th+5, y+th+5, w-2*th-10, h-2*th-10, 20, colFluxEnc3);

  ttgo->tft->fillRoundRect(x+3, y+h-4, w-6, 4, 2, colFluxEnc2);  // shades
  ttgo->tft->fillRoundRect(x, y+3, 3, h-6, 2, colFluxEnc2);
  ttgo->tft->fillRoundRect(x+w-4, y+3, 3, h-6, 2, colFluxEnc4);

  
  // bases 
  uint16_t colFluxBase = color565(60, 52, 20);
  const byte baseradius = 10;
  const byte horizshift = 34;
  const byte vertshift = 45; //51; 
  const byte vertshift2 = 34;
  ttgo->tft->fillCircle(x+w*0.5,y+h*0.5+vertshift,baseradius,colFluxBase);
  ttgo->tft->fillCircle(x+w*0.5-horizshift,y+h*0.5-vertshift2,baseradius,colFluxBase);
  ttgo->tft->fillCircle(x+w*0.5+horizshift,y+h*0.5-vertshift2,baseradius,colFluxBase);
 
  // yellow cables 
  uint16_t colFluxCable = color565(220, 200, 0);
  const byte plugLength = 18;
  const byte plugWidth = 10;
  const byte cableLength = 28;
  const byte cableWidth = 6;
  ttgo->tft->fillRoundRect(x+w*0.5-plugWidth*0.5+plugLength-3, y+h*0.5+vertshift-plugWidth*0.5+2, cableLength, cableWidth, 3, colFluxCable);
  ttgo->tft->fillRoundRect(x+w*0.5-horizshift-plugWidth*0.5+2, y+h*0.5-vertshift2-plugWidth*0.5+plugLength-3, cableWidth, cableLength, 3, colFluxCable); 
  ttgo->tft->fillRoundRect(x+w*0.5+horizshift-plugWidth*0.5+2, y+h*0.5-vertshift2-plugWidth*0.5+plugLength-3, cableWidth, cableLength, 3, colFluxCable);

  // red plugs
  uint16_t colFluxPlug = color565(210, 0, 0);
  ttgo->tft->fillRoundRect(x+w*0.5-plugWidth*0.5, y+h*0.5+vertshift-plugWidth*0.5, plugLength, plugWidth, 5, colFluxPlug);
  ttgo->tft->fillRoundRect(x+w*0.5-horizshift-plugWidth*0.5, y+h*0.5-vertshift2-plugWidth*0.5, plugWidth, plugLength, 5, colFluxPlug); 
  ttgo->tft->fillRoundRect(x+w*0.5+horizshift-plugWidth*0.5, y+h*0.5-vertshift2-plugWidth*0.5, plugWidth, plugLength, 5, colFluxPlug);
  ttgo->tft->fillRect(x+w*0.5-plugWidth*0.5+5, y+h*0.5+vertshift-plugWidth*0.5, plugLength-5, plugWidth, colFluxPlug);
  ttgo->tft->fillRect(x+w*0.5-horizshift-plugWidth*0.5, y+h*0.5-vertshift2-plugWidth*0.5+5, plugWidth, plugLength-5, colFluxPlug); 
  ttgo->tft->fillRect(x+w*0.5+horizshift-plugWidth*0.5, y+h*0.5-vertshift2-plugWidth*0.5+5, plugWidth, plugLength-5, colFluxPlug);

  // acrylics
  const byte centx = 116;
  const byte centy = 105;
  uint16_t colFluxAcrylic = color565(30, 30, 40);
  for (int n = -3 ; n < 4; n++) {
    ttgo->tft->drawLine(centx+n, centy+7-6, centx+n, centy+38-6, colFluxAcrylic);   // bottom
    if (abs(n)<3) {
      ttgo->tft->drawLine(centx-2+n, centy-5-n, centx-22-2+n, centy-22-5-n, colFluxAcrylic); // left
      ttgo->tft->drawLine(centx+2-n, centy-5-n, centx+22+2-n, centy-22-5-n, colFluxAcrylic); // right
    }
  }

  // red labels
  const byte labelwidth = 4;
  ttgo->tft->fillRect(x+w*0.5-41,y+4, 83, labelwidth, TFT_RED);
  ttgo->tft->fillRect(x+w*0.5-26,y+11, 53, labelwidth, TFT_RED);
  ttgo->tft->fillRect(x+w*0.5-36,y+h*0.64, 73, labelwidth, TFT_RED);
}



// -----------------------------------------------------------------
// --- updateFlux      operate the Flux Capacitor
// -----------------------------------------------------------------
void updateFlux() {
  static unsigned long previousTime1 = 0;
  static unsigned long previousTime2 = 0;
  static unsigned interval1 = 5000;
  static unsigned interval2 = 50;
  static byte step = 0;
  
  if ((millis() - previousTime1) > interval1) {
    previousTime1 = millis();
    interval1 = 10000 + random(13000);
    //interval2 = 20 + random(45);   // for four LEDs 
    interval2 = 15 + random(35);    // for six LEDs
    //activity =  (65-interval2)/2;   // range: 0-22  used in SID approx speed/4
  }
    
  if ((millis() - previousTime2) > interval2) {
    previousTime2 = millis();
    step++;
    if (step > 14) step = 0; 
    if (0 < step && step <6) setFluxLed(step-0,1);
    if (1 < step && step <7) setFluxLed(step-1,2);
    if (2 < step && step <8) setFluxLed(step-2,3);
    if (3 < step && step <9) setFluxLed(step-3,4);
    if (4 < step && step <10) setFluxLed(step-4,5);
    if (5 < step && step <11) setFluxLed(step-5,6);
    const byte x = 45;
    const byte y = 20;
    const byte w = 142;
    const byte h = 170;
    const byte labelwidth = 4;
    if (step < 9) ttgo->tft->fillRect(x+w*0.5-5,y+h*0.64, 11, labelwidth, TFT_RED); 
  }
  
}



// -------------------------------------------------------------
void setFluxLed(byte ledNo, byte state) {
  uint16_t color;
  const byte centx = 116;
  const byte centy = 105;
  byte i = 0;

  if (state == 1) color = color565(255,220,160);         // warm white
  else if (state == 2) color = color565(130,110,100);
  else if (state == 3) color = color565(106,90,90);
  else if (state == 4) color = color565(70,60,50);
  else if (state == 5) color = color565(35,35,45);
  else if (state == 6) color = color565(30, 30, 40);
  
  // set LED 
  i = 6-ledNo;
  ttgo->tft->fillCircle(centx,centy+i*6-2,2,color);
  ttgo->tft->fillCircle(centx-i*4,centy-i*4-3,2,color);
  ttgo->tft->fillCircle(centx+i*4,centy-i*4-3,2,color); 
}



// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------


// -----------------------------------------------------------------------
// --- SID code
// -----------------------------------------------------------------------
// global variables
byte activity = 14;    // needs to be varied... range 0-22 see MIcroTC code
byte peak[10];
byte lastPeak[10];

// -----------------------------------------------------------------------
// --- draw SID enclosure
// 
void sidEnclosure() {
  byte x = 82;
  byte y = 5;   
  byte w1 = 85;
  byte w2 = 109;
  byte h = 170;

  // - colors
  uint16_t colSidCase = color565(19,19,5);
  uint16_t colSidLine = color565(69,65,29);

  ttgo->tft->fillRect(x, y, w2, h, colSidCase);
  ttgo->tft->drawRect(x, y, w2, h, colSidLine);
  ttgo->tft->fillRect(x+(w2-w1)/2, y , w1, h, colSidCase);
  ttgo->tft->drawRect(x+(w2-w1)/2, y , w1, h, colSidLine);

  // mounting holes
  ttgo->tft->fillCircle(x+7, y+20, 3, colSidLine);
  ttgo->tft->fillCircle(x+w2-7, y+20, 3, colSidLine);
  ttgo->tft->fillCircle(x+7, y+h-20, 3, colSidLine);
  ttgo->tft->fillCircle(x+w2-7, y+h-20, 3, colSidLine);

  // cable
  uint16_t col1 = color565(25,25,37);
  uint16_t col2 = color565(39,39,53);
  ttgo->tft->fillRect(x*0.5, y+h+8, x*0.5+w2-9, 6, col1);
  for (byte n = 0 ; n < 22; n++) { 
    ttgo->tft->fillRect(x*0.5+5+n*6, y+h+6+1, 3, 8, col2);
  }

  // cable holders
  ttgo->tft->fillRect(x+(w2-w1)/2+10, y+h-5,    8, 20, color565(148,0,148));
  ttgo->tft->fillRect(x+(w2-w1)/2+w1-16, y+h-5, 8, 20, color565(148,0,148));
  ttgo->tft->drawRect(x+(w2-w1)/2+10, y+h-5,    8, 20, color565(67,47,67));
  ttgo->tft->drawRect(x+(w2-w1)/2+w1-16, y+h-5, 8, 20, color565(67,47,67));
  ttgo->tft->fillRect(x+(w2-w1)/2+10+3, y+h-5+3,    2, 14, color565(38,38,38));
  ttgo->tft->fillRect(x+(w2-w1)/2+w1-16+3, y+h-5+3, 2, 14, color565(38,38,38));

  //  LED lenses -> draw Sid
  redrawSid();

  
  // - gas bottle
  uint16_t colGasBottle = color565(150,16,14);
  uint16_t colGasBottle3 = color565(190,20,18); 
  uint16_t colGasBottle4 = color565(180,24,22);

  ttgo->tft->fillRoundRect(x/2-10, y+10, 10, 10, 5, color565(105,63,25));   // bronze pipe hole
  ttgo->tft->fillRoundRect(x/2-8, y+12, 6, 30, 3, color565(205,127,50));   // bronze pipe
  //ttgo->tft->fillRect(40, 74, 1, 2, color565(60,50,50));
  ttgo->tft->fillRoundRect(x/2-14, y+26, 18, 30, 9, colGasBottle);   // top
  ttgo->tft->fillRoundRect(x/2-25, y+46, 40, 120, 18, colGasBottle4);  // bottle
  ttgo->tft->fillRoundRect(x/2-18, y+56, 18, 100, 9, colGasBottle3);
  // holder
  ttgo->tft->fillRect(x/2-25, y+66, 40, 2, TFT_BLACK); 
  ttgo->tft->fillRoundRect(x/2-29, y+68, 48, 4, 2, TFT_DARKGREY); 
  ttgo->tft->fillRect(x/2-25, y+72, 40, 2, TFT_BLACK); 

  ttgo->tft->fillRect(x/2-25, y+126, 40, 2, TFT_BLACK); 
  ttgo->tft->fillRoundRect(x/2-29, y+128, 48, 4, 2, TFT_DARKGREY); 
  ttgo->tft->fillRect(x/2-25, y+132, 40, 2, TFT_BLACK); 

}

// -----------------------------------------------------------------
// --- updateSid        update the SID display (only the changes)
//     peak[i]=0      i=0(no LED) -> i from 1-20
// -----------------------------------------------------------------
void updateSid() {
  for (byte col = 0 ; col < 10; col++) { 
    if (peak[col] != lastPeak[col]) {
      if (peak[col] > lastPeak[col]) {
      
        for (byte i = lastPeak[col]; i < peak[col]; i++) {
          // draw additional LEDs
          plotSid(col, i+1, 1);  
        }
      } else if (peak[col] < lastPeak[col]) { 
        for (byte i = peak[col]; i <lastPeak[col]; i++) { 
         // turn off LEDs
         plotSid(col, i+1, 0);
        }
      }
      lastPeak[col] = peak[col];
    }
  }
}

// -----------------------------------------------------------------
// --- redrawSid        draw the full SID display
//     peak[i]=0      i=0(no LED) -> i from 1-20
// -----------------------------------------------------------------
void redrawSid() {
  for (byte col = 0 ; col < 10; col++) { 
    for (byte i = 0; i < 20; i++) {
      if (peak[col] > i) {
        plotSid(col, i+1, 1);  
      } else {
        plotSid(col, i+1, 0);
      }
    }
  } 
}

// -----------------------------------------------------------------
// --- nextSid        compute next SID configuration - random walk
//                    the amount of action depends on the global 
//                    variable "activity" which is in the range 0-22
//                    (for a real SID, this would correspond to 1/4*speed)
// -----------------------------------------------------------------
void nextSid() {
  // state: 0: down  1:stop-down  2:stop-up  3:up
  //
  static unsigned long previousTime = 0;
  static unsigned interval = 3;
  static byte state[10];
  static byte col = 0;
  byte nbrL = 0;
  byte nbrR = 0;
   
  if ((millis() - previousTime) > interval) {
    previousTime = millis();

    // - find neighbors
    if (col > 0) {
      nbrL = col - 1;
    } else {
      nbrL = col + 1;
    }
    if (col < 9) {
      nbrR = col + 1;
    } else {
      nbrL = col - 1;
    }

    // - change state - when reached bottom/top - or when stopped
    if (state[col] == 0 && peak[col] ==  1) {               // when reached bottom - stop
      state[col] = 1;
    } else if (state[col] == 3 && peak[col] == 20) {        // when reached top - stop
      state[col] = 2;
    } else if (state[col] == 1 && random(100) < 20) {       // when stopped down - go up
      state[col] = 3;
    } else if (state[col] == 2 && random(100) < 20) {       // when stopped up - go down
     state[col] = 0;
     
    } else if (state[col] == 0 && peak[col] < peak[nbrR] && state[nbrR] == 3 && random(100) < 18) { // follow nbrR up
      state[col] = 1;
    } else if (state[col] == 0 && peak[col] < peak[nbrL] && state[nbrL] == 3 && random(100) < 18) { // follow nbrL up
      state[col] = 1;
    } else if (state[col] == 3 && peak[col] > peak[nbrR] && state[nbrR] == 0 && random(100) < 20) { // follow nbrR down
      state[col] = 1;
    } else if (state[col] == 3 && peak[col] > peak[nbrL] && state[nbrL] == 0 && random(100) < 20) { // follow nbrL down
      state[col] = 1;
      
    //} else if (state[col] == 3 && random(100) < 8) {       // small chance to change up direction
    } else if (state[col] == 3 && random(100) < (10-activity*0.2)) {       // small chance to change up direction
      state[col] = 2;
    //} else if (state[col] == 0 && random(100) < 4) {        // small chance to change down direction
    } else if (state[col] == 0 && random(100) < (2+activity*0.2)) {        // small chance to change down direction
      state[col] = 1;
    }

    //if (state[col] == 3 && peak[col] < 20 && random(100) < 30) {        // go up w/ prob  def:30
    if (state[col] == 3 && peak[col] < 20 && random(100) < (20+activity)) {        // go up w/ prob
      peak[col]++;
    //} else if (state[col] == 0 && peak[col] > 0 && random(100) < 35) {  // go down w/ prob.  def:35
    } else if (state[col] == 0 && peak[col] > 0 && random(100) < (45-activity)) {  // go down w/ prob.
      peak[col]--;
    }
          
    col++;
    if (col > 9) col = 0;  
       
  } 
  
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// --- plot or remove(val=0) circle in SID panel
void plotSid(byte x, byte y, byte val) {
  uint16_t colSidLenses = color565(28,28,12);
  uint16_t colSidLenses2 = color565(36,35,19);
  uint16_t colSidGreen = color565(0,200,0);
  uint16_t colSidYellow = color565(210,210,0);
  
  uint16_t color;
  if (val == 0) {
    color = colSidLenses;
  } else if (y < 14) { 
    color = colSidGreen;   //TFT_GREEN;  // darker green
  } else if (y < 20) {
    color = colSidYellow; //TFT_YELLOW;
  } else {
    color = TFT_RED;
  }
  ttgo->tft->fillCircle(xSidConvert(x), ySidConvert(y), 2, color);
  if (val == 0) {
    ttgo->tft->fillCircle(xSidConvert(x), ySidConvert(y), 1, colSidLenses2);
  }
}

// -----------------------------------------------------------------------------
// --- convert SID coordinates to display coordinates
byte xSidConvert(byte x) {
   return 82+18+x*8;
   //return 88+x*9;
}
byte ySidConvert(byte y) {
   return 174-y*8;
   //return 174-y*9;
}






// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------


// -----------------------------------------------------------------------------
void updateSpeedoAnalog() {
  static unsigned long previousTime = 0;
  static unsigned long previousTimeS= 0;
  static int pos[3] = {4,4,6};
  const byte maxPos[3] = {4,4,6};
  static int move[3] = {-1,-1,-1};
  static int speed = 0;
  static int speedDir = 1;
  //static byte keep = 0;
  static unsigned long keepStart = 0;
  
  if ((millis() - previousTime) > 10 ) {
    previousTime = millis();

    // - needles
    for (byte i=0; i<3; i++) {
      if (random(200) > 196) pos[i] += move[i];
      if (pos[i] < 0) {
        pos[i] = 0;
        move[i] = - move[i]; 
      } else if (pos[i] > maxPos[i]) {
        pos[i] = maxPos[i];
        move[i] = - move[i]; 
      }
    }
    agNeedles(pos[0],pos[1],pos[2]);
   
    // - speed
    if ((millis() - keepStart) > 2900 ) {
      keepStart = 0;
      if (random(100) > 90) {
        speed += speedDir;
        if (speed > 88) {
          speed = 88;
          speedDir  = -speedDir;
          keepStart = millis();
        } else if (speed < 0) {
          speed = 0;
          speedDir  = - speedDir;
          keepStart = millis();
          agBlink();   // blink red warning light
        }
        tcNumber(speed,4,0);
      }
    }
  }
}

// -----------------------------------------------------------------------------
void speedoEnclosure() { 
  
  const byte x = 26;
  const byte y = 57;
  const byte w = 100; // 90 
  const byte h = 38;
  ttgo->tft->fillRoundRect(x, y, w, h, 2, colSpeedoCase);
  ttgo->tft->fillRect(x+27, y+4, w*31/59, h*20/24, colTcLabelsBlack);
  ttgo->tft->fillRect(x+2, y+4, 23, 9, colTcLabelsBlack);
  ttgo->tft->fillRect(x+6, y+15, 16, 9, colTcLabelsBlack);
  ttgo->tft->fillRect(x+6, y+26, 16, 9, color565(190,0,0));
  
}


// --------------------------------------------------------------
// --------------------------------------------------------------
// -------------------   uses delay - problem ????
void agBlink() {
 
  for (byte i=0; i<2; i++) {
    agEmpty(1);
    delay(600); //900);
    agEmpty(0);
    delay(400); //600); 
  }
}
 
// --------------------------------------------------------------
// ------------------------------------------------------------
void agEmpty(byte state) { 
  // from AG encosure
  const byte x = 2;
  const byte y = 100;
  if (state == 1) 
   ttgo->tft->fillRect(x+140+7+24+1, y+3+7+1, 21-2, 11-2,colAgEmpty); 
  else
    ttgo->tft->fillRect(x+140+7+24, y+3+7, 21, 11,colAgOff); 
}


// --------------------------------------------------------------
// ------------------------------------------------------------
void agNeedles(byte pos0, byte pos1, byte pos2) {
  static byte prevPosit[3] = {0,0,0};
  byte pos[3];
  pos[0] = pos0;
  pos[1] = pos1;
  pos[2] = pos2;
  
  for (byte i=0; i<3; i++) {
    agDrawNeedle(i,prevPosit[i],0); 
    agDrawNeedle(i,pos[i],1); 
    prevPosit[i] = pos[i];
  }
}

// ------------------------------------------------------------
void agDrawNeedle(byte n, byte pos, byte col) { 
  // from AG enclosure
  const byte x = 2;
  const byte y = 100;
    
  uint16_t color;
  if (col == 1) color = colTcLabelsBlack; 
  else color = colAgBright;
      
  if (n==0) {
    // needle - left
    ttgo->tft->drawLine(x+35-6+3*pos, y+3+7+25, x+35-10+5*pos, y+3+7+12+abs(pos-2), color);
    ttgo->tft->drawLine(x+36-6+3*pos, y+3+7+25, x+36-10+5*pos, y+3+7+12+abs(pos-2), color);
  } else if (n == 1) {
    ttgo->tft->drawLine(x+95-6+3*pos, y+3+7+23, x+95-10+5*pos, y+3+7+12+abs(pos-2), color);
    ttgo->tft->drawLine(x+96-6+3*pos, y+3+7+23, x+96-10+5*pos, y+3+7+12+abs(pos-2), color);
  } else {
    ttgo->tft->drawLine(x+140+7+33-6+2*pos, y+3+7+34, x+140+7+33-18+6*pos, y+3+7+13+2*abs(pos-3), color);
    ttgo->tft->drawLine(x+140+7+34-6+2*pos, y+3+7+34, x+140+7+34-18+6*pos, y+3+7+13+2*abs(pos-3), color);
  }

}



// -----------------------------------------------------------------------------
void agEnclosure() {  
  const byte x = 2;
  const byte y = 100;
  const byte w = 236; 
  const byte h = 72;
  ttgo->tft->fillRect(x, y, w, h, colAgCase);
  
  ttgo->tft->fillCircle(x+36, y+34, 27, colTcLabelsBlack);
  ttgo->tft->fillCircle(x+36, y+34, 20, colAgBright);
  ttgo->tft->fillCircle(x+96, y+34, 27, colTcLabelsBlack);
  ttgo->tft->fillCircle(x+96, y+34, 20, colAgBright);
  
  ttgo->tft->fillRoundRect(x+140, y+3, 83, 61, 4,colTcLabelsBlack);
  ttgo->tft->fillRoundRect(x+140+7, y+3+7, 69, 36, 4,colAgBright);
  ttgo->tft->fillRect(x+140+7+24, y+3+7, 21, 11,colAgOff); 
  
  // screws
  ttgo->tft->fillCircle(x+7, y+15, 3, colAgScrew);
  ttgo->tft->fillCircle(x+w-7, y+15, 3, colAgScrew);
  ttgo->tft->fillCircle(x+7, y+h-15, 3, colAgScrew);
  ttgo->tft->fillCircle(x+w-7, y+h-15, 3, colAgScrew);
  
  // labels
  ttgo->tft->fillRect(x+36-17, y+37, 34, 6, colTcLabelsBlack);
  ttgo->tft->fillRect(x+96-17, y+35, 36, 6, colTcLabelsBlack); 
  ttgo->tft->fillRect(x+96-12, y+42, 26, 6, colTcLabelsBlack); 
  //ttgo->tft->fillRect(x+140+4, y+3+7+36+2, 75, 6, TFT_RED);//colTcLabelsBlack); 

  // label text
  for(byte j = 0 ; j < 8; j++) {
    ttgo->tft->fillRect(x+36-17+4+3.78*j, y+37+2 , 1, 2, colFluxElbow2);  
    ttgo->tft->fillRect(x+96-17+4+3.78*j, y+35+2 , 1, 2, colFluxElbow2);  
    ttgo->tft->fillRect(x+96-12+3+3.18*j, y+42+2 , 1, 2, colFluxElbow2);  
  }
  for(byte j = 0 ; j < 11; j++) { 
    ttgo->tft->fillRect(x+140+4+11+4.78*j, y+3+7+36+2+2 , 2, 2, colTcLabelsFont); 
  }
}


// --------------------------------------------------------------
// ----- Time Circuits ------------------------------------------
// --------------------------------------------------------------
void tcEnclosure() { 
  byte w = 240;
  byte yOffset = 57;
  byte h = 44;

  byte x;
  byte y;
  
  // - case plus labels  
  for(byte i = 0 ; i < 3; i++) {
    x = 0;
    //y = 24+i*27;
    y = yOffset+i*h;
    
    ttgo->tft->fillRect(x, y , w, 41, colTcCase);
    
    ttgo->tft->fillRect(12, y+3,  30, 3, colTcLabelsRed); 
    ttgo->tft->fillRect(63, y+3,  16, 3, colTcLabelsRed); 
    ttgo->tft->fillRect(110, y+3,  26, 3, colTcLabelsRed); 
    ttgo->tft->fillRect(167, y+3, 24, 3, colTcLabelsRed); 
    ttgo->tft->fillRect(209, y+3, 22, 3, colTcLabelsRed); 
   
    ttgo->tft->fillRect(83, y+33, 75, 4, colTcLabelsBlack); 
    for(byte j = 0 ; j < 12; j++) {
       if (j!=3) ttgo->tft->fillRect(89+5.37*j, y+34, 1, 2, colTcLabelsFont); 
    } 
  }

  // - display openings
  for(byte i = 0 ; i < 3; i++) {
    y = yOffset+i*h+9;
    x = 5;     // 5
    ttgo->tft->fillRect(x, y , 45, 22, colTcOpenings);
    x = 56;    // 6
    ttgo->tft->fillRect(x, y , 30, 22, colTcOpenings);
    x = 93;    // 7
    ttgo->tft->fillRect(x, y , 60, 22, colTcOpenings);
    x = 165;   // 12
    ttgo->tft->fillRect(x, y , 30, 22, colTcOpenings);
    x = 205;   // 10
    ttgo->tft->fillRect(x, y , 30, 22, colTcOpenings); 
               // 5
               
    
    // - test: am/pm
    ttgo->tft->fillCircle(159, y+6,2,colTcLeds[i]);
    // - labels: am/pm
    ttgo->tft->fillRect(156,y-1, 8, 3, colTcLabelsRed); 
    ttgo->tft->fillRect(156,y+10, 8, 3, colTcLabelsRed); 
  }
}


// -----------------------------------------------------------------------------
// operate TC
void tcOperate(uint8_t force) {
  static unsigned long previousTime = 0;
  static unsigned long previousTime2 = 0;
  static byte oldDate[6];
  static byte newDate[6] = {10,26,19,85,1,21};
  static uint8_t prevMinute = 99;
   
  // --- update current date/time
  if (force == 1 || (millis() - previousTime) > 1000) {
    previousTime = millis();

    uint8_t hou, minu, mon, da; 
    uint16_t yea;   
    // - get the current date & time
    RTC_Date tnow = ttgo->rtc->getDateTime();
    // - extract quantities 
    minu = tnow.minute;
    if (force == 1 || minu != prevMinute) {
       prevMinute = minu;
       hou  = tnow.hour;
       da   = tnow.day;
       mon  = tnow.month; 
       yea  = tnow.year;  
       tcMonth(mon,1);
       tcNumber(da,4,1); 
       tcNumber(yea/100,3,1);
       tcNumber(yea%100,2,1);
       uint8_t h12 = hou;
       if (hou == 0) h12 = 12;
       else if (hou >12) h12 = hou - 12; 
       tcNumber(h12,1,1);
       tcNumber(minu,0,1); 
      
       // --- am/pm LEDs
       if (hou < 12) {        // am
         ttgo->tft->fillCircle(159, 117,2,color565(0,244,0));
         ttgo->tft->fillCircle(159, 126,2,color565(0,0,0));
       } else {               // pm
         ttgo->tft->fillCircle(159, 117,2,color565(0,0,0));
         ttgo->tft->fillCircle(159, 126,2,color565(0,244,0));
       }
    }
  }

  
  // --- update top/bottom rows
  if (force == 1 || (millis() - previousTime2) > 11000) {
    previousTime2 = millis();

    // --- copy "next Date" to "prev Date"
    for (byte i=0; i<6; i++) {
      oldDate[i] = newDate[i]; 
    }
 
    // --- get a random new Date from list for upper display "next date"
    byte r = 1+random(noDates-1);
    newDate[0] = myDates[r][0]/100;
    newDate[1] = myDates[r][0]%100;
    newDate[2] = myDates[r][1]/100;
    newDate[3] = myDates[r][1]%100; 
    newDate[4] = myDates[r][2]/100;
    newDate[5] = myDates[r][2]%100;

    if (newDate[4] > 12) newDate[4] = newDate[4] - 12;
    else if (newDate[4] == 0) newDate[4] = 12; 

    // --- erase bottom
    tcMonth(13,2);
    for (byte i=0; i<5; i++) { 
       tcNumber(100,4-i,2); 
    }
    delay(100);
    
    // --- erase top & fill bottom
    tcMonth(13,0);
    for (byte i=0; i<5; i++) { 
       tcNumber(100,4-i,0); 
    }
    delay(50);
    tcMonth(oldDate[0],2);
    for (byte i=0; i<5; i++) {
       tcNumber(oldDate[i+1],4-i,2);  
    }
    
    // --- fill top display
    delay(100);
    tcMonth(newDate[0],0);
    for (byte i=0; i<5; i++) { 
       tcNumber(newDate[i+1],4-i,0); 
    }
    
  }
}



// -----------------------------------------------------------------------------
// blink colons in TC
void tcBlink() {
  static unsigned long previousTime = 0;
  static byte state = 0;
  byte yOffset = 57;
  byte h = 44;
    
  if ((millis() - previousTime) > 500) {
    previousTime = millis();
    for (byte row=0; row<3; row++) {
      byte y = yOffset+row*h;
      if (state == 0) {
        ttgo->tft->fillCircle(199,y+15,2,colTcOpenings );
        ttgo->tft->fillCircle(199,y+23,2,colTcOpenings );
      } else {
        ttgo->tft->fillCircle(199,y+15,2,colTcLeds[row]);
        ttgo->tft->fillCircle(199,y+23,2,colTcLeds[row]);     
      }
    }
    state = 1 - state;
  }
}


// -----------------------------------------------------------------------------
// draw a 2-dig number in a given color in a given row   
//    column: 0:min 1:hour 2:year-low 3:year-high 4:day
void tcNumber(byte number, byte col, byte row) {
   byte yOffset = 57;
   byte h = 44;
   byte xoffset[5] = {205,165,123,93,56};      // offset in x
   byte digitLo = number%10;
   byte digitHi = number/10;
    
   // clear display
   ttgo->tft->fillRect(xoffset[col],yOffset+row*h+9, 30, 22, color565(0,0,0)); 
   // write
   if (digitHi < 10) {
      tcDigit(xoffset[col]+15,   digitLo, row);
      tcDigit(xoffset[col], digitHi, row);
   }
}


// -----------------------------------------------------------------------------
// draw a 3-dig month in a given row     
//    column: 0:min 1:hour 2:year-low 3:year-high 4:day
void tcMonth(byte month, byte row) {
   byte yOffset = 57;
   byte h = 44;
   byte xoffset = 5;
 
  // clear display
  ttgo->tft->fillRect(xoffset,yOffset+row*h+9, 45, 22, color565(0,0,0)); 
  // write
  if (month == 1) {
    tcDigit(xoffset   , 10, row); 
    tcDigit(xoffset+15, 11, row);
    tcDigit(xoffset+30, 12, row); 
  } else if (month == 2) {
    tcDigit(xoffset,    13, row); 
    tcDigit(xoffset+15, 14, row);
    tcDigit(xoffset+30, 15, row); 
  } else if (month == 3) {
    tcDigit(xoffset,    16, row); 
    tcDigit(xoffset+15, 11, row);
    tcDigit(xoffset+30, 17, row); 
  } else if (month == 4) {
    tcDigit(xoffset,    11, row); 
    tcDigit(xoffset+15, 18, row);
    tcDigit(xoffset+30, 17, row); 
  } else if (month == 5) {
    tcDigit(xoffset,    16, row); 
    tcDigit(xoffset+15, 11, row);
    tcDigit(xoffset+30, 19, row); 
  } else if (month == 6) {
    tcDigit(xoffset,    10, row); 
    tcDigit(xoffset+15, 20, row);
    tcDigit(xoffset+30, 12, row); 
  } else if (month == 7) {
    tcDigit(xoffset,    10, row); 
    tcDigit(xoffset+15, 20, row);
    tcDigit(xoffset+30, 21, row); 
  } else if (month == 8) {
    tcDigit(xoffset,    11, row); 
    tcDigit(xoffset+15, 20, row);
    tcDigit(xoffset+30, 22, row); 
  } else if (month == 9) {
    tcDigit(xoffset,    5, row); 
    tcDigit(xoffset+15, 14, row);
    tcDigit(xoffset+30,    18, row); 
  } else if (month == 10) {
    tcDigit(xoffset, 0, row); 
    tcDigit(xoffset+15, 23, row);
    tcDigit(xoffset+30, 24, row); 
  } else if (month == 11) {
    tcDigit(xoffset, 12, row); 
    tcDigit(xoffset+15, 0, row);
    tcDigit(xoffset+30, 25, row); 
  } else if (month == 12) {
    tcDigit(xoffset,     0, row); 
    tcDigit(xoffset+15, 14, row);
    tcDigit(xoffset+30, 23, row); 
  } 
}

// -----------------------------------------------------------------------------
// draw a single digit in a given row   a-g -> 0-6
void tcDigit(byte y, byte digit, byte row) {
  if (digit == 0) {
    tcSegment(y, 0, row);
    tcSegment(y, 1, row);
    tcSegment(y, 2, row); 
    tcSegment(y, 3, row);
    tcSegment(y, 4, row);
    tcSegment(y, 5, row);
  } else if (digit == 1) {
    tcSegment(y, 1, row);
    tcSegment(y, 2, row);
  } else if (digit == 2) {
    tcSegment(y, 0, row);
    tcSegment(y, 1, row);
    tcSegment(y, 3, row);
    tcSegment(y, 4, row);
    tcSegment(y, 6, row);
  } else if (digit == 3) {
    tcSegment(y, 0, row);
    tcSegment(y, 1, row);
    tcSegment(y, 2, row); 
    tcSegment(y, 3, row);
    tcSegment(y, 6, row);
  } else if (digit == 4) {
    tcSegment(y, 1, row);
    tcSegment(y, 2, row); 
    tcSegment(y, 5, row);
    tcSegment(y, 6, row);
  } else if (digit == 5) {
    tcSegment(y, 0, row);
    tcSegment(y, 2, row); 
    tcSegment(y, 3, row);
    tcSegment(y, 5, row);
    tcSegment(y, 6, row);
  } else if (digit == 6) {
    tcSegment(y, 2, row); 
    tcSegment(y, 3, row);
    tcSegment(y, 4, row);
    tcSegment(y, 5, row);
    tcSegment(y, 6, row);
  } else if (digit == 7) {
    tcSegment(y, 0, row);
    tcSegment(y, 1, row);
    tcSegment(y, 2, row); 
  } else if (digit == 8) {
    tcSegment(y, 0, row);
    tcSegment(y, 1, row);
    tcSegment(y, 2, row); 
    tcSegment(y, 3, row);
    tcSegment(y, 4, row);
    tcSegment(y, 5, row);
    tcSegment(y, 6, row);
  } else if (digit == 9) {
    tcSegment(y, 0, row);
    tcSegment(y, 1, row);
    tcSegment(y, 2, row); 
    tcSegment(y, 5, row);
    tcSegment(y, 6, row);

  } else if (digit == 10) {          // J
    tcSegment(y, 0, row);
    tcSegment(y, 1, row);
    tcSegment(y, 2, row); 
    tcSegment(y, 3, row); 
  } else if (digit == 11) {          // A
    tcSegment(y, 0, row);
    tcSegment(y, 1, row);
    tcSegment(y, 2, row); 
    tcSegment(y, 4, row); 
    tcSegment(y, 5, row); 
    tcSegment(y, 6, row);
  } else if (digit == 12) {          // N
    tcSegment(y, 1, row);
    tcSegment(y, 2, row); 
    tcSegment(y, 4, row); 
    tcSegment(y, 5, row); 
    tcSegment(y, 7, row); 
    tcSegment(y, 9, row);
  } else if (digit == 13) {          // F
    tcSegment(y, 0, row); 
    tcSegment(y, 4, row); 
    tcSegment(y, 5, row); 
    tcSegment(y, 6, row); 
  } else if (digit == 14) {          // E
    tcSegment(y, 0, row); 
    tcSegment(y, 3, row);  
    tcSegment(y, 4, row); 
    tcSegment(y, 5, row); 
    tcSegment(y, 6, row); 
  } else if (digit == 15) {          // B
    tcSegment(y, 0, row);
    tcSegment(y, 2, row); 
    tcSegment(y, 3, row);  
    tcSegment(y, 4, row); 
    tcSegment(y, 5, row); 
    tcSegment(y, 6, row); 
    tcSegment(y, 8, row); 
  } else if (digit == 16) {          // M
    tcSegment(y, 1, row);
    tcSegment(y, 2, row);   
    tcSegment(y, 4, row); 
    tcSegment(y, 5, row); 
    tcSegment(y, 7, row);  
    tcSegment(y, 8, row); 
  } else if (digit == 17) {          // R
    tcSegment(y, 0, row);
    tcSegment(y, 1, row);   
    tcSegment(y, 4, row); 
    tcSegment(y, 5, row); 
    tcSegment(y, 6, row);  
    tcSegment(y, 9, row); 
  } else if (digit == 18) {          // P
    tcSegment(y, 0, row);
    tcSegment(y, 1, row);   
    tcSegment(y, 4, row); 
    tcSegment(y, 5, row); 
    tcSegment(y, 6, row);  
  } else if (digit == 19) {          // Y
    tcSegment(y, 7, row);
    tcSegment(y, 8, row); 
    tcSegment(y, 12, row);
  } else if (digit == 20) {          // U 
    tcSegment(y, 1, row); 
    tcSegment(y, 2, row);
    tcSegment(y, 3, row);     
    tcSegment(y, 4, row); 
    tcSegment(y, 5, row); 
  } else if (digit == 21) {          // L
    tcSegment(y, 3, row);     
    tcSegment(y, 4, row); 
    tcSegment(y, 5, row); 
  } else if (digit == 22) {          // G
    tcSegment(y, 0, row);
    tcSegment(y, 2, row);
    tcSegment(y, 3, row);     
    tcSegment(y, 4, row); 
    tcSegment(y, 5, row); 
    tcSegment(y, 6, row);  
  } else if (digit == 23) {          // C
    tcSegment(y, 0, row);
    tcSegment(y, 3, row);     
    tcSegment(y, 4, row); 
    tcSegment(y, 5, row);   
  } else if (digit == 24) {          // T
    tcSegment(y, 0, row);
    tcSegment(y, 11, row); 
    tcSegment(y, 12, row);
  } else if (digit == 25) {          // V
    tcSegment(y, 4, row);
    tcSegment(y, 5, row); 
    tcSegment(y, 8, row);
    tcSegment(y, 10, row);     
  }   
}


// -----------------------------------------------------------------------------
// draw a single segment in a given row   a-g -> 0-6
// 
void tcSegment(byte x, byte segment, byte row) {
  uint16_t color = colTcDigits[row];
  byte yOffset = 57;
  byte h = 44;
   byte hlength = 9;
  byte vlength = 8;
  byte width = 3; 
  
  byte y = yOffset+row*h+9;    // baseline

  // horizontal segments
  if (segment == 0) {                       // a  top horiz
    //ttgo->tft->drawLine(color);
    ttgo->tft->fillRect(x+3, y+2 , hlength, width, color);  
  } else if (segment == 1) {                // b  right top
    ttgo->tft->fillRect(x+10, y+3 , width,vlength, color); 
  } else if (segment == 2) {                // c  right bot 
    ttgo->tft->fillRect(x+10, y+11 ,  width,vlength, color); 
  } else if (segment == 3) {                // d  bottom horiz
    ttgo->tft->fillRect(x+3, y+17 , hlength,width, color);  
  } else if (segment == 4) {                // e  left bottom
    ttgo->tft->fillRect(x+2, y+11 ,  width,vlength, color); 
  } else if (segment == 5) {                // f  left top
    ttgo->tft->fillRect(x+2, y+3 , width,vlength, color); 
  } else if (segment == 6) {                // g  middle horiz
    ttgo->tft->fillRect(x+3, y+9 , hlength,width, color);  
  
  // - add segments     
  } else if (segment == 7) {                //  top left diag
    //ttgo->tft->drawLine(x+4,y+4, x+6, y+11, color); 
    ttgo->tft->drawLine(x+5,y+4, x+7, y+11, color); 
    ttgo->tft->drawLine(x+4,y+5, x+6, y+12, color);
  } else if (segment == 8) {                //  top right diag
    //ttgo->tft->drawLine(x+11,y+4, x+8, y+11, color); 
    ttgo->tft->drawLine(x+10,y+4, x+7, y+11, color);
    ttgo->tft->drawLine(x+11,y+5, x+8, y+12, color);
    
  } else if (segment == 9) {                //  bot right diag
    ttgo->tft->drawLine(x+11,y+18, x+9, y+12, color); 
    ttgo->tft->drawLine(x+10,y+18, x+8, y+12, color); 
    ttgo->tft->drawLine(x+11,y+17, x+9, y+11, color);
  } else if (segment == 10) {               //  bot left diag 
    ttgo->tft->drawLine(x+4,y+18, x+7, y+12, color); 
    ttgo->tft->drawLine(x+5,y+18, x+8, y+12, color);
    ttgo->tft->drawLine(x+4,y+17, x+7, y+11, color);
  } else if (segment == 11) {               //  upper T 
    ttgo->tft->fillRect(x+6, y+3 , width, vlength, color); 
  } else if (segment == 12) {               // lower T
    ttgo->tft->fillRect(x+6, y+11, width, vlength, color);
  }
}



// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------
void twinPinesMall(uint8_t flag) {

 
   uint16_t col = color565(30,60,165);
   uint16_t colt = color565(240,240,100);
   static uint8_t prevMin; 

   if (flag == 1) {
      ttgo->tft->fillScreen(TFT_BLACK); 
      uint8_t r = random(2);
      
      ttgo->tft->fillRoundRect(5,  60, 230, 175, 18, color565(60,60,60));
      ttgo->tft->fillRoundRect(10,  65, 220, 165, 15, color565(190,190,160));
      ttgo->tft->fillRoundRect(20,  170, 200, 54, 10, TFT_BLACK);
   
      int font = 4; 
      ttgo->tft->setTextSize(1);
      ttgo->tft->setTextColor(TFT_BLACK);
      if (r == 0) ttgo->tft->drawString("TWIN PINES MALL", 17, 145, font); 
      else ttgo->tft->drawString("LONE PINE MALL", 22, 145, font); 
      
      uint8_t x = 17;
      uint8_t y = 122;
      uint8_t ytop = 15;
      uint16_t green = color565(8,130,16);
      ttgo->tft->fillTriangle(20,y, 100,y, 60,ytop, green);
      if (r == 0) ttgo->tft->fillTriangle(70,y, 150,y, 110,ytop, green);
      ttgo->tft->fillRect(50, y, 20, 20, green);
      if (r == 0) ttgo->tft->fillRect(100, y, 20, 20, green);
      // inner brighter triangle
      uint16_t green2 = color565(16,140,24);
      uint8_t f = 5;
      ttgo->tft->fillTriangle(20+f,y-f*0.7, 100-f,y-f*0.7, 60,ytop+2*f, green2);
      if (r == 0) ttgo->tft->fillTriangle(70+f,y-f*0.7, 150-f,y-f*0.7, 110,ytop+2*f, green2);
      uint16_t green3 = color565(16,150,32);
      f = 10;
      ttgo->tft->fillTriangle(20+f,y-f*0.7, 100-f,y-f*0.7, 60,ytop+2*f, green3);
      if (r == 0) ttgo->tft->fillTriangle(70+f,y-f*0.7, 150-f,y-f*0.7, 110,ytop+2*f, green3);
      
      prevMin = 99; 

   }


   uint8_t x = 36;
   uint8_t y = 178; 
   //uint8_t sec;
   uint8_t min; 
   uint8_t hou;

   RTC_Date tnow = ttgo->rtc->getDateTime();
   //sec = tnow.second; 
   min = tnow.minute;
  
   if (min != prevMin) {
      prevMin = min;
      hou = tnow.hour;
      // --- clear dislay
      for (uint8_t j=0; j<29; j++) {
         for (uint8_t i=0; i<7; i++) {
            ttgo->tft->fillCircle(x+6*j,y+6*i,2,color565(10,10,10) );
         }
      }

      // --- colon & AM/PM
      twinPinesMallDigit(10,8);               // colon
      if (hou < 12) twinPinesMallDigit(11,22);    // A 
      else twinPinesMallDigit(12,22);             // P
      twinPinesMallDigit(13,26);                  //  M

      // --- time alway in 12h format
      uint8_t h12 = hou;
      if (hou == 0) h12 = 12;
      else if (hou >12) h12 = hou - 12; 
      twinPinesMallDigit(min/10,10);
      twinPinesMallDigit(min%10,16);
      if (h12>9) twinPinesMallDigit(h12/10,0);
      twinPinesMallDigit(h12%10,2);
      
      //displayDate();  
      String monthStr[12]={ "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
      uint8_t dday = tnow.day;
      uint8_t mmonth = tnow.month;
   
      uint8_t xpos = 143;
      uint8_t yp = 36;
      uint8_t font = 4;
      uint16_t green3 = color565(16,150,32);
      ttgo->tft->setTextColor(green3);
      ttgo->tft->drawString(monthStr[mmonth-1], xpos, yp, font);
      if (dday > 9) ttgo->tft->drawString(String(dday), xpos+49, yp, font);
      else ttgo->tft->drawString(String(dday), xpos+58, yp, font);
   }
}

// --------------------------------------------------------------
// ----- draw digit in column
void twinPinesMallDigit(uint8_t digit, uint8_t col) {
   if (digit == 0) {
      twinPinesMallSegment(0, col);
      twinPinesMallSegment(1, col);
      twinPinesMallSegment(2, col);
      twinPinesMallSegment(3, col);
      twinPinesMallSegment(4, col);
      twinPinesMallSegment(5, col);
   } else if (digit == 1) { 
      twinPinesMallSegment(1, col);
      twinPinesMallSegment(2, col);
   } else if (digit == 2) { 
      twinPinesMallSegment(0, col);
      twinPinesMallSegment(1, col);
      twinPinesMallSegment(3, col);
      twinPinesMallSegment(4, col);
      twinPinesMallSegment(6, col);
   } else if (digit == 3) { 
      twinPinesMallSegment(0, col);
      twinPinesMallSegment(1, col);
      twinPinesMallSegment(2, col);
      twinPinesMallSegment(3, col);
      twinPinesMallSegment(6, col);
   } else if (digit == 4) { 
      twinPinesMallSegment(1, col);
      twinPinesMallSegment(2, col);
      twinPinesMallSegment(5, col);
      twinPinesMallSegment(6, col);
   } else if (digit == 5) { 
      twinPinesMallSegment(0, col);
      twinPinesMallSegment(2, col);
      twinPinesMallSegment(3, col);
      twinPinesMallSegment(5, col);
      twinPinesMallSegment(6, col);
   } else if (digit == 6) { 
      twinPinesMallSegment(2, col);
      twinPinesMallSegment(3, col);
      twinPinesMallSegment(4, col);
      twinPinesMallSegment(5, col);
      twinPinesMallSegment(6, col);
   } else if (digit == 7) { 
      twinPinesMallSegment(0, col);
      twinPinesMallSegment(1, col);
      twinPinesMallSegment(2, col);
    } else if (digit == 8) { 
      twinPinesMallSegment(0, col);
      twinPinesMallSegment(1, col);
      twinPinesMallSegment(2, col); 
      twinPinesMallSegment(3, col);
      twinPinesMallSegment(4, col);
      twinPinesMallSegment(5, col);
      twinPinesMallSegment(6, col);
    } else if (digit == 9) { 
      twinPinesMallSegment(0, col);
      twinPinesMallSegment(1, col);
      twinPinesMallSegment(2, col); 
      twinPinesMallSegment(5, col);
      twinPinesMallSegment(6, col);
    } else if (digit == 10) {          // colon
      twinPinesMallSegment(10, col); 
    } else if (digit == 11) {          // A
      twinPinesMallSegment(11, col);
    } else if (digit == 12) {          // P
      twinPinesMallSegment(12, col);
    } else if (digit == 13) {          // M
      twinPinesMallSegment(13, col);
   }
}

// --------------------------------------------------------------
// ----- draw segment in given column
void twinPinesMallSegment(uint8_t segment, uint8_t column) {
   uint16_t color = color565(220,200,100);
   uint8_t rad = 2;
   uint8_t spac = 6;
   uint8_t xmin = 36;
   uint8_t ytop = 178; 
   uint8_t x;

   x = xmin + column*6;
   // special case: hours "1" starts at the very left
   if (column == 0) x = x - 4*6;
   
   if (segment == 0) {                        // a  top horiz
      for (uint8_t i=0; i<5; i++) ttgo->tft->fillCircle(x+spac*i,ytop+spac*0,rad,color );
   } else if (segment == 1) {                // b  right top
      for (uint8_t i=0; i<4; i++) ttgo->tft->fillCircle(x+spac*4,ytop+spac*i,rad,color );
   } else if (segment == 2) {                // c  right bot 
      for (uint8_t i=0; i<4; i++) ttgo->tft->fillCircle(x+spac*4,ytop+spac*(i+3),rad,color );
   } else if (segment == 3) {                // d  bottom horiz
      for (uint8_t i=0; i<5; i++) ttgo->tft->fillCircle(x+spac*i,ytop+spac*6,rad,color );
   } else if (segment == 4) {                // e  left bottom
      for (uint8_t i=0; i<4; i++) ttgo->tft->fillCircle(x+spac*0,ytop+spac*(i+3),rad,color );
   } else if (segment == 5) {                // f  left top
      for (uint8_t i=0; i<4; i++) ttgo->tft->fillCircle(x+spac*0,ytop+spac*i,rad,color );
   } else if (segment == 6) {                // g  middle horiz
      for (uint8_t i=0; i<5; i++) ttgo->tft->fillCircle(x+spac*i,ytop+spac*3,rad,color ); 
    
   } else if (segment == 10) {                // colon
      ttgo->tft->fillCircle(x,ytop+6*2,rad,color );   
      ttgo->tft->fillCircle(x,ytop+6*4,rad,color );  
      
   } else if (segment == 11) {                // A
      for (uint8_t i=1; i<5; i++) ttgo->tft->fillCircle(x+spac*0,ytop+spac*i,rad,color );
      ttgo->tft->fillCircle(x+spac*1,ytop+spac*0,rad,color );
      ttgo->tft->fillCircle(x+spac*1,ytop+spac*2,rad,color );
      for (uint8_t i=1; i<5; i++) ttgo->tft->fillCircle(x+spac*2,ytop+spac*i,rad,color );
   } else if (segment == 12) {                // P
      for (uint8_t i=0; i<5; i++) ttgo->tft->fillCircle(x+spac*0,ytop+spac*i,rad,color );
      ttgo->tft->fillCircle(x+spac*1,ytop+spac*0,rad,color );
      ttgo->tft->fillCircle(x+spac*1,ytop+spac*2,rad,color );
      for (uint8_t i=0; i<3; i++) ttgo->tft->fillCircle(x+spac*2,ytop+spac*i,rad,color );
   } else if (segment == 13) {                // M 
      for (uint8_t i=0; i<5; i++) ttgo->tft->fillCircle(x+spac*0,ytop+spac*i,rad,color );
      ttgo->tft->fillCircle(x+spac*1,ytop+spac*1,rad,color );
      for (uint8_t i=0; i<5; i++) ttgo->tft->fillCircle(x+spac*2,ytop+spac*i,rad,color );
   }

}


// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------
