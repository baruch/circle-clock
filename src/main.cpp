#include <Esp.h>
#include <NeoPixelBus.h>
#include <ESP8266WiFi.h>
#include <time.h>
#include <DS3232RTC.h>
#include <Wire.h>

#include "config.h"
#include "table.c"

#define colorSaturation 80

// three element pixels, in different order and speeds
NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> strip(PIXEL_COUNT, -1);

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);
RgbColor red_blue(colorSaturation, 0, colorSaturation);

bool rtc_active = false;

void setup(void)
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Initializing...");
  Wire.begin(0, 12);

  time_t t = RTC.get();
  if (t > 3600) {
          Serial.println("RTC already set");
          rtc_active = true;
  } else {
          Serial.println("Connecting to wifi...");
          WiFi.begin(APNAME, PASSWORD);

          while (!WiFi.isConnected()) {
                  delay(10);
          }

          configTime(3 * 3600, 0 * 3600, NTPSERVER);

          Serial.println("Waiting for time to synchronize...");
          while (time(NULL) < 3600) {
                  delay(10);
          }

          int ret = RTC.set(time(NULL));
          if (ret == 0)
                  rtc_active = true;
  }

  Serial.println("Running...");
}

void loop(void)
{
  time_t t = rtc_active ? RTC.get() : time(NULL);
  int minute = (t / 60) % 60;
  int hour = (t / 3600) % 12;

  Serial.print("Time: ");
  Serial.print(hour);
  Serial.print(':');
  Serial.print(minute);

  int hour_idx = hour_idx_tbl[hour];
  int minute_idx = minute_idx_tbl[minute].idx;

  Serial.print(' ');
  Serial.print(hour_idx);
  Serial.print(' ');
  Serial.println(minute_idx);

  // this resets all the neopixels to an off state
  strip.Begin();

  // Now set the pixels we want
  if (hour_idx == minute_idx) {
          strip.SetPixelColor(hour_idx, red_blue);
  } else {
          strip.SetPixelColor(hour_idx, red);
          strip.SetPixelColor(minute_idx, blue);
  }
  strip.Show();

  int time_diff = minute_idx_tbl[minute].time_to_sleep - (t%60);
  Serial.print("Sleep for ");
  Serial.print(time_diff);
  Serial.println(" seconds");

  delay(2);
  ESP.deepSleep(time_diff * 1000 * 1040); // Add 4% to compensate for deep sleep time inaccuracy
  delay(10);
}
