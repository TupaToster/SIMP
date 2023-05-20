#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include <assert.h>
#include <chrono>
#include <string.h>

const char GlobalTimeFileName[] = "times.txt";

const unsigned int ScrSize = 1000;  // pixel size of screen (its square)
const int topCalcLimit = 100; //highest amount of calculations per point
const float maxDistSq = 100; // max distance squared from center for point calc

const unsigned char R0 = 49; //
const unsigned char G0 = 56; // zero iter color
const unsigned char B0 = 54; //
const unsigned char A0 = 50;//

const unsigned char R1 = 255;//
const unsigned char G1 = 10; // lim for max iter color (truly max iter color is also zero iter colot (for cool looks))
const unsigned char B1 = 10; //
const unsigned char A1 = 255;//
const unsigned int  ExpCountLimit = 100; // amount of runs to be performed (0 for unlimited)

const float Sigma = 0.8; // coeffitient for zooming/dezooming
extern float XLIML; // lower x starting lim
extern float XLIMH; // upper x starting lim
extern float YLIML; // lower y starting lim
extern float YLIMH;  // upper y starting lim
extern float Delta; // how much to move per key press (scales when zooming)

// uncomment to remove after-image and disable builtin alpha-blending in SFML
// #define NO_AFTER_IMAGE

void calcScr (unsigned char* rgba);