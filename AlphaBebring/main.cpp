#include "head.h"

int main () {

    sf::Image backgroundImg, buranyaImg;

    if (!backgroundImg.loadFromFile (backgroundName)) {

        printf ("Error occured during background image loading\n");
        return 0;
    }

    if (!buranyaImg.loadFromFile (buranyaName)) {

        printf ("Error occured during loading of buranya\n");
        return 0;
    }

    unsigned int backSizeX = backgroundImg.getSize ().x;
    unsigned int backSizeY = backgroundImg.getSize ().y;
    unsigned int overSizeX = buranyaImg.getSize ().x;
    unsigned int overSizeY = buranyaImg.getSize ().y;

    unsigned char* background = (unsigned char*) calloc (backSizeX * backSizeY * 4, sizeof (unsigned char));
    assert (background != NULL);
    memcpy (background, (unsigned char*) backgroundImg.getPixelsPtr (), backSizeX * backSizeY * 4);

    unsigned char* overlay = (unsigned char*) calloc (overSizeX * overSizeY * 4, sizeof (unsigned char));
    assert (overlay != NULL);
    memcpy (overlay, (unsigned char*) buranyaImg.getPixelsPtr (), overSizeX * overSizeY * 4);

    sf::RenderWindow window (sf::VideoMode (backgroundImg.getSize ().x, backgroundImg.getSize ().y), "alphabebring");

    sf::Texture pixlArr;    // creates a texture
    pixlArr.create (backSizeX, backSizeY); // sets its dimensions to XSIZE * YSIZE

    while (window.isOpen ()) {

        memcpy (background, (unsigned char*) backgroundImg.getPixelsPtr (), backSizeX * backSizeY * 4);
        memcpy (overlay, (unsigned char*) buranyaImg.getPixelsPtr (), overSizeX * overSizeY * 4);

        clock_t time = clock ();    // gets initial time

        calcScr (background, overlay, overSizeX, overSizeY, backSizeX, backSizeY, 640, 190);

        time = (clock () - time) * 1000 / CLOCKS_PER_SEC; // measures work time

        printf ("%ld\n", time);

        pixlArr.update (background);      // updates texture with rgbquad array

        sf::Sprite sprite (pixlArr);    // creates sprite (хз зачем но так в примере сфмла написано)

        // window.clear ();
        window.draw (sprite);   // draws to window
        window.display ();  // displays it

        sf::Event event;        // Event var

        while (window.pollEvent(event)) {   // Check for being closed if there is an event (to close the window lol)

            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
}
