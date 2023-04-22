#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <immintrin.h>
#include <chrono>
#include <assert.h>

#define backgroundName "TableResize.bmp"
#define buranyaName "buranya.bmp"

const int topCalcLimit = 100; //highest amount of calculations per point

void calcScr (unsigned char* background, unsigned char* buranya,
              unsigned int overSizeX, unsigned int overSizeY,
              unsigned int backSizeX, unsigned int backSizeY,
              const unsigned int x0 = 0, const unsigned int y0 = 0);