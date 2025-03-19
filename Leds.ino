// бегущие частицы , версия с FastLED

#define STRIP_PIN 2     // пин ленты
#define SOUND_PIN A0      // пин звука

#define COLOR_STEP 151  // шаг цвета, интересные 151, 129
#define LEDS_AM 12     // количество светодиодов
#define P_SPEED 2       // скорость движения

// библиотека, с которой будем работать 
//(ранее устанавливали её в разделе Управление библиотеками)
#include <FastLED.h>
// массив из светодиодов назовём leds 
CRGB leds[LEDS_AM];

#include "VolAnalyzer.h"
VolAnalyzer sound(SOUND_PIN);

byte curColor = 0;      // текущий цвет

void setup() {
  FastLED.addLeds<WS2812, STRIP_PIN, GRB>(leds, LEDS_AM);
  FastLED.setBrightness(255);

  // настройки анализатора звука
  sound.setVolK(15);        // снизим фильтрацию громкости (макс. 31)
  sound.setVolMax(255);     // выход громкости 0-255
  sound.setPulseMax(200);   // сигнал пульса
  sound.setPulseMin(150);   // перезагрузка пульса
}

void loop() {
  if (sound.tick()) {   // если анализ звука завершён (~10мс)
    // перематываем массив светодиодов на P_SPEED вправо
    for (int k = 0; k < P_SPEED; k++) {
      for (int i = LEDS_AM - 1; i > 0; i--) leds[i] = leds[i - 1];
    }
    
    // резкий звук - меняем цвет
    if (sound.pulse()) curColor += COLOR_STEP;
    
    // берём текущий цвет с яркостью по громкости (0-255)
    CRGB color = CHSV(curColor, 255, sound.getVol());
    
    // красим P_SPEED первых светодиодов
    for (int i = 0; i < P_SPEED; i++) leds[i] = color;
    
    // выводим
    FastLED.show();
  }
}