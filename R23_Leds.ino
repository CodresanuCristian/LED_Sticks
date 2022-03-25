#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include <FastLED.h>


#define MAX_LEDS 120
#define MAX_STRIPS 9
#define STRIP1  8
#define STRIP2 14
#define STRIP3 17
#define STRIP4 20
#define STRIP5 24
#define STRIP6 29
#define STRIP7 39
// Teensy 4.1 pinout strips:    8, 14, 17, 20, 24, 29, 39


CRGB strip1[MAX_LEDS];
CRGB strip2[MAX_LEDS];
CRGB strip3[MAX_LEDS];
CRGB strip4[MAX_LEDS];
CRGB strip5[MAX_LEDS];
CRGB strip6[MAX_LEDS*2];
CRGB strip7[MAX_LEDS*2];


void Txt(void);
void ShowStrips(void);
void ClearAllStrips(void);
void ClearStrip(byte strip);
void AllStripsSetBrightness(byte bright);
void FillAllStrips(byte R, byte G, byte B);
void FillStrip(byte strip, byte R, byte G, byte B); 



String receiveOption;
int receiveBright, receiveColorType, receiveRH, receiveGS, receiveBV;
int receiveStrips[MAX_STRIPS];
uint8_t paletteIndex = 0;
uint8_t colorIndex[MAX_LEDS];
uint16_t brightnessScale = 150, indexScale = 100;
int hue = 0;
uint8_t whichPalette = 0;




//================ GRADIENT PALETTES ========================================
DEFINE_GRADIENT_PALETTE(christmas_candy_gp) {
    0, 255,255,255,
   25, 255,  0,  0,
   51, 255,255,255,
   76,   0, 55,  0,
  102, 255,255,255,
  127, 255,  0,  0,
  153, 255,255,255,
  178,   0, 55,  0,
  204, 255,255,255,
  229, 255,  0,  0,
  255, 255,255,255};



DEFINE_GRADIENT_PALETTE(wiki_ice_greenland_gp) {
    0, 234,250,252,
   79, 234,250,252,
   79, 222,239,237,
  118, 222,239,237,
  118, 192,223,223,
  158, 192,223,223,
  158, 167,205,203,
  198, 167,205,203,
  198, 148,193,192,
  238, 148,193,192,
  238, 126,182,182,
  255, 126,182,182
};


DEFINE_GRADIENT_PALETTE(srtm_gp) {
    0,   0,  0,  0,
  134,   0,  0,  1,
  137,   0,  0,  1,
  138,   0,  0, 10,
  140,   0,  0, 47,
  140,   0,  0,147,
  141,  22, 55,255,
  141,   5, 80, 27,
  142,  33,139, 20,
  143, 194,203, 45,
  147, 139, 88, 11,
  154, 161,128, 23,
  166, 110, 84, 24,
  179, 173,184,176,
  205, 242,244,242,
  255, 255,255,255};


DEFINE_GRADIENT_PALETTE(Caribbean_gp) {
    0,   0,  0, 43,
   24,   0,  5, 85,
   36,   0, 10,106,
   48,   1, 22,130,
   60,   1, 32,147,
   72,   1, 57,138,
   84,   1, 79,138,
   97,   1, 91,147,
  109,   3,104,156,
  121,  12,104,166,
  133,  17,118,176,
  133,  17,118,138,
  145,  35,118,176,
  157,  64,118,176,
  170,  82,133,156,
  182,  88,149,186,
  194,  95,175,207,
  206, 110,184,230,
  213, 173,203,242,
  218, 206,213,242,
  218,  33, 53, 14,
  219,  44, 62, 14,
  219,  46, 62, 14,
  221,  58, 68, 18,
  223,  75, 73, 24,
  225,  95, 79, 27,
  228, 110, 91, 27,
  230, 126, 97, 31,
  233, 144,111, 27,
  235, 163,118, 27,
  237, 184,125, 26,
  237, 184,125, 27,
  240, 206,141, 25,
  242, 229,149, 25,
  245, 242,166, 24,
  255, 242,223,197
};


DEFINE_GRADIENT_PALETTE(purplish_blue_gp) {
    0,   1,  1,  1,
   37,   1,  3,  9,
   74,   6, 14, 51,
   89,  10, 21, 73,
  104,  16, 30,100,
  113,  20, 36,115,
  122,  24, 45,130,
  150,  53, 68,166,
  178,  97, 95,207,
  205, 142,144,230,
  232, 199,205,255,
  243, 227,229,255,
  255, 255,255,255
};


DEFINE_GRADIENT_PALETTE(pink_gp) {
    0,   1,  1,  1,
   45,   3,  3,  2,
   82,  21, 15, 12,
  119,  75, 31, 29,
  165, 175, 53, 54,
  214, 213,133,133,
  255, 255,255,255
};


