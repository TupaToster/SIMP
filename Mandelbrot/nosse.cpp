#include "head.h"



/// @brief Calculates whole screens specs and writes to rgba array
/// @param rgba array of pixels in rgbquad format
void calcScr (unsigned char* rgba) {

    float y = YLIMH;
    float x = XLIML;
    float x00 = x;
    float shift = (XLIMH - XLIML) / ScrSize;

    int cnt = 0;
    for (int scrIter = 0; scrIter < ScrSize * ScrSize * 4; scrIter+=4) {

        if (cnt == ScrSize) {

            x = x00;
            y -= shift;
            cnt = 0;
        }

        // x = XLIML + float((scrIter / 4) % ScrSize) / ScrSize * (XLIMH - XLIML);
        // y = YLIMH - float(scrIter / (ScrSize * 4)) / ScrSize * (YLIMH - YLIML);


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
        y = y_0;
        cnt++;

        if (i == topCalcLimit) i = 0; // sets to default color if maxxed out the calculation limit
        rgba[scrIter    ] = r0 + (i * (r1 - r0) / topCalcLimit); //
        rgba[scrIter + 1] = g0 + (i * (g1 - g0) / topCalcLimit); // setting color calculated as a simple fader (could be made logarithmical for the lols)
        rgba[scrIter + 2] = b0 + (i * (b1 - b0) / topCalcLimit); //
        rgba[scrIter + 3] = a0 + (i * (a1 - a0) / topCalcLimit); //
    }
}