#include "head.h"

int main (int argc, char* argv[]) {

    sf::Image backgroundImg, buranyaImg;    // creates image classes from SFML for overlay and background

    if (!backgroundImg.loadFromFile (backgroundName)) {

        printf ("Error occured during background image loading\n");
        return -1;
    } // checks for error during loading background

    if (!buranyaImg.loadFromFile (buranyaName)) {

        printf ("Error occured during loading of buranya\n");
        return -1;
    } // checks for error during loading overlay

    unsigned int backSizeX = backgroundImg.getSize ().x; // Retrieves x and y size of background
    unsigned int backSizeY = backgroundImg.getSize ().y; // Retrieves x and y size of background

    unsigned int overSizeX = buranyaImg.getSize ().x;    // Retrieves x and y size of overlay
    unsigned int overSizeY = buranyaImg.getSize ().y;    // Retrieves x and y size of overlay

    unsigned char* background = (unsigned char*) calloc (backSizeX * backSizeY * 4, sizeof (unsigned char)); // creates a copy array that will be changed by function and displayed
    assert (background != NULL);

    unsigned char* overlay = (unsigned char*) calloc (overSizeX * overSizeY * 4, sizeof (unsigned char)); // creates a copy array that will be changed by function and displayed
    assert (overlay != NULL);

    int time[ExpCountLimit > 0 ? ExpCountLimit : 1] = {0};

    if (argc == 2 and strcmp (argv[1], "--test") == 0) {

        FILE* outFile = fopen (GlobalTimeFileName, "ab");
        assert (outFile != NULL);

        memcpy (background, (unsigned char*) backgroundImg.getPixelsPtr (), backSizeX * backSizeY * 4); // copies image to proxy array
        memcpy (overlay, (unsigned char*) buranyaImg.getPixelsPtr (), overSizeX * overSizeY * 4); // -//-

        time[0] = clock ();    // gets initial time

        calcScr (background, overlay, overSizeX, overSizeY, backSizeX, backSizeY, x0, y0);

        time[0] = (clock () - time[0]); // measures cpu time of execution in clocks (thats way cool)

        fprintf (outFile, "%d\n", time[0]);

        fclose (outFile);

        return 0;
    }

    sf::RenderWindow window (sf::VideoMode (backgroundImg.getSize ().x, backgroundImg.getSize ().y), "alphabebring");
    // creates a window of size of background

    sf::Texture pixlArr;    // creates a texture
    pixlArr.create (backSizeX, backSizeY); // sets its dimensions to dimensions of background

    int i = 0;
    while (window.isOpen ()) {

        if (ExpCountLimit != 0 and i >= ExpCountLimit) break;

        memcpy (background, (unsigned char*) backgroundImg.getPixelsPtr (), backSizeX * backSizeY * 4); // copies image to proxy array
        memcpy (overlay, (unsigned char*) buranyaImg.getPixelsPtr (), overSizeX * overSizeY * 4); // -//-

        time[i] = clock ();    // gets initial time

        calcScr (background, overlay, overSizeX, overSizeY, backSizeX, backSizeY, x0, y0);

        time[i] = (clock () - time[i]) * 1000 / CLOCKS_PER_SEC; // measures work cpu time in msec

        printf ("%ld\n", time[i]); // prints result
        i = ExpCountLimit > 0 ? i + 1 : 0;

        pixlArr.update (background);      // updates texture with rgbquad array

        sf::Sprite sprite (pixlArr);    // creates sprite (хз зачем но так в примере сфмла написано)

        window.draw (sprite);   // draws to window
        window.display ();  // displays it

        sf::Event event;        // Event var

        while (window.pollEvent(event)) {   // Check for being closed if there is an event (to close the window lol)

            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    window.close ();

    if (ExpCountLimit == 0) return 0;

    for (int i = 1; i <= ExpCountLimit; i++) {  // Calculates and prints avg time over ExpCountLimit runs

        if (i == ExpCountLimit or time[i] == 0) {

            double res = (double) time[0];
            res /= (double) i;
            printf ("Resulting avg time across %d runs is : %lg\n", i, res);
            break;
        }
        time[0] += time[i];
    }
}
