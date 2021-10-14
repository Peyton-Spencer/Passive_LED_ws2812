
#include  <Arduino.h>
#include <FastLED.h>
#include "Button.h"

//Pixel Definitions
#define DATA_PIN 13
#define LED_TYPE NEOPIXEL
#define NUM_LEDS    300
#define BRIGHTNESS         100
#define FRAMES_PER_SECOND  90

//Pin Definitions
#define UP 3
#define DOWN 2
#define COLOR_BUTTON 13
#define INTENSITY_BUTTON 12
#define COLOR_LIGHT 11
#define INTENSITY_LIGHT 11
#define POT_PIN A0

//LED array
CRGBArray<NUM_LEDS> leds;

//Forward Declarations
void ampBars(uint8_t), ampTwo(uint8_t);
void rainbow(), rainbowWithGlitter(), confetti(), TwodotSin(), juggle(), bpm(), sinWaves(), antiAlias(), paletteFade();
void addGlitter(fract8), toPattern(uint8_t);

//Initialize Buttons
Button patUP = Button(UP); //Pattern up
Button patDOWN = Button(DOWN);  //Pattern down
Button intensitySET = Button(INTENSITY_BUTTON);  //adjust speed, or intensity of animation with potentiometer

void setup()
{
	delay(1000);
	//Initialize strip as FastLED
	FastLED.addLeds<LED_TYPE,DATA_PIN>(leds,NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.setBrightness(BRIGHTNESS);
	//Activate potentiometer and setting toggles
	pinMode(POT_PIN, INPUT);
	pinMode(INTENSITY_LIGHT, OUTPUT);

}

//Typedef which is used to cycle through and call the light functions
using PatternList = void(*[])();  //(alternative syntax) typedef void (*PatternList[])();
PatternList gPatterns = {colorGradient, rainbow, confetti, TwodotSin, juggle, bpm, sinWaves, antiAlias};
uint8_t gCurrentPattern = 0; //index for pattern list
uint8_t gHue = 0; //rotating base color used by patterns

uint8_t intensity = 254;
bool isSettingSpeed = false;

void loop()
{
	//Check pattern up/down buttons
	if(patUP.checkState())
		toPattern(++gCurrentPattern);
	if(patDOWN.checkState())
		toPattern(--gCurrentPattern);
	//Check intensity button
	if(intensitySET.checkState())
	{
		digitalWrite(INTENSITY_LIGHT, 0);
		isSettingSpeed = !isSettingSpeed;
	}
	if(isSettingSpeed)
	{
		digitalWrite(INTENSITY_LIGHT, 1);
		intensity = analogRead(POT_PIN)/4;
	}
	//Call the current pattern function
	
	// send the 'leds' array out to the actual LED strip
	EVERY_N_MILLISECONDS(5)	{gPatterns[gCurrentPattern]();}	
	EVERY_N_MILLISECONDS(15) {FastLED.show();}
	// insert a delay to keep the framerate modest
	// FastLED.show();
	// FastLED.delay(1000/FRAMES_PER_SECOND);

	EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow

}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0])) //for size of pattern array
uint8_t numPatterns = ARRAY_SIZE(gPatterns);
void toPattern(uint8_t index)
{
	if(index==255)
		index = numPatterns-1;
	// add one to the current pattern number, and wrap around at the end
	gCurrentPattern = index % numPatterns;
}




//LIGHT PATTERNS
void rainbow()
{
	// FastLED's built-in rainbow generator
	fill_rainbow(leds, NUM_LEDS, gHue, 9);
}

void rainbowWithGlitter()
{
	// built-in FastLED rainbow, plus some random sparkly glitter
	rainbow();
	addGlitter(80);
}

void colorGradient()
{
	fill_gradient_RGB(leds, 0, CHSV(100, 200, 100), 299,  CHSV(255, 200, 255));
}

void confetti()
{
	// random colored speckles that blink in and fade smoothly
	fadeToBlackBy( leds, NUM_LEDS, 10);
	if(random8() < intensity) //High intensity = high chance of triggering a blink
	{
		int pos = random16(NUM_LEDS-5); //Random position for blink
		//Fills a random hue -- max intensity = 1_width  min intensity = 5_width
		leds(pos, pos + (255 - intensity) / 50).fill_solid(CHSV(gHue + random8(32), 200, 255));
	}
}

void TwodotSin()
{
	// a colored dot sweeping back and forth, with fading trails
	fadeToBlackBy( leds, NUM_LEDS, 40);
	int pos = beatsin16( intensity/32, 0, NUM_LEDS-1 );
	leds[pos] += CHSV( gHue, 255, 192);
	pos = NUM_LEDS - 1 - pos;
	leds[pos] += CHSV(gHue, 255, 192);
}

void bpm()
{
	// colored stripes pulsing at a defined Beats-Per-Minute (BPM)
	uint8_t BeatsPerMinute = 62;
	CRGBPalette16 palette = PartyColors_p;
	uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
	for( int i = 0; i < NUM_LEDS; i++) { //9948
		leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
	}
}

void juggle() {
	// eight colored dots, weaving in and out of sync with each other
	fadeToBlackBy( leds, NUM_LEDS, 40);
	byte dothue = 0;
	for( int i = 0; i < 8; i++) {
		leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
		dothue += 32;
	}
}

void sinWaves()
{
	//Dots along the length of the strip following a sin wave function.
	//Length of each sin wave depends on intensity variable
	fadeToBlackBy(leds, NUM_LEDS, 40);
	uint8_t waveSize = intensity/8;
	int pos = beatsin8(40, 0 , waveSize-1); //Position of one dot
	for(int i = 0; i < NUM_LEDS/waveSize; i++)
	{
		leds[i*waveSize + pos] =  CHSV(gHue, 255, 255); //Copy each dot along the strip
	}
}

void antiAlias()
{
	//Smooth waves that run along the strip
	static uint8_t iteration = 0;
	iteration+=1;
    uint8_t effectLength = intensity/5;
	for(int sinwave = 0; sinwave<effectLength; sinwave++)
	{
		//get sinWave brightness for a dot in each section of animation
		uint8_t bright = quadwave8(iteration + sinwave*255/effectLength);
		for(int i = 0; i < NUM_LEDS/effectLength; i++)
		{
			//Copy the dot brightness into each animation section
			leds[i*effectLength+sinwave] = CHSV(200, 255, bright);
		}
	}

}

void paletteFade()
{
	CRGBPalette16 palette = ForestColors_p;
	uint8_t sectionLength = NUM_LEDS/15;
	static uint8_t iteration = 0;
	static uint8_t colorCounter = 1;
	for(int i = 0; i < 15; i++)
	{
		leds[i*sectionLength+iteration] = ColorFromPalette(palette, (i+colorCounter)*15, 255);
	}
	iteration++;
	if(iteration == 20)
	{
		iteration = 0;
		colorCounter++;
	}
}








void ampBars(uint8_t amp)
{
	fill_solid(leds, amp, CRGB::Maroon);
}

void ampTwo(uint8_t amp)
{
	leds[amp] = CRGB::Cornsilk;
}
