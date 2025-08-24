#include "Smartlight.h"
#include <Arduino.h>
#include <FastLED.h>
#define DATA_PIN 13
#define NUM_LEDS 118
#define SEGMENT_LEDS 59


#define LAMP1_START 0
#define LAMP1_END 58
#define LAMP2_START 59
#define LAMP2_END 118
unsigned long lastUpdateRainbow = 0;

uint8_t colorIndex = 0;
uint8_t wavePos = 0;


int pos = 0;
int direction = 1;

int lastEffect = -1;
bool fireJustStarted = true;

int tempIndex = 0;
bool temp_flag = 0;
CRGB leds[NUM_LEDS];



byte random_effect = 1;





#define MAX_FIREFLIES 8  // сколько "светлячков" одновременно
unsigned long lastUpdate = 0;
unsigned long lastSpark = 0;

struct Firefly {
  int pos;
  CRGB color;
  uint8_t brightness;
  bool active;
};

Firefly fireflies[MAX_FIREFLIES];

// Мягкие "вечерние" цвета
CRGBPalette16 eveningPalette = CRGBPalette16(
  CRGB::LightYellow, CRGB::LightPink, CRGB::SkyBlue, CRGB::MistyRose,
  CRGB::LightBlue, CRGB::Lavender, CRGB::LemonChiffon, CRGB::Thistle,
  CRGB::LightYellow, CRGB::LightPink, CRGB::SkyBlue, CRGB::MistyRose,
  CRGB::LightBlue, CRGB::Lavender, CRGB::LemonChiffon, CRGB::Thistle
);




// --- Мягкая пастельная палитра (16 цветов в формате 0xRRGGBB) ---
const TProgmemRGBPalette16 SoftLampPalette PROGMEM = {
  0xFFB496, // Персиковый
  0xB4DCFF, // Мягкий голубой
  0xC8FFC8, // Нежно-зелёный
  0xFFE6B4, // Бежево-золотистый
  0xB4B4FF, // Лавандовый
  0xFFB4D2, // Розовый пастель
  0xDCF0FF, // Ледяной голубой
  0xFFC8AA, // Кремовый
  0xFFB496, // Персиковый
  0xB4DCFF, // Мягкий голубой
  0xC8FFC8, // Нежно-зелёный
  0xFFE6B4, // Бежево-золотистый
  0xB4B4FF, // Лавандовый
  0xFFB4D2, // Розовый пастель
  0xDCF0FF, // Ледяной голубой
  0xFFC8AA  // Кремовый
};

void dualLampEffect() {
  wavePos   += 1;
  colorIndex += 1;

  // Левая лампа
  for (int i = 0; i < SEGMENT_LEDS; i++) {
    uint8_t wave = sin8(i * 8 + wavePos * 2);
    leds[i] = ColorFromPalette(SoftLampPalette, wave + colorIndex, 200, LINEARBLEND);
  }

  // Правая лампа (противофаза)
  for (int i = 0; i < SEGMENT_LEDS; i++) {
    uint8_t wave = sin8(i * 8 - wavePos * 2);
    leds[i + SEGMENT_LEDS] = ColorFromPalette(SoftLampPalette, wave + colorIndex, 200, LINEARBLEND);
  }

  // Искры
  if (random8() < 8) leds[random16(SEGMENT_LEDS)] += CRGB(255, 255, 255) / 4;
  if (random8() < 8) leds[random16(SEGMENT_LEDS) + SEGMENT_LEDS] += CRGB(255, 255, 255) / 4;
}







Smartlight::Smartlight() : brightness(0), currenteffect(0), hue(0) {}


void Smartlight::setEffect(int id) {
  currenteffect = id;
    

  // Сброс при старте эффекта
  if (currenteffect == 2) {
    fireReset = true;
  }
}

void Smartlight::begin(){
    
    
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);

    FastLED.setBrightness(brightness);
    FastLED.setTemperature(Candle);
    FastLED.clear();
    FastLED.show();
    for (int i = 0; i < MAX_FIREFLIES; i++) {
      fireflies[i].active = false;
  }
}