DEFINE_GRADIENT_PALETTE(pink_ribbon_gp) {
    0,   0,  0,  0,
   36,   6,  2,  1,
   72,  41, 10,  3,
   80,  51, 13,  6,
   88,  64, 15, 10,
  113, 106, 29, 23,
  138, 163, 45, 44,
  155, 177, 66, 49,
  171, 192, 88, 53,
  182, 184,109, 61,
  194, 175,135, 68,
  224, 213,189,144,
  255, 255,255,255
};


DEFINE_GRADIENT_PALETTE(rain_gp) {
    0,   0,  0,  0,
   11,   1,  1,  2,
   23,   1,  1, 16,
   42,   1,  3, 27,
   62,   5,  8, 40,
   93,   8, 32, 47,
  125,  12, 75, 54,
  190,  84,151,132,
  255, 255,255,255
};


DEFINE_GRADIENT_PALETTE(purple_ice_gp) {
    0,   1,  0,  1,
    2,   1,  0,  1,
    4,   1,  1,  1,
    7,   1,  1,  1,
    9,   1,  1,  1,
   12,   1,  1,  1,
   14,   1,  1,  1,
   17,   1,  1,  1,
   19,   1,  1,  2,
   22,   1,  1,  2,
   24,   1,  1,  3,
   27,   1,  1,  4,
   29,   1,  1,  5,
   31,   1,  1,  6,
   34,   1,  1,  7,
   36,   1,  1,  9,
   39,   1,  2, 10,
   41,   2,  3, 12,
   44,   2,  3, 14,
   46,   2,  4, 16,
   49,   3,  4, 18,
   51,   4,  5, 21,
   54,   4,  5, 23,
   57,   5,  6, 25,
   59,   6,  6, 29,
   62,   7,  7, 31,
   64,   8,  7, 34,
   67,  10,  8, 38,
   69,  11,  8, 42,
   72,  13,  9, 45,
   74,  15,  9, 50,
   77,  17, 10, 53,
   79,  19, 10, 57,
   82,  21, 11, 63,
   84,  24, 12, 67,
  153,  45, 87,142,
  222,  73,244,255,
  238, 148,248,255,
  255, 255,255,255};
  

DEFINE_GRADIENT_PALETTE(lava_gp) {
    0,   0,  0,  0,
   46,  18,  0,  0,
   96, 113,  0,  0,
  108, 142,  3,  1,
  119, 175, 17,  1,
  146, 213, 44,  2,
  174, 255, 82,  4,
  188, 255,115,  4,
  202, 255,156,  4,
  218, 255,203,  4,
  234, 255,255,  4,
  244, 255,255, 71,
  255, 255,255,255
};


DEFINE_GRADIENT_PALETTE(frosted_ice_gp) {
    0,   1,  1,  1,
    2,   1,  1,  1,
    5,   1,  1,  1,
    7,   1,  1,  1,
   10,   1,  1,  1,
   12,   1,  1,  1,
   15,   1,  1,  1,
   17,   1,  1,  2,
   20,   1,  1,  3,
   22,   1,  2,  4,
   25,   1,  2,  5,
   27,   1,  3,  7,
   30,   1,  3,  9,
   32,   1,  4, 10,
   35,   1,  4, 12,
   37,   2,  5, 14,
   40,   2,  6, 17,
   42,   2,  6, 19,
   45,   3,  7, 22,
   47,   3,  8, 26,
   50,   3,  9, 29,
   52,   4,  9, 33,
   55,   4, 10, 37,
   57,   5, 11, 41,
   60,   5, 12, 45,
   63,   5, 13, 50,
   65,   6, 13, 54,
   68,   6, 14, 61,
   70,   7, 15, 66,
   73,   7, 16, 72,
   75,   7, 17, 78,
   78,   8, 17, 84,
   80,   8, 18, 91,
   83,   9, 19, 98,
   85,   9, 20,106,
   88,   9, 21,114,
   33,  20, 61,192,
  135,  21, 66,194,
  138,  21, 70,194,
  140,  22, 73,197,
  143,  23, 78,199,
  145,  24, 82,201,
  148,  26, 86,201,
  150,  26, 91,203,
  153,  27, 96,205,
  155,  28,100,205,
  158,  30,105,207,
  160,  31,109,210,
  163,  32,115,210,
  165,  33,119,212,
  168,  35,124,214,
  170,  36,130,214,
  173,  37,135,216,
  175,  39,139,219,
  178,  40,146,219,
  180,  41,151,221,
  183,  43,156,223,
  185,  45,161,223,
  188,  46,166,226,
  190,  47,171,228,
  193,  49,176,228,
  195,  51,182,230,
  198,  53,187,233,
  200,  54,193,235,
  203,  56,199,235,
  205,  58,205,237,
  208,  60,211,240,
  210,  63,215,240,
  213,  64,221,242,
  215,  66,227,245,
  218,  68,231,245,
  220,  71,237,247,
  223,  75,237,247,
  225,  79,239,247,
  228,  84,239,250,
  230,  88,239,250,
  232,  94,241,250,
  235,  98,241,250,
  237, 104,241,250,
  240, 109,244,252,
  242, 115,244,252,
  245, 121,244,252,
  247, 126,246,252,
  250, 133,246,255,
  252, 139,246,255,
  255, 146,248,255
};


