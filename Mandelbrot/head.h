#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include <chrono>

const unsigned int ScrSize = 1000;  // pixel size of screen (its square)
const int topCalcLimit = 100; //highest amount of calculations per point
const float maxDistSq = 100; // max distance squared from center for point calc

const unsigned char r0 = 49; //
const unsigned char g0 = 56; // zero iter color
const unsigned char b0 = 54; //
const unsigned char a0 = 50;//

const unsigned char r1 = 255;//
const unsigned char g1 = 10; // lim for max iter color (truly max iter color is also zero iter colot (for cool looks))
const unsigned char b1 = 10; //
const unsigned char a1 = 255;//
const unsigned int  expCountLimit = 0; // amount of runs to be performed (0 for unlimited)

const float sigma = 0.8; // coeffitient for zooming/dezooming
extern float XLIML; // lower x starting lim
extern float XLIMH; // upper x starting lim
extern float YLIML; // lower y starting lim
extern float YLIMH;  // upper y starting lim
extern float delta; // how much to move per key press (scales when zooming)

// uncomment to remove after-image and disable builtin alpha-blending in SFML
// #define NO_AFTER_IMAGE

void calcScr (unsigned char* rgba);