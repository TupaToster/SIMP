#include "head.h"

int main() {

    char progName[10] = "nosse";
    if (ExpCountLimit == 0) {

        printf("Please set amount of experiments in ExpCountLimit in head.h\n");
        return 0;
    }
    char command[150] = "";

    for (int vers = 0; vers < 2; vers++) {

        sprintf(command, "rm -rf %s", GlobalTimeFileName);

        system(command);

        sprintf(command, "./%s --test", progName);

        for (unsigned int i = 0; i < ExpCountLimit; i++) {

            system(command);
        }

        double sum = 0;
        FILE* inFile = fopen (GlobalTimeFileName, "rb");
        assert (inFile != NULL);
        double t = 0;

        for (int i = 0; i < ExpCountLimit; i++) {

            fscanf (inFile, "%lf ", &t);
            sum += t;
        }

        fclose (inFile);

        sum /= (double) ExpCountLimit;

        printf ("In version %d (0 - nosse, 1 - avx) avg run time in cpu clocks is : %.2lf\n", vers, sum);

        strcpy (progName, "avx");
    }
}