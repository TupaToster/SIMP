#include "head.h"

/// @brief Calculates whole screens specs and writes to rgba array
/// @param rgba array of pixels in rgbquad format
void calcScr (unsigned char* rgba) {

    volatile float x = XLIML;   // initial x
    volatile float y = YLIMH;   // initial y
    float x00 = x;  // initial x save
    float shift = (XLIMH - XLIML) / ScrSize; // width of a pixel

    int cnt = 0;    // counts how many pixels were processed
    for (int scrIter = 0; scrIter < ScrSize * ScrSize * 4; scrIter+=4) {

        if (cnt == ScrSize) {

            x = x00;
            y -= shift;     // sets x and y for a new line of pixels
            cnt = 0;
        }

        // x = XLIML + float((scrIter / 4) % ScrSize) / ScrSize * (XLIMH - XLIML);
        // y = YLIMH - float(scrIter / (ScrSize * 4)) / ScrSize * (YLIMH - YLIML);      // obsolete methods that calculated each pixels location independently


        float x_0 = x;  // saves initial pos
        float y_0 = y;  // saves initial pos

        int i = 0;  // iter count
        for (i = 0; i < topCalcLimit; i++) {

            float x_2 = x * x;  // x, y squared
            float y_2 = y * y;  // x, y squared

            if (x_2 + y_2 > maxDistSq) break;
            y = 2 * x * y + y_0;       // calculation of new iteration
            x = x_2 - y_2 + x_0;    // calculation of new iteration

        }

        x = x_0 + shift;
        y = y_0;        // switches to the next pixel
        cnt++;

        if (i == topCalcLimit) i = 0; // sets to default color if maxxed out the calculation limit
        rgba[scrIter    ] = R0 + (i * (R1 - R0) / topCalcLimit); //
        rgba[scrIter + 1] = G0 + (i * (G1 - G0) / topCalcLimit); // setting color calculated as a simple fader (could be made logarithmical for the lols)
        rgba[scrIter + 2] = B0 + (i * (B1 - B0) / topCalcLimit); //
        rgba[scrIter + 3] = A0 + (i * (A1 - A0) / topCalcLimit); //
    }
}