void Smartlight::setPower(bool power) {
  switch(power)
  {
  case 1:
    brightness = 255;
    break;
  case 0:
    brightness = 0;
    break;
  
  }

}

void Smartlight::turnOn() {
    brightness = 255;
    // код для включения светильника
}

void Smartlight::turnOff() {
    brightness = 0;
    // код для выключения светильника
}

void Smartlight::setBrightness(int level) {
    brightness = map(level, 0, 100, 0, 255);
    
    

    // код для установки яркости
}




void Smartlight::update() {
  
  FastLED.setBrightness(brightness);
    FastLED.show();
       if (currenteffect != lastEffect) {
        lastEffect = currenteffect;
        fireJustStarted = true;  
    }
    switch (currenteffect) {
        case 0:
            Fireflies();
            break;
        case 1:
          stars();
            
            break;
        case 2:
                fire(fireJustStarted);  // передаём флаг
            fireJustStarted = false; // после первого вызова — сброс
            
            break;
        case 3:
             LeafNoiseCanopy();
            break;
        case 4:
          rainbowFlow();
          break;
        case 5:
          off();
          break;
        case 6:
          
          white();
          break;
          case 7:
          
          
          break;
          case 8:
          
          random_eff();

          break;

       case 10:
          
          ocean();

          break;


        default:
                 fire(fireJustStarted);
            fireJustStarted = false;
            break;
    }

    
}





//////////Theeeese are all effects, good luck to learn them:)//////////////

void Smartlight::rainbow(){
  for (int i = 0; i < NUM_LEDS; i++) {
    //leds[i] = CHSV(hue, 255, 255);
    leds[i] = CHSV(hue + (i * 10), 255, 255);
  }

  EVERY_N_MILLISECONDS(10){
    hue++;
  }
}




void Smartlight::off() {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    
}


void Smartlight::fire(bool reset) {
    static byte heat[NUM_LEDS];

    if (reset) {
        memset(heat, 0, sizeof(heat));
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show(); // отрисовать черный экран сразу
        return;         // выходим, чтобы на этом кадре не шёл огонь
    }

    // Step 1. Охлаждение
    for (int i = 0; i < NUM_LEDS; i++) {
        heat[i] = qsub8(heat[i], random8(0, ((20 * 10) / NUM_LEDS) + 2));
    }

    // Step 2. Перенос "огня"
    for (int k = NUM_LEDS - 1; k >= 2; k--) {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }

    // Step 3. Вспышка у основания
    if (random8() < 128) {
        int y = random8(7);
        heat[y] = qadd8(heat[y], random8(160, 255));
    }

    // Step 4. Цвета
    for (int j = 0; j < NUM_LEDS; j++) {
        leds[j] = HeatColor(heat[j]);
    }
}


void Smartlight::setTemperature(int number){
  tempIndex = number;
}

void Smartlight::white(){
 
fill_solid(leds, NUM_LEDS, CRGB::White);



switch (tempIndex) {
    case 0:
      FastLED.setTemperature(Candle);
      break;
    case 1:
      FastLED.setTemperature(Tungsten40W);
    break;
    case 2:
      FastLED.setTemperature(Tungsten100W);
    break;
    case 3:
      FastLED.setTemperature(Halogen);
    break;
    case 4:
      FastLED.setTemperature(CarbonArc);
    break;
    case 5:
      FastLED.setTemperature(HighNoonSun);
    break;
    case 6:
      FastLED.setTemperature(DirectSunlight);
    break;
    case 7:
      FastLED.setTemperature(OvercastSky);
    break;
    case 8:
      FastLED.setTemperature(ClearBlueSky);
    break;
  }
    
}








void Smartlight::rainbowFlow() {
  unsigned long now = millis();
  if (now - lastUpdateRainbow > 20) {  // обновлять примерно каждые 20 мс
    fill_rainbow(leds, NUM_LEDS, hue, 7);
    FastLED.show();
    hue++;  // двигаем радугу
    lastUpdateRainbow = now;
  }
}