DEFINE_GRADIENT_PALETTE(fire_gp) {
    0,   1,  1,  0,
   76,  32,  5,  0,
  146, 192, 24,  0,
  197, 220,105,  5,
  240, 252,255, 31,
  250, 252,255,111,
  255, 255,255,255
};


DEFINE_GRADIENT_PALETTE(sticky_mud_gp) {
    0,   0,  0,  0,
    2,   1,  1,  1,
    4,   1,  1,  1,
    7,   1,  1,  1,
    9,   1,  1,  1,
   12,   1,  1,  1,
   15,   1,  1,  1,
   17,   1,  1,  1,
   20,   1,  1,  1,
   22,   1,  1,  1,
   25,   1,  1,  1,
   27,   2,  1,  1,
   30,   2,  1,  1,
   32,   3,  1,  1,
   35,   4,  1,  1,
   37,   5,  2,  1,
   40,   6,  2,  1,
   42,   7,  2,  1,
   45,   8,  3,  1,
   47,  10,  3,  1,
   50,  11,  4,  1,
   52,  12,  4,  1,
   55,  14,  5,  1,
   57,  16,  5,  1,
   60,  18,  5,  1,
   62,  20,  6,  1,
   65,  22,  7,  1,
   67,  25,  7,  1,
   70,  27,  8,  1,
   72,  30,  9,  2,
   75,  32,  9,  2,
   77,  35, 10,  2,
   79,  39, 11,  2,
   82,  42, 12,  2,
   84,  45, 13,  3,
   87,  48, 13,  3,
   31, 135, 35,  9,
  133, 139, 37, 10,
  136, 144, 40, 10,
  138, 148, 42, 11,
  141, 153, 44, 11,
  143, 157, 46, 12,
  146, 163, 49, 13,
  148, 167, 52, 13,
  151, 173, 54, 14,
  153, 177, 56, 14,
  156, 184, 59, 15,
  158, 188, 62, 16,
  161, 194, 65, 16,
  164, 199, 68, 17,
  166, 201, 71, 20,
  169, 201, 74, 22,
  171, 203, 79, 25,
  174, 206, 82, 29,
  176, 206, 86, 31,
  179, 208, 90, 35,
  181, 208, 93, 38,
  184, 210, 99, 42,
  186, 213,103, 47,
  189, 213,107, 51,
  191, 215,111, 55,
  194, 217,115, 61,
  196, 217,121, 65,
  199, 220,125, 71,
  201, 222,130, 77,
  204, 222,135, 82,
  206, 224,139, 88,
  209, 227,146, 95,
  212, 227,151,100,
  214, 229,156,108,
  217, 229,161,114,
  219, 232,166,122,
  222, 234,171,130,
  224, 234,178,137,
  227, 237,184,145,
  229, 239,189,155,
  232, 239,195,162,
  234, 242,201,172,
  237, 244,209,182,
  239, 244,215,190,
  242, 247,221,201,
  244, 247,227,210,
  247, 249,233,221,
  249, 252,241,233,
  252, 252,248,242,
  255, 255,255,255
};


DEFINE_GRADIENT_PALETTE(haline_gp) {
    0,   2,  1, 29,
    0,   2,  1, 29,
    0,   2,  1, 31,
    1,   2,  1, 31,
    1,   2,  1, 33,
    2,   2,  1, 33,
    2,   2,  1, 34,
    3,   2,  1, 34,
    3,   2,  1, 36,
    4,   2,  1, 36,
    4,   2,  1, 38,
    5,   2,  1, 38,
    5,   2,  1, 40,
    6,   2,  1, 40,
    6,   2,  1, 42,
    7,   2,  1, 42,
    7,   2,  1, 44,
    8,   2,  1, 44,
    8,   2,  1, 47,
    9,   2,  1, 47,
    9,   2,  1, 49,
   10,   2,  1, 49,
   10,   2,  1, 52,
   11,   2,  1, 52,
   11,   2,  1, 53,
   12,   2,  1, 53,
   12,   2,  1, 56,
   13,   2,  1, 56,
   13,   2,  1, 59,
   14,   2,  1, 59,
   14,   2,  2, 62,
   15,   2,  2, 62,
   15,   2,  2, 64,
   16,   2,  2, 64,
   16,   2,  2, 66,
   17,   2,  2, 66,
   17,   2,  2, 6,
   30, 153,193, 31,
  231, 153,193, 31,
  231, 157,193, 32,
  232, 157,193, 32,
  232, 161,195, 34,
  233, 161,195, 34,
  233, 165,195, 34,
  234, 165,195, 34,
  234, 169,197, 36,
  235, 169,197, 36,
  235, 173,197, 37,
  236, 173,197, 37,
  236, 177,199, 39,
  237, 177,199, 39,
  237, 182,201, 40,
  238, 182,201, 40,
  238, 184,201, 42,
  239, 184,201, 42,
  239, 188,203, 43,
  240, 188,203, 43,
  240, 192,203, 45,
  241, 192,203, 45,
  241, 197,205, 46,
  242, 197,205, 46,
  242, 201,205, 48,
  243, 201,205, 48,
  243, 206,207, 50,
  244, 206,207, 50,
  244, 208,209, 52,
  245, 208,209, 52,
  245, 213,209, 53,
  246, 213,209, 53,
  246, 217,211, 54,
  247, 217,211, 54,
  247, 222,211, 56,
  248, 222,211, 56,
  248, 224,213, 59,
  249, 224,213, 59,
  249, 229,215, 61,
  250, 229,215, 61,
  250, 234,215, 63,
  251, 234,215, 63,
  251, 237,217, 65,
  252, 237,217, 65,
  252, 242,219, 67,
  253, 242,219, 67,
  253, 247,219, 69,
  254, 247,219, 69,
  254, 249,221, 72,
  255, 249,221, 72
};


