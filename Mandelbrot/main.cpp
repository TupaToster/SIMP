#include "head.h"

float XLIML = -2.0; // lower x starting lim
float XLIMH = 0.0; // upper x starting lim
float YLIML = -1.0; // lower y starting lim
float YLIMH = 1.0;  // upper y starting lim
float Delta = 0.1; // how much to move per key press (scales when zooming)

#define NO_AFTER_IMAGE

int main (int argc, char* argv[]) {

    unsigned char rgbaArray[ScrSize * ScrSize * 4] = {0};    // Array of rgb quads
    int time[ExpCountLimit > 0 ? ExpCountLimit : 1] = {0}; // array for storing times of each exp

    if (argc == 2 and strcmp (argv[1], "--test") == 0) {

        FILE* outFile = fopen (GlobalTimeFileName, "ab");
        assert (outFile != NULL);

        time[0] = clock ();    // gets initial time

        calcScr (rgbaArray); // calculates screen for display

        time[0] = (clock () - time[0]); // measures cpu time of execution in clocks (thats way cool)

        fprintf (outFile, "%d\n", time[0]);

        fclose (outFile);

        return 0;
    }

    sf::RenderWindow window (sf::VideoMode (ScrSize, ScrSize), "mndlbrt"); // creates a window of ScrSize

    sf::Texture pixlArr;    // creates a texture
    pixlArr.create (ScrSize, ScrSize); // sets its dimensions to ScrSize * ScrSize

    int i = 0;  // experiment counter
    // if ExpCountLimit is zero, than an unlimited amount of runs can be performed
    while (window.isOpen ()) {

        if (ExpCountLimit != 0 and i == ExpCountLimit) break;

        time[i] = clock ();    // gets initial time

        calcScr (rgbaArray); // calculates screen for display

        time[i] = (clock () - time[i]) * 1000 / CLOCKS_PER_SEC; // measures cpu time of execution in msec

        printf ("%ld\n", time[i]); // prints time of current calculation
        i = ExpCountLimit > 0 ? i + 1 : 0;

        pixlArr.update (rgbaArray);      // updates texture with rgbquad array

        sf::Sprite sprite (pixlArr);    // creates sprite (хз зачем но так в примере сфмла написано)

        #ifdef NO_AFTER_IMAGE       // Here I observed effect of afterimage, that can be explained with me calculating alpha for each pixel, and SFML having built in alpha-blending
        window.clear ();
        #endif // if NO_AFTER_IMAGE is defined then the program will clean the screen before displaying new image
        // This also effects colors, that, if window is not cleared each time, will become brighter over time because of blending with themselves from previous calculations
        window.draw (sprite);   // draws to window
        window.display ();  // displays it


        // This block lets user interract with image, moving it in all directions and zooming in and out
        // zooming coef can be changed in head.h
        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Left)) {

            XLIMH -= Delta;
            XLIML -= Delta;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Right)) {

            XLIML += Delta;
            XLIMH += Delta;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Up)) {

            YLIML += Delta;
            YLIMH += Delta;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Down)) {

            YLIML -= Delta;
            YLIMH -= Delta;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Equal)) {    // Zoom

            float shift = (XLIMH - XLIML - Sigma * (XLIMH - XLIML)) / 2;
            XLIML += shift;
            XLIMH -= shift;
            YLIMH -= shift;
            YLIML += shift;
            Delta *= Sigma;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Dash)) {     // UnZoom

            float shift = ((XLIMH - XLIML) / Sigma - XLIMH + XLIML) / 2;
            XLIML -= shift;
            XLIMH += shift;
            YLIMH += shift;
            YLIML -= shift;
            Delta /= Sigma;
        }

        sf::Event event;        // Event var

        while (window.pollEvent(event)) {   // Check for being closed if there is an event (to close the window lol)

            if (event.type == sf::Event::Closed)
                window.close ();
        }
    }

    if (ExpCountLimit == 0) return 0; // checks if no ExpCountLimit set, to avoid segfault

    window.close (); // closes the window

    // calculates avg run time across ExpCountLimit
    for (int i = 1; i <= ExpCountLimit; i++) {

        if (i == ExpCountLimit or time[i] == 0) {

            double res = (double) time[0];
            res /= (double) i;
            printf ("Resulting avg time across %d runs is : %lg\n", i, res);
            break;
        }
        time[0] += time[i];
    }
}
