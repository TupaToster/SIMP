#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <immintrin.h>
#include <chrono>
#include <assert.h>

#define backgroundName "backgrnd.bmp" // File names for pictures
#define buranyaName "buranya.bmp" //

const char GlobalTimeFileName[] = "times.txt";

const int topCalcLimit = 1000; //amount of calculations per point
const unsigned int ExpCountLimit = 100; // total amount of program runs
const unsigned int x0 = 690; // initial pos of top left corner of overlay (pls check if overlay fits on background)
const unsigned int y0 = 500; // initial pos of top left corner of overlay (pls check if overlay fits on background)


void calcScr (unsigned char* background, unsigned char* buranya,
              const unsigned int overSizeX, const unsigned int overSizeY,
              const unsigned int backSizeX, const unsigned int backSizeY,
              const unsigned int x0, const unsigned int y0);