DEFINE_GRADIENT_PALETTE(aqua_gp){
    0,   0, 255, 245,   
   46,   0,  21, 255,   
  179,  12, 250, 100,   
  255,   0, 255, 245    
};


DEFINE_GRADIENT_PALETTE(ocean_gp ){
    0,   0,  0,  0,
   31,   0,  1,  1,
   63,   0,  1,  4,
   95,   0, 19, 42,
  127,   0, 79,138,
  159,  15,144,112,
  191,  91,233, 89,
  223, 155,244,158,
  255, 242,255,255
};


DEFINE_GRADIENT_PALETTE(bhv1_gp){
    0,   0,  0,  0,
   39,   7, 55,  8,
   99,  42,255, 45,
  153,   7, 55,  8,
  255,   0,  0,  0
};



CRGBPalette16 EmeraldGreenPalette = bhv1_gp;
CRGBPalette16 OceanPalette = ocean_gp;
CRGBPalette16 AquaPalette = aqua_gp;
CRGBPalette16 HalinePalette = haline_gp;
CRGBPalette16 StickyMudPalette = sticky_mud_gp;
CRGBPalette16 FirePalette1 = fire_gp;
CRGBPalette16 FirePalette2 = HeatColors_p;
CRGBPalette16 FrostedIcePalette = frosted_ice_gp;
CRGBPalette16 LavaPalette = lava_gp;
CRGBPalette16 PurpleIcePalette = purple_ice_gp;
CRGBPalette16 RainPalette = rain_gp;
CRGBPalette16 PinkPalette2 = pink_ribbon_gp;
CRGBPalette16 PinkPalette1 = pink_gp;
CRGBPalette16 PurplishBluePalette = purplish_blue_gp;
CRGBPalette16 CaribbeanPalette = Caribbean_gp;
CRGBPalette16 SilverGoldPalette = srtm_gp;
CRGBPalette16 GreenlandIcePalette = wiki_ice_greenland_gp;
CRGBPalette16 XmasCandyPalette = christmas_candy_gp;

CRGBPalette16 currentPalette(EmeraldGreenPalette);
CRGBPalette16 targetPalette(XmasCandyPalette);

