/*!
 * @file uart.ino
 * @brief  Control the voice recognition module via UART
 * @n  Get the recognized command ID and play the corresponding reply audio according to the ID;
 * @n  Set the wake-up state duration, set mute mode, set volume, enter the wake-up state, and reset the module
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence  The MIT License (MIT)
 * @author  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2022-12-30
 * @url  https://github.com/DFRobot/DFRobot_DF2301Q
 */
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>
#include "DFRobot_DF2301Q.h"

#define SCREEN_WIDTH    240
#define SCREEN_HEIGHT   320
#define SCREEN_ROTATION 1

int screen_line_ctr = 0;
int screen_max_lines = 13;

#define TFT_CS  14  //for D32 Pro
#define TFT_DC  27  //for D32 Pro
#define TFT_RST 33 //for D32 Pro
#define TS_CS   12  //for D32 Pro

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// The TIRQ interrupt signal used for this example.
// #define TIRQ_PIN  15
// XPT2046_Touchscreen ts(TS_CS, TIRQ_PIN);    // using IRQ pin... make sure to solder wire from TFT PCB to D32 board
// or if not using interrupts

XPT2046_Touchscreen ts(TS_CS); 

typedef struct{
  byte x;
  int16_t y;
  int16_t color = ILI9341_CYAN; // initial color
} TouchScreenPoint; 

TouchScreenPoint  currentPoint;
// TouchScreenPoint screenPoint[16000];

/**
 * @brief DFRobot_URM13_RTU constructor
 * @param serial - serial ports for communication, supporting hard and soft serial ports
 * @param rx - UART The pin for receiving data
 * @param tx - UART The pin for transmitting data
 */
#if (defined(ARDUINO_AVR_UNO) || defined(ESP8266))   // Use software serial
  SoftwareSerial softSerial(/*rx =*/4, /*tx =*/5);
  DFRobot_DF2301Q_UART DF2301Q(/*softSerial =*/&softSerial);
#elif defined(ESP32)   // Use the hardware serial with remappable pin: Serial1
  // for LOLIN, UART1 try rx=12, tx=14  // conflict with TFT/CS
  //            UART2 try rx=32, tx=33  // conflict with TFT/CS
  DFRobot_DF2301Q_UART DF2301Q(/*hardSerial =*/&Serial1, /*rx =*/ 5, /*tx =*/ 18);     // use correct pins for Lolin D32
#else   // Use hardware serial: Serial1
  DFRobot_DF2301Q_UART DF2301Q(/*hardSerial =*/&Serial1);
#endif

void clearScreen(void){
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0,0);
  screen_line_ctr = 0;
}

