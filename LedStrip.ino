// import the necessary libraries
#include "FastLED.h"
#include "IRremote.h"

//define some variables
#define LEDPIN       6
#define RECV_PIN     3
#define LED_TYPE     WS2812B
#define NUM_LEDS     60
byte brightness = 70;
CRGB leds[NUM_LEDS];

//set up IR receiver information
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0;

//<————————————————————————————SETUP————————————————————————————>
void setup()
{
  //begin serial communication
  //Serial.begin(9600);
  //sanity delay
  delay(1000);
  // On éteint la pin 13 (qui active la led "L" présente sur la UNO)
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  //start the receiver
  irrecv.enableIRIn();

  //set up LED strip information
  FastLED.addLeds<LED_TYPE, LEDPIN>(leds, NUM_LEDS);//.setCorrection( TypicalSMD5050 );
  FastLED.setBrightness(brightness);
}

//<—————————————————————————LOOP———————————————————————————————>
void loop() {  
  if (irrecv.decode(&results)) {
    key_value = results.value;
    //Serial.println(results.value, HEX);
    FastLED.delay(10);

    //<——————————————————BUTTON FUNCTIONS—————————————————————————>
    //200+ = plain white
    if (results.value == 0xFFB04F) {
      toutColorer(255, 200, 200);
    }

    //vol+ = brightness up
    else if (results.value == 0xFFA857) {
      if(brightness <= 245)   brightness += 10;
      else                    brightness = 255;
      FastLED.setBrightness(brightness);
      FastLED.show();
    }

    //vol- = brightness down
    else if (results.value == 0xFFE01F) {
      if(brightness > 10)    brightness -= 10;
      else                    brightness = 5;
      FastLED.setBrightness(brightness);
      FastLED.show();
    }

    // CH- : lever de soleil
    else if (results.value == 0xFFA25D) {
      degrade(255, 0, 0, 255, 200, 0);
    }

    //CH : jaune vers vert
    else if (results.value == 0xFF629D) {
      degrade(255, 80, 0, 0, 200, 20);
    }

    // CH+ : nuit
    else if (results.value == 0xFFE21D) {
      degrade(255, 0, 200, 0, 0, 150);
    }

    // >|| : Ajouter des etoiles
    else if (results.value == 0xFFC23D) {
      ajouterEtoile();
    }

    //eq = rainbow
    else if (results.value == 0xFF906F) {
      initialize_rainbow();
    }

    // 0 = éteint
    else if (results.value == 0xFF6897) {
      toutColorer(0,0,0);
    }

    // 1 = rouge
    else if (results.value == 0xFF30CF) {
      toutColorer(255,0,0);
    }

    // 2 = vert
    else if (results.value == 0xFF18E7) {
      toutColorer(0,255,0);
    }

    // 3 = bleu
    else if (results.value == 0xFF7A85) {
      toutColorer(0,0,255);
    }

    // 4 = magenta
    else if (results.value == 0xFF10EF) {
      toutColorer(255,0,200);
    }

    // 5 = Jaune
    else if (results.value == 0xFF38C7) {
      toutColorer(255,140,0);
    }

    // 6 = Cyan
    else if (results.value == 0xFF5AA5) {
      toutColorer(0,255,255);
    }

    // 7 = orange
    else if (results.value == 0xFF42BD) {
      toutColorer(255, 80, 0);
    }

    // 8 = violet
    else if (results.value == 0xFF4AB5) {
      toutColorer(150,0,200);
    }

    // 9 = Turquoise
    else if (results.value == 0xFF52AD) {
      toutColorer(0,255,80);
    }

    irrecv.resume(); // Receive the next value
  }
}

//<——————————————————————OTHER FUNCTIONS————————————————————>

void toutColorer(int r, int g, int b){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB(g, r, b);
  }
  FastLED.show();
}

// Permet d'avoir un dégradé orange -> jaune -> bleu
void sunset() {
   static uint8_t starthue = 0;
   fill_rainbow( leds + 5, NUM_LEDS - 5, --starthue, 20);
   FastLED.show();
}


/// Permet d'initialiser toutes les leds à une couleur d'intervalle. Fonctionne uniquement pour 60 LEDs
void initialize_rainbow() {
  int r = 255, g = 0, b = 0;
  leds[NUM_LEDS-1] = CRGB( g, r, b);
  for (int i = NUM_LEDS-2 ; i >= 0; --i) {
    if (i > 39)       {
      r -= 12;
      g += 12;
    }
    else if (i == 39) {
      r = 0;
      g = 255;
      b = 0;
    }
    else if (i > 19)  {
      g -= 12;
      b += 12;
    }
    else if (i == 19) {
      g = 0;
      b = 255 ;
    }
    else              {
      b -= 12;
      r += 12;
    }

    leds[i] = CRGB( g, r, b);
  }
  FastLED.show();
}

void degrade(int r1, int g1, int b1, int r2, int g2, int b2) {
  float diffR, diffG, diffB;
  diffR = (float)((r2-r1)/NUM_LEDS);
  diffG = (float)((g2-g1)/NUM_LEDS);
  diffB = (float)((b2-b1)/NUM_LEDS);

  int r, g, b;
  
  for(int i = NUM_LEDS-1; i >=0 ; i--) {
    leds[i] = CRGB(g1, r1, b1);
    r1+=diffR;
    g1+=diffG;
    b1+=diffB;
  }
  FastLED.show();
}

void ajouterEtoile() {
  long randomValue = 0;
  for(int i = 0; i < NUM_LEDS; i++){
    randomValue = random(0, 15);
    if(randomValue == 5)  leds[i] = CRGB(255,255,255);
  }
  FastLED.show();
}
