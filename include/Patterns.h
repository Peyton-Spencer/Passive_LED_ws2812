///*
// * PatternList.h
// *
// *  Created on: Dec 2, 2017
// *      Author: pas12_000
// */
//
//#ifndef PATTERNLIST_H_
//#define PATTERNLIST_H_
//
//#include <Arduino.h>
//#include "FastLED.h"
//#include "EEPROM.h"
//
////typedef void (*Pattern)();
//
//#define NUM_PATTERNS 5
//#define PATTERN_NUMBER_ADDRESS 0
//
////Variables: (led_array, num_leds, rotating_hue, color, intensity)
//using Pattern = void(*)(CRGB*, uint16_t, uint8_t, CRGB, uint8_t);
//
////using PatternList = void(Patterns::*[])();
//
//
////typedef array to hold the patterns
////using PatternList = void(*)();
//
////std::array<void(*)(), 5> arraypatlist;
//
//
//
//	Pattern rainbow(CRGB* leds, uint16_t numLeds, uint8_t gHue, CRGB color, uint8_t intensity)
//	{
//			// FastLED's built-in rainbow generator
//			fill_rainbow( leds, numLeds, gHue, 7);
//	}
//
//	Pattern rainbowWithGlitter()
//	{
//		// built-in FastLED rainbow, plus some random sparkly glitter
//		rainbow();
//		addGlitter(80);
//	}
//
//	void addGlitter( fract8 chanceOfGlitter)
//	{
//		if( random8() < chanceOfGlitter) {
//			leds[ random16(numLeds) ] += CRGB::White;
//		}
//	}
//
//	void confetti()
//	{
//		// random colored speckles that blink in and fade smoothly
//		fadeToBlackBy( leds, numLeds, 10);
//		int pos = random16(numLeds);
//		leds[pos] += CHSV( gHue + random8(64), 200, 255);
//	}
//
//	void sinelon()
//	{
//		// a colored dot sweeping back and forth, with fading trails
//		fadeToBlackBy( leds, numLeds, 20);
//		int pos = beatsin16( 13, 0, numLeds-1 );
//		leds[pos] += CHSV( gHue, 255, 192);
//	}
//
//	void bpm()
//	{
//		// colored stripes pulsing at a defined Beats-Per-Minute (BPM)
//		uint8_t BeatsPerMinute = 62;
//		CRGBPalette16 palette = PartyColors_p;
//		uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
//		for( int i = 0; i < numLeds; i++) { //9948
//			leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
//		}
//	}
//
//	void juggle() {
//		// eight colored dots, weaving in and out of sync with each other
//		fadeToBlackBy( leds, numLeds, 20);
//		byte dothue = 0;
//		for( int i = 0; i < 8; i++) {
//			leds[beatsin16( i+7, 0, numLeds-1 )] |= CHSV(dothue, 200, 255);
//			dothue += 32;
//		}
//	}
//
//
//
//
//
//
//#endif /* PATTERNLIST_H_ */
