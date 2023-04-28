#include "head.h"

void calcScr (unsigned char* background, unsigned char* buranya,
              unsigned int overSizeX, unsigned int overSizeY,
              unsigned int backSizeX, unsigned int backSizeY,
              const unsigned int x0, const unsigned int y0) {

    for (unsigned int y = overSizeY - 1; y > 0; y--) {

        for (unsigned int x = 0; x < overSizeX; x++) {

            volatile unsigned int backIter = (y0 + y) * backSizeX * 4 + (x0 + x) * 4;    // iterator for background image
            volatile unsigned int overIter = y * overSizeX * 4 + x * 4;      // iterator for overlay image
            volatile unsigned char alpha = buranya[overIter + 3];        // alpha value, so that it doesn't get changed
            volatile unsigned char screen[4] = {0};     // current pixel values for calculation, so that real ones don't change

            for (int i = 0; i < topCalcLimit; i++) {

                screen[0] = (buranya[overIter    ] * alpha + background[backIter    ] * (255 - alpha)) >> 8;    // red blending
                screen[1] = (buranya[overIter + 1] * alpha + background[backIter + 1] * (255 - alpha)) >> 8;    // blue blending
                screen[2] = (buranya[overIter + 2] * alpha + background[backIter + 2] * (255 - alpha)) >> 8;    // green blending
                screen[3] = (buranya[overIter + 3] * alpha + background[backIter + 3] * (255 - alpha)) >> 8;    // alpha blending
            }

            memcpy (background + backIter, (char*) screen, 4); // saving pixel values to background image
        }
    }
}