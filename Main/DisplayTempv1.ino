#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <RH_ASK.h>
#include <SPI.h>

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

RH_ASK driver;

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void setup(void) {
  Serial.begin(115200);
  if (!driver.init())
    Serial.println("init failed");

  Serial.println(F("TFT LCD test"));

#ifdef USE_Elegoo_SHIELD_PINOUT
  Serial.println(F("Using Elegoo 2.4\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Elegoo 2.4\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  tft.reset();

  uint16_t identifier = tft.readID();
  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  } else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else if(identifier == 0x0101) {     
    identifier = 0x9341;
    Serial.println(F("Found 0x9341 LCD driver"));
  } else if(identifier == 0x1111) {     
    identifier = 0x9328;
    Serial.println(F("Found 0x9328 LCD driver"));
  } else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Elegoo 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_Elegoo_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Elegoo_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    identifier = 0x9328;
  }

  tft.begin(identifier);
}

void loop(void) {
  uint8_t buf[32];
  uint8_t buflen = sizeof(buf);
 

  if (driver.recv(buf, &buflen)) // Non-blocking
  {
    tft.setCursor(0,0);
    buf[buflen] = '\0'; // Set the last character to null terminator
    Serial.print("Raw Received Message: ");
    Serial.println((char*)buf);
    tft.setRotation(0);
    tft.fillScreen(0x001F);
    tft.setRotation(1);

    // Parse and print individual values
    parseReceivedMessage((char*)buf);
  }
  tft.setRotation(1);
  delay(7000);
}


void parseReceivedMessage(const char* receivedMsg) {
  char* token = strtok(receivedMsg, ";");
  
  if (token != NULL) {
 token = strtok(NULL, ";");
    if (token != NULL) {
      float temperature = atof(token);
      displayFloat("Temperature", temperature," C" ,2);
    }

    token = strtok(NULL, ";");
    if (token != NULL) {
      float pressure = atof(token);
      displayFloat("Pressure: ", pressure," hPA", 2);
    }

    token = strtok(NULL, ";");
    if (token != NULL) {
      float altitude = atof(token);
      displayFloat("Altitude: ", altitude," m", 2);
    }

    token = strtok(NULL, ";");
    if (token != NULL) {
      float humidity = atof(token);
      displayFloat("Humidity: ", humidity," %", 2);
    }
  }
}

void displayText(const char* label, const String& value, uint8_t textSize) {
  tft.setTextColor(MAGENTA);
  tft.setTextSize(textSize);
  tft.println(label);
  tft.setTextSize(textSize + 1);
  tft.println(value);
  tft.println();
}

void displayFloat(const char* label, float value, const char* Unit,uint8_t precision) {
  tft.setTextColor(MAGENTA);
  
  tft.setTextSize(2);
  tft.println(label);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.print(value, precision);
  tft.print(Unit);
  tft.println();
  tft.println();
}