void DisplayCommand(uint8_t CMDID){

  char Msg[23] = " ";
  switch(CMDID){
    case 1:
      strcpy(Msg," Custom Wake Word"); break;
    case 2:
      strcpy(Msg," Waiting for Command"); break;
    case 22:
      strcpy(Msg,"Go Forward"); break;
    case 23:
      strcpy(Msg,"Retreat"); break;      
    case 24:
      strcpy(Msg,"Park The Car"); break;
    case 25:
      strcpy(Msg,"Turn Left 90-deg"); break;
    case 26:
      strcpy(Msg,"Turn Left 45-deg"); break;
    case 27:
      strcpy(Msg,"Turn Left 30-deg"); break;
    case 28:
      strcpy(Msg,"Turn right 90-deg"); break;
    case 29:
      strcpy(Msg,"Turn right 45-deg"); break;
    case 30:
      strcpy(Msg,"Turn right 30-deg"); break;
    case 31:
      strcpy(Msg,"Shift down a gear"); break;   
    case 32:
      strcpy(Msg,"Line Tracking Mode"); break;
    case 33:
      strcpy(Msg,"Light Tracking Mode"); break;
    case 34:
      strcpy(Msg,"Bluetooth Mode"); break;        
    case 35:
      strcpy(Msg,"Obstacle Avoidance"); break;      
    case 36:
      strcpy(Msg,"Face Recognition"); break;
    case 37:
      strcpy(Msg,"Object Tracking"); break;
    case 38:
      strcpy(Msg,"Object Recognition"); break;                  
    case 39:
      strcpy(Msg,"Line Tracking"); break; 
    case 40:
      strcpy(Msg,"Color Recognition"); break;
    case 41:
      strcpy(Msg,"Tag Recognition"); break;
    case 42:
      strcpy(Msg,"Object Sorting"); break;
    case 43:
      strcpy(Msg,"QR Code Recognition"); break;
    case 44:
      strcpy(Msg,"General Settings"); break;
    case 45:
      strcpy(Msg,"Clear Screen"); 
      clearScreen();
      break;        
    case 46:
      strcpy(Msg,"Learn once"); break;
    case 47:
      strcpy(Msg,"Forget"); break;
    case 48:
      strcpy(Msg,"Load Model"); break;      
    case 49:
      strcpy(Msg,"Save Model"); break;
    case 50:
      strcpy(Msg,"Take Photo & Save"); break;
    case 51:
      strcpy(Msg,"Save & Return"); break;                  
    case 52:
      clearScreen();
      strcpy(Msg,"Display Number Zero"); 
      tft.setTextSize(15); tft.print("0");
      delay(2000); clearScreen();
      tft.setTextSize(2); break;      
    case 53:
      clearScreen();
      strcpy(Msg,"Display Number One"); 
      tft.setTextSize(15); tft.print("1");
      delay(2000); clearScreen();
      tft.setTextSize(2); break;  
    case 54:
      clearScreen();
      strcpy(Msg,"Display Number Two"); 
      tft.setTextSize(15); tft.print("2");
      delay(2000); clearScreen();
      tft.setTextSize(2); break;       
    case 55:
      clearScreen();
      strcpy(Msg,"Display Number Three"); 
      tft.setTextSize(15); tft.print("3");
      delay(2000); clearScreen();
      tft.setTextSize(2); break;     
    case 56:
      clearScreen();
      strcpy(Msg,"Display Number Four"); 
      tft.setTextSize(15); tft.print("4");
      delay(2000); clearScreen();
      tft.setTextSize(2); break;       
    case 57:
      clearScreen();
      strcpy(Msg,"Display Number Five"); 
      tft.setTextSize(15); tft.print("5");
      delay(2000); clearScreen();
      tft.setTextSize(2); break;       
    case 58:
      clearScreen();
      strcpy(Msg,"Display Number Six"); 
      tft.setTextSize(15); tft.print("6");
      delay(2000); clearScreen();
      tft.setTextSize(2); break;       
    case 59:
      clearScreen();
      strcpy(Msg,"Display Number Seven"); 
      tft.setTextSize(15); tft.print("7");
      delay(2000); clearScreen();
      tft.setTextSize(2); break;       
    case 60:
      clearScreen();
      strcpy(Msg,"Display Number Eight"); 
      tft.setTextSize(15); tft.print("8");
      delay(2000); clearScreen();
      tft.setTextSize(2); break;       
    case 61:
      clearScreen();
      strcpy(Msg,"Display Number Nine"); 
      tft.setTextSize(15); tft.print("9");
      delay(2000); clearScreen();
      tft.setTextSize(2); break;        

    case 92:
      strcpy(Msg,"Play Music");break; 
    case 93:
      strcpy(Msg,"Stop Playing"); break; 
    case 94:
      strcpy(Msg,"The Last Track"); break;
    case 95:
      strcpy(Msg,"The Next Track"); break;
    case 96:
      strcpy(Msg,"Repeat This Track"); break;
    case 97:
      strcpy(Msg,"Volume Up"); break;
    case 98:
      strcpy(Msg,"Volume Down"); break;
  }

  // clear screen if we reach bottom of TFT display
  if (screen_line_ctr > screen_max_lines){
    clearScreen();
  }

  // display Msg on TFT screen
  tft.print(CMDID); tft.print(": "); tft.println(Msg);
  screen_line_ctr++;
}


void setup()
{
  Serial.begin(115200);

  ts.begin();
  ts.setRotation(SCREEN_ROTATION);

  tft.begin();
  tft.setRotation(SCREEN_ROTATION);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("DFRobot Voice Activation");
  tft.println(" ");
  screen_line_ctr = 2;
  // tft.fillScreen(ILI9341_BLACK);

  #if defined(ARDUINO_SAM_ZERO)   // The M0 board needs to be initialized at this time
    Serial1.begin(DF2301Q_UART_BAUDRATE);
  #endif

  // Init the sensor
  while( !( DF2301Q.begin() ) ) {
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");

  /**
   * @brief Reset the module
   */
  // DF2301Q.resetModule();

  /**
   * @brief Set commands of the module
   * @param setType - Set type
   * @n       DF2301Q_UART_MSG_CMD_SET_VOLUME : Set volume, the set value range 1-7 
   * @n       DF2301Q_UART_MSG_CMD_SET_ENTERWAKEUP : Enter wake-up state; set value 0
   * @n       DF2301Q_UART_MSG_CMD_SET_MUTE : Mute mode; set value 1: mute, 0: unmute
   * @n       DF2301Q_UART_MSG_CMD_SET_WAKE_TIME : Wake-up duration; the set value range 0-255s
   * @param setValue - Set value, refer to the set type above for the range
   */
//  DF2301Q.settingCMD(DF2301Q_UART_MSG_CMD_SET_MUTE, 0);
//  DF2301Q.settingCMD(DF2301Q_UART_MSG_CMD_SET_VOLUME, 5);
//  DF2301Q.settingCMD(DF2301Q_UART_MSG_CMD_SET_WAKE_TIME, 20);
// DF2301Q.settingCMD(DF2301Q_UART_MSG_CMD_SET_ENTERWAKEUP, 0);

  /**
   * @brief Play the corresponding reply audio according to the command word ID
   * @param CMDID - Command word ID
   */
  DF2301Q.playByCMDID(23);

}

void loop()
{
  /**
   * @brief Get the ID corresponding to the command word 
   * @return Return the obtained command word ID, returning 0 means no valid ID is obtained
   */
  uint8_t CMDID = 0;
  CMDID = DF2301Q.getCMDID();
  if(0 != CMDID) {
    Serial.print("CMDID = ");
    Serial.println(CMDID);
    DisplayCommand(CMDID);
  }
  else {
    Serial.println("waiting....");
  }
  delay(1000);
}