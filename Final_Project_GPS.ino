#include <stdint.h>
#include <Adafruit_GFX.h>       // Helps draw on the screen
#include <MCUFRIEND_kbv.h>      // Helps control the TFT screen
#include <SD.h>                 // Helps read from SD card
#include <TouchScreen.h>

#define SD_CS 10                // SD card chip select pin
#define BUFFPIXEL 20           // Number of pixels to load at once

MCUFRIEND_kbv tft;
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


//------------------------------------------------------------------------------//------------------------------------------------------------------------------
// most mcufriend shields use these values and pins settings
uint16_t identifier;uint8_t Orientation = 1;
uint8_t YP = A1;uint8_t XM = A2;uint8_t YM = 7;uint8_t XP = 6;uint8_t SwapXY = 0;   // Touch pins
uint16_t TS_LEFT = 920;uint16_t TS_RT  = 150;uint16_t TS_TOP = 940;uint16_t TS_BOT = 120;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 350);TSPoint tp;
#define MINPRESSURE 20
#define MAXPRESSURE 1000
#define SWAP(a, b) {uint16_t tmp = a; a = b; b = tmp;}

uint16_t Point_x, Point_y;  //screen coordinates
boolean Touch      (void);
void initialize_LCD(void);
void bmpDraw(char *filename, int x, int y) ;
int mapFloat(float x, float in_min, float in_max, float out_min, float out_max);
void drawMyCircle(int x, int y);
// Run once at the beginning
void setup() {
  Serial.begin(9600);           // Start serial monitor
  initialize_LCD();
 
  tft.setRotation(1);           // Rotate screen (0-3)
  tft.fillScreen(BLACK);        // Clear screen

  if (!SD.begin(SD_CS)) {       // Start SD card
    Serial.println("SD card not found!");
    return;
  }
  Serial.println("SD card ready!");

  bmpDraw("05.bmp", 0, 0);      // Show image from SD card
}

    
void loop() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n'); // نقرأ السطر بالكامل
    int commaIndex = data.indexOf(',');
    if (commaIndex > 0) {
      float lat = data.substring(0, commaIndex).toFloat();
      float lon = data.substring(commaIndex + 1).toFloat();

    int y = mapFloat(lat, 3003.9324, 3003.7945, 0, tft.height());
    int x = mapFloat(lon, 3116.6129, 3116.8665, 0, tft.width());
    Serial.println(lat);Serial.println(lon);
    Serial.print("X");Serial.println(x);
    Serial.print("y");Serial.println(y);
    drawMyCircle(x, y);
      
    }
  }
  if(Touch()){
    Serial.print("POINT X");Serial.println(Point_x);
    Serial.print("POINT Y");Serial.println(Point_y);
    if( ((Point_x>=238)&&(Point_x<=258))&&((Point_y>=134)&&(Point_y<=154)) ) { // Hall A and B
      bmpDraw("02.bmp", 0, 0);
    } else if( ((Point_x>=185)&&(Point_x<=220)&&(Point_y>=210)&&(Point_y<=230)) ) { // Luban Workshop
      bmpDraw("04.bmp", 0, 0);
    } else if( ((Point_x>=80)&&(Point_x<=90)&&(Point_y>=210)&&(Point_y<=236)) ) {
      bmpDraw("01.bmp", 0, 0);
    } 
    else if( ((Point_x>=225)&&(Point_x<=250)&&(Point_y>=1)&&(Point_y<=21)) ) {
      bmpDraw("03.bmp", 0, 0);
    } 
    else if( ((Point_x>=120)&&(Point_x<=155)&&(Point_y>=140)&&(Point_y<=160)) ) {
      bmpDraw("06.bmp", 0, 0);
    } 
    delay(500);bmpDraw("05.bmp", 0, 0); 
  }
 
}

int mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

boolean Touch(void)
{ boolean  value=false;
  tp = ts.getPoint();delay(1);pinMode(XM, OUTPUT);pinMode(YP, OUTPUT);delay(1);
  if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE)
    {  if (SwapXY == 1)
              {Point_x = map(tp.x, TS_LEFT, TS_RT, 0, tft.width() );}
       else
              {Point_x = map(tp.x, TS_LEFT, TS_RT, tft.width(), 0 );}
              {Point_y = map(tp.y, TS_TOP, TS_BOT, tft.height(),0 );}
       return(true);}
  else
    {Point_x=0;Point_y=0;return(false);}  }
