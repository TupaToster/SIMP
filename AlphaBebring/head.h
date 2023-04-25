#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <immintrin.h>
#include <chrono>
#include <assert.h>

#define backgroundName "backgrnd.bmp"
#define buranyaName "buranya.bmp"
// #define buranyaName "buranya_transparent.bmp"

const int topCalcLimit = 100; //highest amount of calculations per point

void calcScr (unsigned char* background, unsigned char* buranya,
              const unsigned int overSizeX, const unsigned int overSizeY,
              const unsigned int backSizeX, const unsigned int backSizeY,
              const unsigned int x0, const unsigned int y0);