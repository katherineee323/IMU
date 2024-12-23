#include <Wire.h>
#include <JY901.h>

#include "FS.h"
#include "SD.h"
#include "SPI.h"
/*
http://item.taobao.com/item.htm?id=43511899945
Test on mega2560.
JY901   mega2560
TX <---> 0(Rx)
*/

#define REASSIGN_PINS
int sck = 7;
int miso = 8;
int mosi = 9;
int cs = 21;


void setup() 
{
  Serial.begin(9600);  
  Serial0.begin(9600);

#ifdef REASSIGN_PINS
  SPI.begin(sck, miso, mosi, cs);
  if (!SD.begin(cs)) {
#else
  if (!SD.begin()) {
#endif
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}


void loop() 
{
  //print received data. Data was received in serialEvent;
               
  Serial.print((float)JY901.stcAcc.a[0]/32768*16);Serial.print(" ");Serial.print((float)JY901.stcAcc.a[1]/32768*16);Serial.print(" ");Serial.println((float)JY901.stcAcc.a[2]/32768*16);
  
  Serial.print((float)JY901.stcGyro.w[0]/32768*2000);Serial.print(" ");Serial.print((float)JY901.stcGyro.w[1]/32768*2000);Serial.print(" ");Serial.println((float)JY901.stcGyro.w[2]/32768*2000);
  
  Serial.print((float)JY901.stcAngle.Angle[0]/32768*180);Serial.print(" ");Serial.print((float)JY901.stcAngle.Angle[1]/32768*180);Serial.print(" ");Serial.println((float)JY901.stcAngle.Angle[2]/32768*180);
  
  Serial.print(JY901.stcMag.h[0]);Serial.print(" ");Serial.print(JY901.stcMag.h[1]);Serial.print(" ");Serial.println(JY901.stcMag.h[2]);

  char message[200];
  sprintf(message, "%.2f %.2f %.2f\r\n%.2f %.2f %.2f\r\n%.2f %.2f %.2f\r\n%d %.d %d\r\n", ((float)JY901.stcAcc.a[0]/32768*16), ((float)JY901.stcAcc.a[1]/32768*16), ((float)JY901.stcAcc.a[2]/32768*16), \
                                                                                                ((float)JY901.stcGyro.w[0]/32768*2000), ((float)JY901.stcGyro.w[1]/32768*2000), ((float)JY901.stcGyro.w[2]/32768*2000), \
                                                                                                ((float)JY901.stcAngle.Angle[0]/32768*180), ((float)JY901.stcAngle.Angle[1]/32768*180), ((float)JY901.stcAngle.Angle[2]/32768*180), \
                                                                                                (JY901.stcMag.h[0]), (JY901.stcMag.h[1]), (JY901.stcMag.h[2]));

File file = SD.open("/IMU.txt", FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    File file = SD.open("/IMU.txt", FILE_WRITE);
    if (!file) {
      Serial.println("Failed to open file for writing");
      return;
    }
    if (file.print(message)) {
      Serial.println("File written");
    } else {
      Serial.println("Write failed");
    }
    file.close();
  } else {
    if (file.print(message)) {
      Serial.println("Message appended");
    } else {
      Serial.println("Append failed");
    }
      file.close();
    }

  delay(500);

  while (Serial0.available()) 
  {
    JY901.CopeSerialData(Serial0.read()); //Call JY901 data cope function
  }

}