//------------------------------------------------------------------------------//------------------------------------------------------------------------------
void initialize_LCD() 
//   tft_No=2;  //--> tft.begin(0x7783);
//   tft_No=4;  //--> tft.begin(0x9320);
{ String name;uint16_t tmp;char tft_No=2;
  unsigned int ID[16]={0x9341, 0x0154, 0x7783, 0x7789, 0x9320, 0x9325, 0x9325, 0x9327, 0x9329, 0x9341, 0, 0x9341, 0x9486, 0x9488, 0xB509};

  tft_No=2; name = "ST7781" ; SwapXY = 1;
  
//tft_No=4; name = "ILI9320";  

  TS_LEFT = 935; TS_RT = 190; TS_TOP = 910; TS_BOT = 195;

  switch (Orientation) {      // adjust for different aspects
        case 0:   break;        //no change,  calibrated for PORTRAIT
        case 1:   tmp = TS_LEFT, TS_LEFT = TS_BOT, TS_BOT = TS_RT, TS_RT = TS_TOP, TS_TOP = tmp;  break;
        case 2:   SWAP(TS_LEFT, TS_RT);  SWAP(TS_TOP, TS_BOT); break;
        case 3:   tmp = TS_LEFT, TS_LEFT = TS_TOP, TS_TOP = TS_RT, TS_RT = TS_BOT, TS_BOT = tmp;  break;  }

    ts = TouchScreen(XP, YP, XM, YM, 300);
    tft.reset();tft.begin(ID[tft_No]);
    tft.setRotation(Orientation);
    tft.fillScreen(BLACK);
}
//----------------------------------------------Print Time

void drawMyCircle(int x, int y) {
  
  tft.fillCircle(x, y, 5, RED); // ترسمي دائرة فارغة

}
void bmpDraw(char *filename, int x, int y) {
  File bmpFile;
  int bmpWidth, bmpHeight;
  uint8_t bmpDepth;
  uint32_t bmpImageoffset, rowSize;
  uint8_t sdbuffer[3 * BUFFPIXEL];
  uint16_t lcdbuffer[BUFFPIXEL];
  uint8_t buffidx = sizeof(sdbuffer), lcdidx = 0;
  boolean goodBmp = false, flip = true, first = true;

  if ((x >= tft.width()) || (y >= tft.height())) return;

  bmpFile = SD.open(filename);  // Try to open file
  if (!bmpFile) {
    Serial.println("File not found");
    return;
  }

  if (read16(bmpFile) == 0x4D42) {  // Check BMP signature "BM"
    read32(bmpFile);               // Skip file size
    read32(bmpFile);               // Skip reserved
    bmpImageoffset = read32(bmpFile); // Start of image
    read32(bmpFile);               // Skip header size
    bmpWidth = read32(bmpFile);
    bmpHeight = read32(bmpFile);

    if (read16(bmpFile) == 1) {    // Number of planes = 1
      bmpDepth = read16(bmpFile); // Should be 24-bit
      if (bmpDepth == 24 && read32(bmpFile) == 0) {
        goodBmp = true;
        rowSize = (bmpWidth * 3 + 3) & ~3; // Each row is padded

        if (bmpHeight < 0) { // If image is top to bottom
          bmpHeight = -bmpHeight;
          flip = false;
        }

        // Limit image size to screen size
        if ((x + bmpWidth) > tft.width()) x = 0;
        if ((y + bmpHeight) > tft.height()) y = 0;

        tft.setAddrWindow(x, y, x + bmpWidth - 1, y + bmpHeight - 1);

        for (int row = 0; row < bmpHeight; row++) {
          uint32_t pos = bmpImageoffset + (flip ? (bmpHeight - 1 - row) : row) * rowSize;
          if (bmpFile.position() != pos) {
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer);
          }

          for (int col = 0; col < bmpWidth; col++) {
            if (buffidx >= sizeof(sdbuffer)) {
              if (lcdidx > 0) {
                tft.pushColors(lcdbuffer, lcdidx, first);
                first = false;
                lcdidx = 0;
              }
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0;
            }

            // Read pixel (Blue, Green, Red)
            uint8_t b = sdbuffer[buffidx++];
            uint8_t g = sdbuffer[buffidx++];
            uint8_t r = sdbuffer[buffidx++];
            lcdbuffer[lcdidx++] = tft.color565(r, g, b); // Convert to TFT color
          }
        }

        // Draw remaining pixels
        if (lcdidx > 0) {
          tft.pushColors(lcdbuffer, lcdidx, first);
        }
      }
    }
  }

  bmpFile.close();
  if (!goodBmp) Serial.println("Not a valid BMP file");
}
uint16_t read16(File f) {
  return f.read() | (f.read() << 8); // Little-endian
}

uint32_t read32(File f) {
  return f.read() | (f.read() << 8) | (f.read() << 16) | (f.read() << 24);
}





