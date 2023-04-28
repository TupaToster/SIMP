#include "head.h"

void calcScr (unsigned char* background, unsigned char* buranya,
              const unsigned int overSizeX, const unsigned int overSizeY,
              const unsigned int backSizeX, const unsigned int backSizeY,
              const unsigned int x0, const unsigned int y0) {

    int* overArray = (int*) buranya;    // int ptr for overlay (1 int is 1 pixel)
    int* backArray = (int*) background; // int ptr for background

    const __m256i epi8Lo_epi16 = _mm256_set_epi8 (-1, 14, -1, 12, -1, 10, -1, 8, -1, 6, -1, 4, -1, 2, -1, 0, -1, 14, -1, 12, -1, 10, -1, 8, -1, 6, -1, 4, -1, 2, -1, 0);
    // mask to convert lower part of each epi16 to epi16
    const __m256i epi8Hi_epi16 = _mm256_set_epi8 (-1, 15, -1, 13, -1, 11, -1, 9, -1, 7, -1, 5, -1, 3, -1, 1, -1, 15, -1, 13, -1, 11, -1, 9, -1, 7, -1, 5, -1, 3, -1, 1);
    // mask to convert higher part of each epi16 to epi16

    const __m256i shr8 = _mm256_set_epi8 (-1, 15, -1, 13, -1, 11, -1, 9, -1, 7, -1, 5, -1, 3, -1, 1, -1, 15, -1, 13, -1, 11, -1, 9, -1, 7, -1, 5, -1, 3, -1, 1);
    //mask to shift right 8 bits
    const __m256i blender = _mm256_set_epi8 (0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0);
    // mask to blend 2 epi8 numbers (a, b) with each first from b, and each second from a

    // masks to handle image size not being %8
    const __m256i masks[9] = {
                               _mm256_setzero_si256 (),
                               _mm256_set_epi32 (0, 0, 0, 0, 0, 0, 0, 0xFFFFFFFF),
                               _mm256_set_epi32 (0, 0, 0, 0, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF),
                               _mm256_set_epi32 (0, 0, 0, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF),
                               _mm256_set_epi32 (0, 0, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF),
                               _mm256_set_epi32 (0, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF),
                               _mm256_set_epi32 (0, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF),
                               _mm256_set_epi32 (0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF),
                               _mm256_set1_epi32 (0xFFFFFFFF),
                               };

    unsigned int cnt = 0;   // counter of pixels in current line
    unsigned int overIter = 0; // overlay iter
    unsigned int backIter = y0 * backSizeX + x0; // background iter

    for (;;) {

        if (overIter >= overSizeX * overSizeY) break; // if all overlay was blended, break

        if (cnt >= overSizeX) { // go to new line in background

            cnt = 0;
            backIter += backSizeX - overSizeX;
        }

        __m256i mask = masks[(cnt + 8) > overSizeX ? overSizeX - cnt : 8]; // current mask for given cnt
        __m256i over = _mm256_maskload_epi32 (overArray + overIter, mask); // 8 pixels (or less if mask != masks[8]) from overlay
        __m256i back = _mm256_maskload_epi32 (backArray + backIter, mask); // -//- from background
        volatile __m256i alpha = _mm256_shuffle_epi8 (over, _mm256_set_epi8 (-1, 15, -1, 15, -1, 11, -1, 11, -1, 7, -1, 7, -1, 3, -1, 3, -1, 15, -1, 15, -1, 11, -1, 11, -1, 7, -1, 7, -1, 3, -1, 3));
        // alpha from overlay
        __m256i backLo = _mm256_setzero_si256 ();  //backLo is defined outside of loop because it will be used as storage for an output value

        for (int i = 0; i < topCalcLimit; i++) {

            __m256i overLo = _mm256_shuffle_epi8 (over, epi8Lo_epi16); // lower half of each epi16 over
            __m256i overHi = _mm256_shuffle_epi8 (over, epi8Hi_epi16); // higher half of each epi16 over

                             backLo = _mm256_shuffle_epi8 (back, epi8Lo_epi16); // lower half of each epi16 back
            __m256i backHi = _mm256_shuffle_epi8 (back, epi8Hi_epi16); // higher half of each epi16 back

            overHi = _mm256_mullo_epi16 (overHi, alpha); // multiply over pixel vals by alpha
            overLo = _mm256_mullo_epi16 (overLo, alpha);

            backHi = _mm256_mullo_epi16 (backHi, _mm256_sub_epi16 (_mm256_set1_epi16 (255), alpha)); // multiply back pixel vals by 255 - alpha
            backLo = _mm256_mullo_epi16 (backLo, _mm256_sub_epi16 (_mm256_set1_epi16 (255), alpha));

            backLo = _mm256_add_epi16 (backLo, overLo); // sum up over and back
            backHi = _mm256_add_epi16 (backHi, overHi);


            backLo = _mm256_shuffle_epi8 (backLo, shr8); // shift right backLo
            backLo = _mm256_blendv_epi8 (backLo, backHi, blender); // blend backLo and backHi
        }

        _mm256_maskstore_epi32 (backArray + backIter, mask, backLo); // store calculated pixels in array

        overIter = (cnt + 8) > overSizeX ? overIter + overSizeX - cnt : overIter + 8; // shift over and back depending on how many pixels left till line os over
        backIter = (cnt + 8) > overSizeX ? backIter + overSizeX - cnt : backIter + 8; // shift over and back depending on how many pixels left till line os over
        cnt = (cnt + 8) > overSizeX ? overSizeX : cnt + 8; // shift cnt
    }
}