void Smartlight::Meteor(){
 // Плавно меняем цвет "головы"
  hue += 1;
  CRGB headColor = CHSV(hue, 255, 255);

  // Затухание: каждый пиксель постепенно темнеет
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].fadeToBlackBy(40);
  }

  // Нарисовать "метеор" с хвостом
  leds[pos] = headColor; // голова

  // Хвост с мягким градиентом
  for (int i = 1; i < 10; i++) {
    int index = pos - i * direction;
    if (index >= 0 && index < NUM_LEDS) {
      leds[index] += headColor.nscale8(255 - i * 25);  // хвост светлеет к концу
    }
  }



   pos += direction;
  if (pos >= NUM_LEDS - 1 || pos <= 0) {
    direction = -direction;
  }
}







void spawnFirefly() {
  for (int i = 0; i < MAX_FIREFLIES; i++) {
    if (!fireflies[i].active) {
      fireflies[i].pos = random(NUM_LEDS);
      fireflies[i].color = ColorFromPalette(eveningPalette, random8(), 255, LINEARBLEND);
      fireflies[i].brightness = 255;
      fireflies[i].active = true;
      break;
    }
  }
}

void updateFireflies() {
  for (int i = 0; i < MAX_FIREFLIES; i++) {
    if (fireflies[i].active) {
      leds[fireflies[i].pos] += fireflies[i].color.nscale8(fireflies[i].brightness);
      fireflies[i].brightness = qsub8(fireflies[i].brightness, 5);
      if (fireflies[i].brightness == 0) {
        fireflies[i].active = false;
      }
    }
  }
}






void Smartlight::Fireflies(){
    unsigned long currentMillis = millis();

  // Затухание всех светодиодов
  if (currentMillis - lastUpdate > 20) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i].fadeToBlackBy(8); // очень мягкое затухание
    }
    lastUpdate = currentMillis;
    FastLED.show();
  }

  // Создание новых светлячков с интервалом
  if (currentMillis - lastSpark > 150) {
    spawnFirefly();
    lastSpark = currentMillis;
  }

  // Обновление светлячков
  updateFireflies();

}

// ==== FastLED setup ====
// ==== Palettes ====
DEFINE_GRADIENT_PALETTE(sunset_gp) {
  0,   255, 60, 0,
  100, 255, 0, 80,
  200, 20,  0, 150,
  255, 0,   0, 0
};
CRGBPalette16 sunsetPal = sunset_gp;

DEFINE_GRADIENT_PALETTE(cotton_gp) {
  0,   255, 150, 200,
  128, 100, 200, 255,
  255, 255, 150, 200
};
CRGBPalette16 cottonPal = cotton_gp;

// ==== Timers ====
uint8_t t1 = 0;
uint8_t t2 = 0;
uint8_t t3 = 0;
uint8_t t4 = 0;
uint8_t t5 = 0, t6 = 0, t7 = 0, t8 = 0;
// ==== Effects (no args) ====
void oceanFade() {
  t1++;
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(OceanColors_p, i + t1);
  }
}

void sunsetFlow() {
  t2 += 1;
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(sunsetPal, i * 3 + t2, 255, LINEARBLEND);
  }
}

void cottonCandy() {
  t3++;
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t wave = sin8(i * 8 + t3);
    leds[i] = ColorFromPalette(cottonPal, wave);
  }
}

void lavaPulse() {
  t4 += 1;
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(LavaColors_p, t4 + i * 2, 255, LINEARBLEND);
  }
}




// 🌊 Ocean Wave Fade
void OceanWaveFade() {
  fill_palette(leds, NUM_LEDS,
               t5,                         // phase counter
               255 / NUM_LEDS,
               OceanColors_p,
               255,
               LINEARBLEND);
  t5++;
}

