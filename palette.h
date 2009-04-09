#ifndef PALETTE_H
#define PALETTE_H

#include "image.h"

class PaletteCouleur
{
public:
    PaletteCouleur() {
        drouge = new Image("images/bombeRouge.PNG");
        djaune = new Image("images/bombeJaune.PNG");
        dvert = new Image("images/bombeVert.PNG");
        dbleu = new Image("images/bombeBleu.PNG");
        jaune = new Image("images/carreJaune.png");
        rouge = new Image("images/carreRouge.png");
        bleu = new Image("images/carreBleu.png");
        vert = new Image("images/carreVert.png");

        blanc = new Image("images/carreBlanc.JPG");
        noir = new Image("images/carreNoir.gif");

        debogue = new Image("images/carreDebogue.gif");

        tableau = new Image("images/tab.jpg");
    }

    ~PaletteCouleur() {
        delete bleu;
        delete vert;
        delete jaune;
        delete rouge;

        delete drouge;
        delete dvert;
        delete djaune;
        delete dbleu;

        delete blanc;
        delete noir;

        delete debogue;
        delete tableau;
    }

    Image *Bleu() {
        return bleu;
    }

    Image *Vert() {
        return vert;
    }

    Image *Rouge() {
        return rouge;
    }

    Image *Jaune() {
        return jaune;
    }

    Image *DBleu() {
        return dbleu;
    }

    Image *DVert() {
        return dvert;
    }

    Image *DRouge() {
        return drouge;
    }

    Image *DJaune() {
        return djaune;
    }

    Image *Noir() {
        return noir;
    }

    Image *Blanc() {
        return blanc;
    }

    Image *Debogue() {
        return debogue;
    }

    Image *Tableau() {
        return tableau;
    }
private:
    Image *rouge;
    Image *jaune;
    Image *drouge;
    Image *djaune;
    Image *dvert;
    Image *dbleu;
    Image *vert;
    Image *bleu;
    Image *noir;
    Image *debogue;
    Image *blanc;
    Image *tableau;
};
#endif
