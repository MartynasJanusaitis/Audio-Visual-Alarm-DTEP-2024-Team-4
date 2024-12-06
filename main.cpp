#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIXEL_PIN 6
#define NUMPIXELS 7
#define DELAYVAL 500000 // microseconds
#define LEVEL 60//up to 100
#define THRESHOLD 650
#define LEVEL 30

Adafruit_NeoPixel pixels(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
int pixelsOn = 0;
unsigned long int timestamp = micros();
int analogVal = 0;

void setup() {
    Serial.begin(9600);
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif

    pixels.begin();

}

int getOnCount(){
    for (int i=NUMPIXELS-1; i>-1; i--){
        if (pixels.getPixelColor(i) != 0){
            return i+1;
        }
    }
    for (int i=NUMPIXELS-1; i>-1; i--)
    {
        Serial.print(pixels.getPixelColor(i));
        Serial.print(" ");

    }
    Serial.println();
    return 0;
}

int soundLevelToLedCount(int soundLevel)
{
    int pixelCount = ((soundLevel - THRESHOLD)/LEVEL) + 2; //+2 Because one has to always be on when above the threshold and one is hidden under the wood
    if (pixelCount > NUMPIXELS){
        pixelCount = NUMPIXELS;
    }
    return pixelCount;
}

void lightPixels(int pixelCount) {
    pixels.clear();
    if (pixelCount < pixelsOn){
        return;
    }
    for(int i=0; i<pixelCount; i++) {
        int offset = round((255/NUMPIXELS)*1.5*(i+1));
        if (offset > 255){
            offset = 255;
        }
        pixels.setPixelColor(i, pixels.Color(offset, 255-offset, 0));

    }
    pixels.show();
    pixelsOn = pixelCount;
}


void loop() {
    analogVal = analogRead(A0);
    if (analogVal > THRESHOLD) {
        lightPixels(soundLevelToLedCount(analogVal));
        timestamp = micros();
    }
    if (pixelsOn && (micros() - timestamp) > DELAYVAL){
        lightPixels(--pixelsOn);
        timestamp = micros();
    }
}