// 🔥 Lava Pulse
void LavaPulse() {
  uint8_t bright = beatsin8(12, 50, 255);   // 12 BPM = slower pulse
  fill_palette(leds, NUM_LEDS,
               t6,
               255 / NUM_LEDS,
               HeatColors_p,
               bright,
               LINEARBLEND);
  t6++;
}

// ✨ Star Twinkle Field
void StarTwinkleField() {
  fadeToBlackBy(leds, NUM_LEDS, 10);
  leds[random16(NUM_LEDS)] += CRGB::White;
  t7++;
}

// 🌈 Double Rainbow Flow
void DoubleRainbowFlow() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(RainbowColors_p, i * 4 + t8, 255);
  }
  for (int i = NUM_LEDS / 2; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(RainbowColors_p, i * 4 - t8, 255);
  }
  t8++;
}




// Tunables
static const uint16_t UPDATE_MS     = 60;   // frame pacing (bigger = slower)
static const uint8_t  SPATIAL_SCALE = 15;   // size of leaf patches
static const uint8_t  TIME_STEP     = 1;    // evolution speed
static const uint8_t  SWAY_BPM      = 6;    // global brightness sway

// Deep green → yellow gradient
DEFINE_GRADIENT_PALETTE( DeepLeaf_gp ) {
  0,    0,  16,  0,    // very dark green
  80,   0,  80,  0,    // mid-forest green
  160,  20,160, 20,    // bright lush green
  220, 180,255, 40,    // leaf under sun
  255, 255,255,  0     // full yellow highlight
};
CRGBPalette16 DeepLeafPal = DeepLeaf_gp;

// State
static uint32_t lastUpdates = 0;
static uint16_t t = 0;          // noise time phase
static uint16_t highlightT = 5000;  // separate phase for yellow dapple



void Smartlight::LeafNoiseCanopy() {
  uint32_t now = millis();
  if (now - lastUpdates < UPDATE_MS) return;
  lastUpdates = now;
  t += TIME_STEP;
  highlightT += TIME_STEP / 2;  // slower phase for sun-dapple

  // global brightness sway like a soft breeze
  uint8_t sway = beatsin8(SWAY_BPM, 130, 200);

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    // Base canopy motion
    uint16_t x = i * SPATIAL_SCALE;
    uint8_t noiseVal = inoise8(x, t);
    uint8_t idx = scale8(noiseVal, 240);   // avoid muddy extremes

    // Build target color from deep leaf palette
    CRGB target = ColorFromPalette(DeepLeafPal, idx, sway, LINEARBLEND);

    // Yellow dapple highlights
    uint8_t hl = inoise8(x + 300, highlightT);
    if (hl > 200) {
      // blend a touch of yellow where noise peaks
      nblend(target, CRGB::Yellow, map(hl, 200, 255, 50, 200));
    }

    // Ease into new color for silky smooth motion
    nblend(leds[i], target, 60);
  }

  // tie neighboring pixels together like clustered leaves
  blur1d(leds, NUM_LEDS, 32);
}




void Smartlight::random_eff(){

 
  EVERY_N_MILLISECONDS(30000){
    random_effect = random(1, 9);
    
  }



  switch (random_effect) {
    
    case 1:
      
      oceanFade();
      break;
    case 2:
      
      sunsetFlow();
      break;
    case 3:
      
      cottonCandy();
      break;
    case 4:
      
      lavaPulse();
      break;
    
    case 5:
      
      OceanWaveFade();
      break;
    case 6:
      
      LavaPulse();
      break;
    case 7:
      
      StarTwinkleField();
      break;
    case 8:
      
      DoubleRainbowFlow();
      break;
  }
}


void Smartlight::ocean(){

 fill_palette(leds, NUM_LEDS,
               t5,                         // phase counter
               255 / NUM_LEDS,
               OceanColors_p,
               255,
               LINEARBLEND);
t5 = t5 + 1;
}


void Smartlight::stars(){
    fadeToBlackBy(leds, NUM_LEDS, 10);
  leds[random16(NUM_LEDS)] += CRGB::White;
  t7++;
}