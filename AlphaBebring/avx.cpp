#include "head.h"

inline void log (__m256i in, const char* endFmt) {

    int hex[8] = {};
    _mm256_maskstore_epi32 (hex, _mm256_set1_epi8 (0xFF), in);

    for (int i = 0; i < 8; i++) {

        printf ("%.2X%.2X%.2X%.2X", (unsigned char)hex[i], (unsigned char) (hex[i] >> 8), (unsigned char)(hex[i] >> 16), (unsigned char)(hex[i] >> 24));
    }

    printf (endFmt);
}

#define marker printf ("lol\n"); exit (0);

void calcScr (unsigned char* background, unsigned char* buranya,
              const unsigned int overSizeX, const unsigned int overSizeY,
              const unsigned int backSizeX, const unsigned int backSizeY,
              const unsigned int x0, const unsigned int y0) {

    int* overArray = (int*) buranya;
    int* backArray = (int*) background;

    const __m256i epi8Lo_epi16 = _mm256_set_epi8 (-1, 14, -1, 12, -1, 10, -1, 8, -1, 6, -1, 4, -1, 2, -1, 0, -1, 14, -1, 12, -1, 10, -1, 8, -1, 6, -1, 4, -1, 2, -1, 0);
    const __m256i epi8Hi_epi16 = _mm256_set_epi8 (-1, 15, -1, 13, -1, 11, -1, 9, -1, 7, -1, 5, -1, 3, -1, 1, -1, 15, -1, 13, -1, 11, -1, 9, -1, 7, -1, 5, -1, 3, -1, 1);

    const __m256i shr8 = _mm256_set_epi8 (-1, 15, -1, 13, -1, 11, -1, 9, -1, 7, -1, 5, -1, 3, -1, 1, -1, 15, -1, 13, -1, 11, -1, 9, -1, 7, -1, 5, -1, 3, -1, 1);
    const __m256i blender = _mm256_set_epi8 (0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0);

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

    unsigned int cnt = 0;
    unsigned int overIter = 0;
    unsigned int backIter = y0 * backSizeX + x0;

    for (;;) {

        if (overIter >= overSizeX * overSizeY) break;

        if (cnt >= overSizeX) {

            cnt = 0;
            backIter += backSizeX - overSizeX;
        }

        __m256i mask = masks[(cnt + 8) > overSizeX ? overSizeX - cnt : 8];

        __m256i overLo = _mm256_maskload_epi32 (overArray + overIter, mask);
        __m256i alpha = _mm256_shuffle_epi8 (overLo, _mm256_set_epi8 (-1, 15, -1, 15, -1, 11, -1, 11, -1, 7, -1, 7, -1, 3, -1, 3, -1, 15, -1, 15, -1, 11, -1, 11, -1, 7, -1, 7, -1, 3, -1, 3));
        __m256i overHi = _mm256_shuffle_epi8 (overLo, epi8Hi_epi16);
        overLo = _mm256_shuffle_epi8 (overLo, epi8Lo_epi16);

        __m256i backLo = _mm256_maskload_epi32 (backArray + backIter, mask);
        __m256i backHi = _mm256_shuffle_epi8 (backLo, epi8Hi_epi16);
        backLo = _mm256_shuffle_epi8 (backLo, epi8Lo_epi16);

        overHi = _mm256_mullo_epi16 (overHi, alpha);
        overLo = _mm256_mullo_epi16 (overLo, alpha);

        backHi = _mm256_mullo_epi16 (backHi, _mm256_sub_epi16 (_mm256_set1_epi16 (255), alpha));
        backLo = _mm256_mullo_epi16 (backLo, _mm256_sub_epi16 (_mm256_set1_epi16 (255), alpha));

        backLo = _mm256_add_epi16 (backLo, overLo);
        backHi = _mm256_add_epi16 (backHi, overHi);


        backLo = _mm256_shuffle_epi8 (backLo, shr8);
        backLo = _mm256_blendv_epi8 (backLo, backHi, blender);

        _mm256_maskstore_epi32 (backArray + backIter, mask, backLo);

        overIter = (cnt + 8) > overSizeX ? overIter + overSizeX - cnt : overIter + 8;
        backIter = (cnt + 8) > overSizeX ? backIter + overSizeX - cnt : backIter + 8;
        cnt = (cnt + 8) > overSizeX ? overSizeX : cnt + 8;
    }
}