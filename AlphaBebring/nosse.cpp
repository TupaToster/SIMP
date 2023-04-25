#include "head.h"

void calcScr (unsigned char* background, unsigned char* buranya,
              unsigned int overSizeX, unsigned int overSizeY,
              unsigned int backSizeX, unsigned int backSizeY,
              const unsigned int x0, const unsigned int y0) {

    for (unsigned int y = overSizeY - 1; y > 0; y--) {

        for (unsigned int x = 0; x < overSizeX; x++) {

            unsigned int backIter = (y0 + y) * backSizeX * 4 + (x0 + x) * 4;
            unsigned int overIter = y * overSizeX * 4 + x * 4;
            background[backIter    ] = (buranya[overIter    ] * buranya[overIter + 3] + background[backIter    ] * (255 - buranya[overIter + 3])) >> 8;
            background[backIter + 1] = (buranya[overIter + 1] * buranya[overIter + 3] + background[backIter + 1] * (255 - buranya[overIter + 3])) >> 8;
            background[backIter + 2] = (buranya[overIter + 2] * buranya[overIter + 3] + background[backIter + 2] * (255 - buranya[overIter + 3])) >> 8;
            background[backIter + 3] = (buranya[overIter + 3] * buranya[overIter + 3] + background[backIter + 3] * (255 - buranya[overIter + 3])) >> 8;
        }
    }
}