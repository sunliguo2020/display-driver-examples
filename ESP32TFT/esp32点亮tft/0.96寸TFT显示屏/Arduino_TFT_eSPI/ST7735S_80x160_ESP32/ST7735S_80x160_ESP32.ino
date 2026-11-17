#include <TFT_eSPI.h>

TFT_eSPI tft;

void setup() {
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(4, 6);
  tft.println("ST7735S");
  tft.setCursor(4, 18);
  tft.println("80 x 160");

  const int16_t barWidth = (tft.width() - 16) / 3;
  const int16_t barY = 48;
  const int16_t barHeight = tft.height() - barY - 12;

  tft.fillRect(4, barY, barWidth, barHeight, TFT_RED);
  tft.fillRect(8 + barWidth, barY, barWidth, barHeight, TFT_GREEN);
  tft.fillRect(12 + 2 * barWidth, barY, barWidth, barHeight, TFT_BLUE);
}

void loop() {
}