//================= SETUP ==============================================================
void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);

  FastLED.addLeds<WS2812SERIAL, STRIP1, BRG>(strip1, MAX_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812SERIAL, STRIP2, BRG>(strip2, MAX_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812SERIAL, STRIP3, BRG>(strip3, MAX_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812SERIAL, STRIP4, BRG>(strip4, MAX_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812SERIAL, STRIP5, BRG>(strip5, MAX_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812SERIAL, STRIP6, BRG>(strip6, MAX_LEDS*2).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812SERIAL, STRIP7, BRG>(strip7, MAX_LEDS*2).setCorrection(TypicalLEDStrip);

  for(int i=0; i<MAX_LEDS; i++)
    colorIndex[i] = random8();
}




//===================LOOP==============================================================
void loop() {
  if (Serial1.available() > 0){
      String btRead = Serial1.readString();
      
      for (int i=0; i<MAX_STRIPS; i++)
        receiveStrips[i] = btRead.substring(i, i+1).toInt();
      receiveOption = btRead.substring(9, 16);
      receiveBright = btRead.substring(16, 19).toInt();
      receiveColorType = btRead.substring(19, 20).toInt();
      receiveRH = btRead.substring(20, 23). toInt();
      receiveGS = btRead.substring(23, 26). toInt();
      receiveBV = btRead.substring(26, 29). toInt();


      if (receiveOption == "turn_on")
        for (int i=0; i<MAX_STRIPS; i++)
          if (receiveStrips[i] == 1){
            AllStripsSetBrightness(receiveBright);
            FillStrip(i+1, receiveRH, receiveGS, receiveBV);
          }

      if (receiveOption == "turn_of")
        for (int i=0; i<MAX_STRIPS; i++)
          if (receiveStrips[i] == 1)
            ClearStrip(i+1);

      if (receiveOption == "aplycol")
        for (int i=0; i<MAX_STRIPS; i++)
          if (receiveStrips[i] == 1)
            FillStrip(i+1, receiveRH, receiveGS, receiveBV);

      if (receiveOption == "aplybri")
        for (int i=0; i<MAX_STRIPS; i++)
          if (receiveStrips[i] == 1)
            AllStripsSetBrightness(receiveBright);
      
      Txt();
  }
}





//============================== MY FUNCTIONS ============================
void Txt(void){
  Serial.println();Serial.println();
    Serial.print("Am selectat bachetele: ");
    for (int i=0; i<MAX_STRIPS; i++){
      if (receiveStrips[i] == 1)
        Serial.print(i+1);
      Serial.print(" ");
    }
    Serial.println();
    Serial.print("Am selectat optiunea: ");
    Serial.println(receiveOption);
    Serial.print("Luminozitate: ");
    Serial.println(receiveBright);
    Serial.print("Tipul de culoare: ");
    if (receiveColorType == 0){
      Serial.println("RGB");
      Serial.print("R: ");
      Serial.println(receiveRH);
      Serial.print("G: ");
      Serial.println(receiveGS);
      Serial.print("B: ");
      Serial.println(receiveBV);
    }
    else{
      Serial.println("HSV");
      Serial.print("H: ");
      Serial.println(receiveRH);
      Serial.print("S: ");
      Serial.println(receiveGS);
      Serial.print("V: ");
      Serial.println(receiveBV);
    }
}


//=========================================================================
void ShowStrips(void){
  FastLED.show();
}


//==========================================================================
void ClearAllStrips(void){
  
  fill_solid(strip1, MAX_LEDS, CRGB::Black);
  fill_solid(strip2, MAX_LEDS, CRGB::Black);
  fill_solid(strip3, MAX_LEDS, CRGB::Black);
  fill_solid(strip4, MAX_LEDS, CRGB::Black);
  fill_solid(strip5, MAX_LEDS, CRGB::Black);
  fill_solid(strip6, MAX_LEDS, CRGB::Black);
  fill_solid(strip7, MAX_LEDS, CRGB::Black);

  ShowStrips();
}


//==========================================================================
void ClearStrip(byte strip){
  
  if (strip == 1) fill_solid(strip1, MAX_LEDS, CRGB::Black);
  if (strip == 2) fill_solid(strip2, MAX_LEDS, CRGB::Black);
  if (strip == 3) fill_solid(strip3, MAX_LEDS, CRGB::Black);
  if (strip == 4) fill_solid(strip4, MAX_LEDS, CRGB::Black);
  if (strip == 5) fill_solid(strip5, MAX_LEDS, CRGB::Black);
  if (strip == 6) fill_solid(strip6, MAX_LEDS, CRGB::Black);
  if (strip == 7) fill_solid(strip7, MAX_LEDS, CRGB::Black);

  ShowStrips();
}


//==========================================================================
void AllStripsSetBrightness(byte bright){
  
  FastLED.setBrightness(bright);

  ShowStrips();
}



//==========================================================================
void FillAllStrips(byte R, byte G, byte B){
  
  fill_solid(strip1, MAX_LEDS, CRGB(R, B, G));
  fill_solid(strip2, MAX_LEDS, CRGB(R, B, G));
  fill_solid(strip3, MAX_LEDS, CRGB(R, B, G));
  fill_solid(strip4, MAX_LEDS, CRGB(R, B, G));
  fill_solid(strip5, MAX_LEDS, CRGB(R, B, G));
  fill_solid(strip6, MAX_LEDS, CRGB(R, B, G));
  fill_solid(strip7, MAX_LEDS, CRGB(R, B, G));

  ShowStrips();
}


//==========================================================================
void FillStrip(byte strip, byte R, byte G, byte B){

  if (strip == 1) fill_solid(strip1, MAX_LEDS, CRGB(R, B, G));
  if (strip == 2) fill_solid(strip2, MAX_LEDS, CRGB(R, B, G));
  if (strip == 3) fill_solid(strip3, MAX_LEDS, CRGB(R, B, G));
  if (strip == 4) fill_solid(strip4, MAX_LEDS, CRGB(R, B, G));
  if (strip == 5) fill_solid(strip5, MAX_LEDS, CRGB(R, B, G));
  if (strip == 6) fill_solid(strip6, MAX_LEDS, CRGB(R, B, G));
  if (strip == 7) fill_solid(strip7, MAX_LEDS, CRGB(R, B, G));

  ShowStrips();
}


//==========================================================================

void Fire(const CRGBPalette16 palette, int bright, float fireSpeed){
  int t = millis();

  for (int i=0; i<MAX_LEDS-1; i++){
    uint8_t math = (i-(MAX_LEDS-1)) * 255 / (MAX_LEDS);
    uint8_t noise1 = inoise8(0,-i*55+t*fireSpeed);
    uint8_t noise2 = inoise8(0,-i*60+t*fireSpeed);
    uint8_t noise3 = inoise8(0,-i*65+t*fireSpeed);
    
    strip1[i] = ColorFromPalette(palette, qsub8(noise2, math), bright);  
    strip2[i] = ColorFromPalette(palette, qsub8(noise1, math), bright);
    strip3[i] = ColorFromPalette(palette, qsub8(noise3, math), bright);
  }

  FastLED.show();
}


void Lava(const CRGBPalette16 palette, int speedBright, int speedMove, uint16_t brightnessScale, uint16_t indexScale){
  
  for (int i=0; i<MAX_LEDS; i++){
    uint8_t bright1 = inoise8(i*brightnessScale, millis()/speedBright);
    uint8_t bright2 = inoise8(i*brightnessScale*2, millis()/speedBright);
    uint8_t bright3 = inoise8(i*brightnessScale*3, millis()/speedBright);
    
    uint8_t index1 = inoise8(i*indexScale, millis()/speedMove);
    uint8_t index2 = inoise8(i*indexScale*2, millis()/speedMove);
    uint8_t index3 = inoise8(i*indexScale*3, millis()/speedMove);
    
    strip1[i] = ColorFromPalette(palette, index1, bright1);
    strip2[i] = ColorFromPalette(palette, index2, bright2);
    strip3[i] = ColorFromPalette(palette, index3, bright3);
  }
  
  FastLED.show();
}


void Bouncing1(int speedBeat, int minBeat, int maxBeat, byte ballNumb, CRGB col1, CRGB col2, CRGB col3,
              CRGB col4, CRGB col5, byte blurTime){

  if (ballNumb >= 1){
    uint8_t sinBeat1 = beatsin8(speedBeat, minBeat, maxBeat, 0, 0);
    strip1[sinBeat1] = col1; 
    strip2[sinBeat1] = col1;
    strip3[sinBeat1] = col1;
  }
  if (ballNumb >= 2){
    uint8_t sinBeat2 = beatsin8(speedBeat, minBeat, maxBeat, 0, 50);
    strip1[sinBeat2] = col2; 
    strip2[sinBeat2] = col2;
    strip3[sinBeat2] = col2;
  }
  if (ballNumb >= 3){
    uint8_t sinBeat3 = beatsin8(speedBeat, minBeat, maxBeat, 0, 100);
    strip1[sinBeat3] = col3; 
    strip2[sinBeat3] = col3;
    strip3[sinBeat3] = col3;
  }
  if (ballNumb >= 4){
    uint8_t sinBeat4 = beatsin8(speedBeat, minBeat, maxBeat, 0, 150);
    strip1[sinBeat4] = col4; 
    strip2[sinBeat4] = col4;
    strip3[sinBeat4] = col4;
  }
  if (ballNumb >= 5){
    uint8_t sinBeat5 = beatsin8(speedBeat, minBeat, maxBeat, 0, 200);
    strip1[sinBeat5] = col5; 
    strip2[sinBeat5] = col5;
    strip3[sinBeat5] = col5;
  }

  EVERY_N_MILLISECONDS(0){ 
    for (int i=0; i<4; i++){
      blur1d(strip1, MAX_LEDS, 10);
      blur1d(strip2, MAX_LEDS, 10);
      blur1d(strip3, MAX_LEDS, 10);
    }
  }

  fadeToBlackBy(strip1, MAX_LEDS, 10);
  fadeToBlackBy(strip2, MAX_LEDS, 10);
  fadeToBlackBy(strip3, MAX_LEDS, 10);

  FastLED.show();
}



void Bouncing2(int speedBeat, int minBeat, int maxBeat, byte ballNumb, CRGB col1, CRGB col2, CRGB col3,
              CRGB col4, CRGB col5, byte blurTime){

  uint8_t sinBeat1 = beatsin8(speedBeat, minBeat, maxBeat, 0, 0);
  uint8_t sinBeat2 = beatsin8(speedBeat, minBeat, maxBeat, 0, 150);
  uint8_t sinBeat3 = beatsin8(speedBeat, minBeat, maxBeat, 0, 50);
  uint8_t sinBeat4 = beatsin8(speedBeat, minBeat, maxBeat, 0, 200);
  uint8_t sinBeat5 = beatsin8(speedBeat, minBeat, maxBeat, 0, 100);
  
  if (ballNumb >= 1){
    strip1[sinBeat1] = col1;
    strip2[sinBeat2] = col1;
    strip3[sinBeat3] = col1;
  }
  if (ballNumb >= 2){
    strip1[sinBeat4] = col2; 
    strip2[sinBeat5] = col2;
    strip3[sinBeat1] = col2;
  }
  if (ballNumb >= 3){
    strip1[sinBeat2] = col3; 
    strip2[sinBeat3] = col3;
    strip3[sinBeat4] = col3;
  }
  if (ballNumb >= 4){
    strip1[sinBeat5] = col4; 
    strip2[sinBeat1] = col4;
    strip3[sinBeat2] = col4;
  }
  if (ballNumb >= 5){
    strip1[sinBeat3] = col5; 
    strip2[sinBeat4] = col5;
    strip3[sinBeat5] = col5;
  }

  EVERY_N_MILLISECONDS(blurTime){ 
    for (int i=0; i<4; i++){
      blur1d(strip1, MAX_LEDS, 10);
      blur1d(strip2, MAX_LEDS, 10);
      blur1d(strip3, MAX_LEDS, 10);
    }
  }

  fadeToBlackBy(strip1, MAX_LEDS, 10);
  fadeToBlackBy(strip2, MAX_LEDS, 10);
  fadeToBlackBy(strip3, MAX_LEDS, 10);

  FastLED.show();
}




void Shooting(const CRGBPalette16 palette, byte speedShooting, byte minPos, byte maxPos, byte sizeLed, 
              byte palTimer, byte dirTimer, int dir1, int dir2, int dir3){
  
  uint8_t pos = map(beat8(speedShooting, 0), 0, 255, minPos, maxPos);

  
  strip1[abs(dir1-pos)] = ColorFromPalette(palette, paletteIndex, 255);
  strip2[abs(dir2-pos)] = ColorFromPalette(palette, paletteIndex, 255);
  strip3[abs(dir3-pos)] = ColorFromPalette(palette, paletteIndex, 255);

  fadeToBlackBy(strip1, MAX_LEDS, sizeLed);
  fadeToBlackBy(strip2, MAX_LEDS, sizeLed);
  fadeToBlackBy(strip3, MAX_LEDS, sizeLed);

  EVERY_N_MILLISECONDS(palTimer){
    paletteIndex++; 
  }

  FastLED.show();
}



void Shake(const CRGBPalette16 palette, byte speedUp, byte speedDown, byte fastMove1, byte fastMove2, byte fastMove3, 
           float len1, float len2, float len3, byte div1, byte div2, byte div3){
  uint8_t beatA = beatsin8(speedUp,   0, 255);
  uint8_t beatB = beatsin8(speedDown, 0, 255);

  fill_palette(strip1, MAX_LEDS, len1*(beatA+beatB) / fastMove1, div1 ,palette, 255, LINEARBLEND);
  fill_palette(strip2, MAX_LEDS, len2*(beatA+beatB) / fastMove2, div2 ,palette, 255, LINEARBLEND);
  fill_palette(strip3, MAX_LEDS, len3*(beatA+beatB) / fastMove3, div3 ,palette, 255, LINEARBLEND);

  FastLED.show();
}


void Circus(const CRGBPalette16 palette, byte spd1, byte spd2, byte spd3, byte spd4, byte d1, byte d2, byte d3, byte d4, byte d5, byte d6, byte d7, byte d8,
            byte d9, byte d10, byte d11, byte d12, byte d13, byte d14, byte d15, byte d16, int palTime,byte f1, byte f2, byte f3){
  uint8_t posBeat1 = beatsin8(spd1, d1, d2, d9, d10);
  uint8_t posBeat2 = beatsin8(spd2, d3, d4, d11, d12);
  uint8_t posBeat3 = beatsin8(spd3, d5, d6, d13, d14);
  uint8_t posBeat4 = beatsin8(spd4, d7, d8, d15, d16);
  
  uint8_t colBeat = beatsin8(45, 0, 255, 0, 0);
  
  strip1[(posBeat1 + posBeat2)/2] = ColorFromPalette(palette, paletteIndex, 255);//CHSV(colBeat, 255, 255);
  strip2[(posBeat1 + posBeat2)/2] = ColorFromPalette(palette, paletteIndex, 255);//CHSV(colBeat, 255, 255);
  strip3[(posBeat1 + posBeat2)/2] = ColorFromPalette(palette, paletteIndex, 255);//CHSV(colBeat, 255, 255);
  strip1[(posBeat3 + posBeat4)/2] = ColorFromPalette(palette, paletteIndex, 255);//CHSV(colBeat, 255, 255);
  strip2[(posBeat3 + posBeat4)/2] = ColorFromPalette(palette, paletteIndex, 255);//CHSV(colBeat, 255, 255);
  strip3[(posBeat3 + posBeat4)/2] = ColorFromPalette(palette, paletteIndex, 255);//CHSV(colBeat, 255, 255);

  fadeToBlackBy(strip1, MAX_LEDS, f1);
  fadeToBlackBy(strip2, MAX_LEDS, f2);
  fadeToBlackBy(strip3, MAX_LEDS, f3);

  EVERY_N_MILLISECONDS(palTime){
    paletteIndex++; 
  }
  
  FastLED.show();
}



void Blending(byte timeToChangePalette, boolean wave, int waveSpeed){
  for (int i=0; i<MAX_LEDS; i++){
    strip1[i] = ColorFromPalette(currentPalette, colorIndex[i]);
    strip2[i] = ColorFromPalette(currentPalette, colorIndex[i]);
    strip3[i] = ColorFromPalette(currentPalette, colorIndex[i]);
  }
  
  nblendPaletteTowardPalette(currentPalette, targetPalette, 10);

  switch(whichPalette){
    case  0:{targetPalette = EmeraldGreenPalette; break;}
    case  1:{targetPalette = OceanPalette;        break;}
    case  2:{targetPalette = AquaPalette;         break;}
    case  3:{targetPalette = HalinePalette;       break;}
    case  4:{targetPalette = StickyMudPalette;    break;}
    case  5:{targetPalette = FirePalette1;        break;}
    case  6:{targetPalette = FirePalette2;        break;}
    case  7:{targetPalette = FrostedIcePalette;   break;}
    case  8:{targetPalette = LavaPalette;         break;}
    case  9:{targetPalette = PurpleIcePalette;    break;}
    case 10:{targetPalette = RainPalette;         break;}
    case 11:{targetPalette = PinkPalette1;        break;}
    case 12:{targetPalette = PinkPalette2;        break;}
    case 13:{targetPalette = PurplishBluePalette; break;}
    case 14:{targetPalette = CaribbeanPalette;    break;}
    case 15:{targetPalette = SilverGoldPalette;   break;}
    case 16:{targetPalette = GreenlandIcePalette; break;}
    case 17:{targetPalette = XmasCandyPalette;    break;}
  }

  EVERY_N_SECONDS(timeToChangePalette){
    whichPalette++;
    if (whichPalette > 17)
      whichPalette = 0;
  }

  if (wave == true){
    EVERY_N_MILLISECONDS(waveSpeed){
      for (int i=0; i<MAX_LEDS; i++)
        colorIndex[i]++;
    }
  }

  FastLED.show();
}


void Mozaic(const CRGBPalette16 palette, int fast, int speedFade, byte ls1, byte ls2, byte ls3, byte lf1, byte lf2, byte lf3,
            byte rf1, byte rf2, byte rf3){
  EVERY_N_MILLISECONDS(fast){
    strip1[random8(ls1, lf1)] = ColorFromPalette(palette, random8(), 255, LINEARBLEND);
    strip2[random8(ls2, lf2)] = ColorFromPalette(palette, random8(), 255, LINEARBLEND);
    strip3[random8(ls3, lf3)] = ColorFromPalette(palette, random8(), 255, LINEARBLEND);
  }

  //fadeToBlackBy(vector, maximul intervalului, timp)
  fadeToBlackBy(strip1, rf1, speedFade);
  fadeToBlackBy(strip2, rf2, speedFade);
  fadeToBlackBy(strip3, rf3, speedFade);

  FastLED.show();
}


void Linear(const CRGBPalette16 palette, byte ledNumb, byte bright, byte paletteTime, byte way){

  // fill_palette(vector, nr.leduri, startul paletei, cat de mult sa acopere bagheta, culoarea paletei, luminozitatea, tipul: LINEARBLEND, NOBLEND);
  fill_palette(strip1, ledNumb, paletteIndex, 255/MAX_LEDS, palette, bright, LINEARBLEND);
  fill_palette(strip2, ledNumb, paletteIndex, 255/MAX_LEDS, palette, bright, LINEARBLEND);
  fill_palette(strip3, ledNumb, paletteIndex, 255/MAX_LEDS, palette, bright, LINEARBLEND);

  if (way == 0){
    EVERY_N_MILLISECONDS(paletteTime){
      paletteIndex++;
    }
  }else if (way == 1){
    EVERY_N_MILLISECONDS(paletteTime){
      paletteIndex--;
    }
  }
  
  FastLED.show(); 
}



void Snake(const CRGBPalette16 palette, byte maxLeds, byte minLeds, boolean clear1, boolean clear2, int time1, int time2, byte scale){
  for(int i = maxLeds; i >= minLeds; i--) {
    strip1[i] = ColorFromPalette(palette, colorIndex[i]);
    strip2[i] = ColorFromPalette(palette, colorIndex[i]);
    strip3[i] = ColorFromPalette(palette, colorIndex[i]);
    FastLED.show();

    if (clear1 == true){
     strip1[i] = CRGB::Black;
     strip2[i] = CRGB::Black;
     strip3[i] = CRGB::Black;
    }
  
    for(int i = maxLeds; i >= minLeds; i--){
      strip1[i].nscale8(scale);
      strip2[i].nscale8(scale);
      strip3[i].nscale8(scale);
    }
    delay(time1);
  }

  
  for(int i = 0; i < MAX_LEDS; i++) {
    strip1[i] = ColorFromPalette(palette, colorIndex[i]);
    strip2[i] = ColorFromPalette(palette, colorIndex[i]);
    strip3[i] = ColorFromPalette(palette, colorIndex[i]);
    FastLED.show();
    
    if (clear2 == true){
      strip1[i] = CRGB::Black;
      strip2[i] = CRGB::Black;
      strip3[i] = CRGB::Black;
    }
    for(int i = 0; i < MAX_LEDS; i++){
      strip1[i].nscale8(scale);
      strip2[i].nscale8(scale);
      strip3[i].nscale8(scale);
    }
    delay(time2);
  }
}
