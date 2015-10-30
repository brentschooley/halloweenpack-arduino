#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include "RGB.h"

#define PIN 1

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN,
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);
  
bool eyesOn = true;
bool isBat = true;
bool isGhost = false;
int cloud = A0;

void setup() {
  Serial.begin(9600);
  matrix.begin();
  matrix.setBrightness(30);

  crossFade(off, orange, 50, 5);
  delay(1000);
  drawBat();
}

void loop() {
  int mode = analogRead(cloud);
  Serial.println(mode);
  if(isGhost && mode > 200 && mode < 600) {
    crossFade(purple, orange, 50, 5);
    drawBat();
    isGhost = false;
    isBat = true;
    delay(2000);
    return;
  }
  else if (isBat && mode > 700) {
    crossFade(orange, purple, 50, 5);
    drawGhost();
    isGhost = true;
    isBat = false;
    delay(2000);
    return;
  }
  
  if(eyesOn) {
    delay(2000);
  }
  else {
    delay(400);
  }
  blinkEyes();
}

// Fill the pixels one after the other with a color
void colorWipe(RGB color, uint8_t wait) {
  for(uint16_t row=0; row < 8; row++) {
    for(uint16_t column=0; column < 8; column++) {
      matrix.drawPixel(column, row, matrix.Color(color.r, color.g, color.b));
      matrix.show();
      delay(wait);
    }
  }
}

// Fade pixel (x, y) from startColor to endColor
void fadePixel(int x, int y, RGB startColor, RGB endColor, int steps, int wait) {
  for(int i = 0; i <= steps; i++) 
  {
     int newR = startColor.r + (endColor.r - startColor.r) * i / steps;
     int newG = startColor.g + (endColor.g - startColor.g) * i / steps;
     int newB = startColor.b + (endColor.b - startColor.b) * i / steps;
     
     matrix.drawPixel(x, y, matrix.Color(newR, newG, newB));
     matrix.show();
     delay(wait);
  }
}

void setPixel(int x, int y, RGB color) {
  matrix.drawPixel(x, y, matrix.Color(color.r, color.g, color.b));
}

// Crossfade entire screen from startColor to endColor
void crossFade(RGB startColor, RGB endColor, int steps, int wait) {
  for(int i = 0; i <= steps; i++)
  {
     int newR = startColor.r + (endColor.r - startColor.r) * i / steps;
     int newG = startColor.g + (endColor.g - startColor.g) * i / steps;
     int newB = startColor.b + (endColor.b - startColor.b) * i / steps;
     
     matrix.fillScreen(matrix.Color(newR, newG, newB));
     matrix.show();
     delay(wait);
  }
}

void drawBat() {
  // This 8x8 array represents the LED matrix pixels. 
  // A value of 1 means we’ll fade the pixel to purple
  int bat[8][8] = {  
   {0, 0, 0, 1, 0, 1, 0, 0},
   {0, 1, 0, 1, 1, 1, 0, 1},
   {1, 1, 0, 1, 1, 1, 0, 1},
   {1, 1, 1, 1, 1, 1, 1, 1},
   {1, 1, 1, 1, 1, 1, 1, 1},
   {1, 1, 0, 1, 1, 0, 1, 1},
   {1, 0, 0, 1, 0, 0, 0, 1},
   {0, 0, 0, 0, 0, 0, 0, 0}
  };
   
  for(int row = 0; row < 8; row++) {
    for(int column = 0; column < 8; column++) {
     if(bat[row][column] == 1) {
       fadePixel(column, row, orange, purple, 30, 0);
     }
   }
  }
  
  fadePixel(3, 2, purple, yellow, 1, 0);
  fadePixel(5, 2, purple, yellow, 1, 0);
}

void drawGhost() {
  // This 8x8 array represents the LED matrix pixels. 
  // A value of 1 means we’ll fade the pixel to white
  int ghost[8][8] = {  
   {0, 0, 0, 1, 1, 1, 0, 0},
   {0, 0, 1, 1, 0, 1, 0, 0},
   {0, 0, 1, 1, 1, 1, 1, 0},
   {0, 1, 1, 1, 1, 0, 1, 1},
   {0, 0, 1, 1, 1, 0, 1, 0},
   {1, 0, 1, 1, 1, 1, 1, 0},
   {0, 1, 1, 1, 1, 1, 1, 0},
   {0, 0, 1, 1, 1, 1, 0, 0}
  };
   
  for(int row = 0; row < 8; row++) {
    for(int column = 0; column < 8; column++) {
     if(ghost[row][column] == 1) {
       fadePixel(column, row, purple, white, 30, 0);
     }
   }
  }
  
  fadePixel(4, 1, purple, yellow, 30, 0);
  fadePixel(6, 1, purple, yellow, 30, 0);

}

void blinkEyes() {
  //matrix.drawPixel(4, 1, matrix.Color(67, 32, 112));
  //matrix.drawPixel(6, 1, matrix.Color(67, 32, 112));
  if(eyesOn) {
    if(isGhost) {
      setPixel(4, 1, white);
      setPixel(6, 1, white);
    }
    else {
      setPixel(3, 2, purple);
      setPixel(5, 2, purple);
    }
    //matrix.drawPixel(x, y, matrix.Color(color.r, color.g, color.b));
    eyesOn = 0;
    
  }
  else
  {
    if(isGhost) {
      setPixel(4, 1, yellow);
      setPixel(6, 1, yellow);
    }
    else {
      setPixel(3, 2, yellow);
      setPixel(5, 2, yellow);
    }
    eyesOn = 1;
  }
  matrix.show();
}
