#include <Esp.h>
#include <NeoPixelBus.h>
#include <ESP8266WiFi.h>
#include <time.h>

#include "config.h"

const uint16_t PixelCount = 16; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266
#define colorSaturation 80

// three element pixels, in different order and speeds
NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> strip(PixelCount, PixelPin);

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);
RgbColor red_blue(colorSaturation, 0, colorSaturation);

void setup(void)
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Initializing...");

  // this resets all the neopixels to an off state
  strip.Begin();
  strip.Show();

  Serial.println("Connecting to wifi...");
  WiFi.begin(APNAME, PASSWORD);
  int idx = 0;
  while (!WiFi.isConnected()) {
    strip.SetPixelColor(idx, black);
    idx = (idx + 1) % PixelCount;
    strip.SetPixelColor(idx, green);
    strip.Show();
    delay(1000);
  }

  configTime(3 * 3600, 0 * 3600, NTPSERVER);

  Serial.println("Waiting for time to synchronize...");
  while (time(NULL) < 3600) {
    strip.SetPixelColor(idx, black);
    idx = (idx + 1) % PixelCount;
    strip.SetPixelColor(idx, green);
    strip.Show();
    delay(1000);
  }
  strip.SetPixelColor(idx, black);

  Serial.println("Running...");
}

static inline int time_to_idx(int t, float factor)
{
  float timef = t;
  return timef / factor;
}

void loop(void)
{
  static int last_hour_idx = -1;
  static int last_minute_idx = -1;
  static float hour_factor = 12.0 / (float)PixelCount;
  static float minute_factor = 60.0 / (float)PixelCount;

  time_t t = time(NULL);
  int minute = (t / 60) % 60;
  int hour = (t / 3600) % 12;

  Serial.print("Time: ");
  Serial.print(hour);
  Serial.print(':');
  Serial.print(minute);

  int hour_idx = time_to_idx(hour, hour_factor);
  int minute_idx = time_to_idx(minute, minute_factor);

  Serial.print(' ');
  Serial.print(hour_idx);
  Serial.print(' ');
  Serial.println(minute_idx);

  if (hour_idx != last_hour_idx || minute_idx != last_minute_idx) {
    strip.SetPixelColor(last_hour_idx, black);
    strip.SetPixelColor(last_minute_idx, black);

    if (hour_idx == minute_idx) {
      strip.SetPixelColor(hour_idx, red_blue);
    } else {
      strip.SetPixelColor(hour_idx, red);
      strip.SetPixelColor(minute_idx, blue);
    }
    strip.Show();

    last_hour_idx = hour_idx;
    last_minute_idx = minute_idx;
  }

  delay(5000);
}
