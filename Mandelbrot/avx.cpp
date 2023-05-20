#include "head.h"

/// @brief Calculates whole screens specs and writes to rgba array
/// @param rgbaArray array of pixels in rgbquad format
void calcScr (unsigned char* rgbaArray) {

    const __m256 maxDistSq256 = _mm256_set1_ps (maxDistSq);   // max dist vector
    const float oneShift = (XLIMH - XLIML) / ScrSize;
    const __m256 shift = _mm256_set1_ps (8 * oneShift);  // delta for each iteration of pixels
    volatile __m256 x = _mm256_set_ps (XLIML + 7.0 * oneShift,
                              XLIML + 6.0 * oneShift,
                              XLIML + 5.0 * oneShift,
                              XLIML + 4.0 * oneShift,
                              XLIML + 3.0 * oneShift,
                              XLIML + 2.0 * oneShift,
                              XLIML + 1.0 * oneShift,
                              XLIML + 0.0 * oneShift); //x initial
    volatile __m256 y = _mm256_set1_ps (YLIMH);
    __m256 x00 = x; //saving initial x

    int cnt = 0;
    for (int scrIter = 0; scrIter < ScrSize * ScrSize * 4; scrIter += 32) { // iterates in 8 pixel wide parts

        if (cnt >= ScrSize) {   //  if end of line
            x = x00;    // reset x
            y = _mm256_sub_ps (y, _mm256_set1_ps (oneShift));// shift y
            cnt = 0;// zero cnt
        }

        __m256 x0 = x;  // saves start coords
        __m256 y0 = y;  // saves start coords

        volatile __m256i iRes = _mm256_setzero_si256 ();

        for (int i = 0 ; i < topCalcLimit; i++) {

            __m256 x2 = _mm256_mul_ps (x, x);   // squared
            __m256 y2 = _mm256_mul_ps (y, y);   // squared

            __m256 mask = _mm256_cmp_ps (_mm256_add_ps (x2, y2), maxDistSq256, _CMP_LE_OQ); // mask[i] = 0xFFFFFFFF if further calculation needed for point, 0x0 otherwise
            __m256i imask = _mm256_castps_si256 (mask); // the same but __m256i

            iRes = _mm256_add_epi32 (_mm256_and_si256 (imask, _mm256_set1_epi32 (1)), iRes);

            if (_mm256_movemask_epi8 (imask) == 0) break; // if no calcs needed (all == 0x0) then break

            y = _mm256_add_ps (_mm256_and_ps (mask, _mm256_sub_ps (     // condition to not calc ones that are zeroed in mask
                _mm256_add_ps (_mm256_mul_ps (_mm256_set1_ps (2.0), _mm256_mul_ps (x, y)), y0), // expression
                y)), y); // end of condition
            x = _mm256_add_ps (_mm256_and_ps (mask, _mm256_sub_ps (
                _mm256_add_ps (_mm256_sub_ps (x2, y2), x0),             // the same as 2 lines up
                x)), x);
        }

        x =_mm256_add_ps (x0, shift);   // shifts x cause its faster than recalculating every time
        y = y0; // resets y
        cnt += 8; // adds cnt

        iRes = _mm256_and_si256 (iRes, _mm256_xor_si256 (_mm256_set1_epi8 (0xFF), _mm256_cmpeq_epi32 (iRes, _mm256_set1_epi32 (topCalcLimit)))); //sets iRes[i] = 0 if iRes[i] == topCalcLimit

        int iResInt[8] = {0};
        _mm256_maskstore_epi32 ( (int*) iResInt, _mm256_set1_epi8 (0xFF), iRes);    // this part was not optimized because it would be very painful to do, and would do next to no effect on performance (unless topCalcLimit is set to around 10 or lower)

        for (int i = 0; i < 8; i++) {

            rgbaArray[scrIter + 4 * i + 0] = R0 + (iResInt[i] * (R1 - R0) / topCalcLimit);  // Formats i into color with alpha
            rgbaArray[scrIter + 4 * i + 1] = G0 + (iResInt[i] * (G1 - G0) / topCalcLimit);  // Formats i into color with alpha
            rgbaArray[scrIter + 4 * i + 2] = B0 + (iResInt[i] * (B1 - B0) / topCalcLimit);  // Formats i into color with alpha
            rgbaArray[scrIter + 4 * i + 3] = A0 + (iResInt[i] * (A1 - A0) / topCalcLimit);  // Formats i into color with alpha
        }
    }
}