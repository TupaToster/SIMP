#include "head.h"

float XLIML = -2.0; // lower x starting lim
float XLIMH = 0.0; // upper x starting lim
float YLIML = -1.0; // lower y starting lim
float YLIMH = 1.0;  // upper y starting lim
float delta = 0.1; // how much to move per key press (scales when zooming)

int main () {

    sf::RenderWindow window (sf::VideoMode (ScrSize, ScrSize), "mndlbrt");

    sf::Texture pixlArr;    // creates a texture
    pixlArr.create (ScrSize, ScrSize); // sets its dimensions to ScrSize * ScrSize

    unsigned char rgbaArray[ScrSize * ScrSize * 4] = {0};    // Array of rgb quads

    while (window.isOpen ()) {

        clock_t time = clock ();    // gets initial time

        calcScr (rgbaArray);

        time = (clock () - time) * 1000 / CLOCKS_PER_SEC; // measures work time

        printf ("%ld\n", time);

        pixlArr.update (rgbaArray);      // updates texture with rgbquad array

        sf::Sprite sprite (pixlArr);    // creates sprite (хз зачем но так в примере сфмла написано)

        #ifdef NO_AFTER_IMAGE
        window.clear ();
        #endif
        window.draw (sprite);   // draws to window
        window.display ();  // displays it

        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Left)) {

            XLIMH -= delta;
            XLIML -= delta;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Right)) {

            XLIML += delta;
            XLIMH += delta;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Up)) {

            YLIML += delta;
            YLIMH += delta;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Down)) {

            YLIML -= delta;
            YLIMH -= delta;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Equal)) {    // Zoom

            float shift = (XLIMH - XLIML - sigma * (XLIMH - XLIML)) / 2;
            XLIML += shift;
            XLIMH -= shift;
            YLIMH -= shift;
            YLIML += shift;
            delta *= sigma;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Dash)) {     // UnZoom

            float shift = ((XLIMH - XLIML) / sigma - XLIMH + XLIML) / 2;
            XLIML -= shift;
            XLIMH += shift;
            YLIMH += shift;
            YLIML -= shift;
            delta /= sigma;
        }

        sf::Event event;        // Event var

        while (window.pollEvent(event)) {   // Check for being closed if there is an event (to close the window lol)

            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
}
