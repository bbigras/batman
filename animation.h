#ifndef ANIMATION_H
#define ANIMATION_H

#include "image.h"

class Animation
{
public:
    Animation() {
        explosion[1].Charger("ex1.gif");
        explosion[2].Charger("ex2.gif");
        explosion[3].Charger("ex3.gif");
        explosion[4].Charger("ex4.gif");
        explosion[5].Charger("ex5.gif");
        explosion[6].Charger("ex6.gif");
        explosion[7].Charger("ex7.gif");
        explosion[8].Charger("ex8.gif");
        explosion[9].Charger("ex9.gif");
        explosion[10].Charger("ex10.gif");
        explosion[11].Charger("ex11.gif");
        explosion[12].Charger("ex12.gif");
        explosion[13].Charger("ex13.gif");
        explosion[14].Charger("ex14.gif");
        explosion[15].Charger("ex15.gif");
        explosion[16].Charger("ex16.gif");
    }

    void Afficher(int x, int y, SDL_Surface* screen, int no) {
        explosion[no].Affiche(x, y, screen);
    }

private:
    Image explosion[16];
};
#endif
