#ifndef BLOC_H
#define BLOC_H

#include <iostream>
#include <vector>

#include "image.h"
#include "palette.h"
#include "pile.h"
#include "outils.h"

class Bloc2 {
public:
    Bloc2(int x1, int y1) : x(x1), y(y1) {
        fait = false;
    }

    int X() {
        return x;
    }
    int Y() {
        return y;
    }
private:
    int x;
    int y;
    bool fait;
};

class Bloc {
public:

    Bloc() {}
    Bloc(Image *couleur, int t, bool tab_d, int c = 0) : case_x(3), case_y(0), type(t), compteur(c), pointeur(couleur), tab_droite(tab_d)  {}

    bool Afficher(SDL_Surface *ecran, PaletteCouleur &lapalette) {
        pointeur->Affiche(30*(case_x + 1) + (tab_droite ? 400 : 0), 30*(case_y + 1), ecran);

        if (pointeur != lapalette.Blanc() && pointeur != lapalette.Noir() && compteur > 0) {
            TTF_Font *font = TTF_OpenFont("fonts/GUNSHIP2.TTF", 14);
            SDL_Color textColor = { 0, 0, 0 };

            ostringstream b;
            b << compteur;

            SDL_Rect offset;

            offset.x = 30 * (case_x + 1) + (tab_droite ? 400 : 0);
            offset.y = 30 * (case_y + 1);

            SDL_Surface *message = TTF_RenderText_Solid(font, b.str().c_str(), textColor);
            SDL_BlitSurface(message, NULL, ecran, &offset);
            TTF_CloseFont(font);
        }
        return 0;
    }

    Bloc& operator=(const Bloc& v) {
        case_x = v.case_x;
        case_y = v.case_y;
        type = v.type;
        compteur = v.compteur;
        pointeur = v.pointeur;
        tab_droite = v.tab_droite;
        return *this;
    }

    void plusCompteur(int c) {
        compteur += c;
    }

    int Compteur() {
        return compteur;
    }

    void Compteur(int c) {
        compteur = c;
    }

    Image *Couleur() {
        return pointeur;
    }

    void SetPosition(unsigned int x, unsigned int y) {
        case_x = x;
        case_y = y;
    }

    unsigned int X() {
        return case_x;
    }

    void X(unsigned int x) {
        case_x = x;
    }

    void Y(int y) {
        case_y = y;
    }

    int Y() {
        return case_y;
    }

    unsigned int Type() {
        return type;
    }

    void Type(unsigned int t) {
        type = t;
    }
private:
    int case_x;
    int case_y;
    int type; // 0:Vide; 1:Normal, 2:Destructeur; 3:Diamant
    int compteur;
    Image *pointeur;

    bool tab_droite;
};

class BlocTombeur {
public:
    BlocTombeur() {}

    BlocTombeur(int tab, Image *couleur, int t, Image *couleur2, int t2) : pointeur(couleur), pointeur2(couleur2), orientation(2), type(t), type2(t2), x(3), y(30), le_tableau(tab) {}

    BlocTombeur Pila(Pile *la_pile, int pile_compteur, PaletteCouleur &lapalette, int tab) {
        le_tableau = tab;
        Image* img1;
        Image* img2;

        int type1;
        ElemPile bleh = la_pile->Donne(pile_compteur);

        switch (bleh.Bloc1()) {
            case 1:
                img1 = lapalette.Bleu();
                type1 = 1;
                break;
            case 2:
                img1 = lapalette.Vert();
                type1 = 1;
                break;
            case 3:
                img1 = lapalette.Rouge();
                type1 = 1;
                break;
            case 4:
                img1 = lapalette.Jaune();
                type1 = 1;
                break;
            case 5:
                img1 = lapalette.DBleu();
                type1 = 2;
                break;
            case 6:
                img1 = lapalette.DVert();
                type1 = 2;
                break;
            case 7:
                img1 = lapalette.DRouge();
                type1 = 2;
                break;
            case 8:
                img1 = lapalette.DJaune();
                type1 = 2;
                break;
        }

        int type2;
        switch (bleh.Bloc2()) {
            case 1:
                img2 = lapalette.Bleu();
                type2 = 1;
                break;
            case 2:
                img2 = lapalette.Vert();
                type2 = 1;
                break;
            case 3:
                img2 = lapalette.Rouge();
                type2 = 1;
                break;
            case 4:
                img2 = lapalette.Jaune();
                type2 = 1;
                break;
            case 5:
                img2 = lapalette.DBleu();
                type2 = 2;
                break;
            case 6:
                img2 = lapalette.DVert();
                type2 = 2;
                break;
            case 7:
                img2 = lapalette.DRouge();
                type2 = 2;
                break;
            case 8:
                img2 = lapalette.DJaune();
                type2 = 2;
                break;
        }
        BlocTombeur un_bloc(le_tableau, img1, bleh.Type1(), img2, bleh.Type2());
        return un_bloc;
    }

    Bloc UnBloc() {
        Bloc ptr(pointeur, type, (le_tableau == 2 ? true : false));
        ptr.X(X());
        ptr.Y(Y2());
        return ptr;
    }

    Bloc UnBloc2() {
        Bloc ptr(pointeur2, type2, (le_tableau == 2 ? true : false));
        ptr.X(X9());
        ptr.Y(Y9());
        return ptr;
    }

    bool Afficher(SDL_Surface *ecran) {
        pointeur->Affiche(30*(x + 1) + (le_tableau == 1 ? 0 : 400), y, ecran);
        switch (orientation) {
            case 0:
                pointeur2->Affiche(30*(x + 1) + (le_tableau == 1 ? 0 : 400), y - 30, ecran);
                break;
            case 1:
                pointeur2->Affiche(30*(x + 1) + 30 + (le_tableau == 1 ? 0 : 400), y, ecran);
                break;
            case 2:
                pointeur2->Affiche(30*(x + 1) + (le_tableau == 1 ? 0 : 400), y + 30, ecran);
                break;
            case 3:
                pointeur2->Affiche(30*(x + 1) - 30 + (le_tableau == 1 ? 0 : 400), y, ecran);
                break;
        }

        Calcul();
        return 0;
    }

    bool Afficher2(SDL_Surface *ecran) {
        pointeur->Affiche(220 + (le_tableau == 2 ? 170 : 0), 20, ecran);
        switch (orientation) {
            case 0:
                pointeur2->Affiche(220 + (le_tableau == 2 ? 170 : 0), 20 - 30, ecran);
                break;
            case 1:
                pointeur2->Affiche(220 + 30 + (le_tableau == 2 ? 170 : 0), 20, ecran);
                break;
            case 2:
                pointeur2->Affiche(220 + (le_tableau == 2 ? 170 : 0), 20 + 30, ecran);
                break;
            case 3:
                pointeur2->Affiche(220 - 30 + (le_tableau == 2 ? 170 : 0), 20, ecran);
                break;
        }

        //Calcul();
        return 0;
    }

    int Y9() {
        if (orientation == 3 || orientation == 1)
            return Y2();
        else if (orientation == 0)
            return Y2() - 1;
        else
            return Y2() + 1;
    }

    int X9() {
        if (orientation == 0 || orientation == 2)
            return X();
        else if (orientation == 3)
            return X() - 1;
        else
            return X() + 1;
    }

    Image *Couleur() {
        return pointeur;
    }

    Image *Couleur2() {
        return pointeur2;
    }

    unsigned int X() {
        return x;
    }

    unsigned int Y() {
        return y;
    }

    unsigned int Y4() {
        return orientation == 2 ? y + 30 : y;
    }

    unsigned int Y2() {
        return int(y / 30) - 1;
    }

    unsigned int Type() {
        return type;
    }

    unsigned int Type2() {
        return type2;
    }

    void Deplacer(int direction, SDL_Surface *ecran, PaletteCouleur &palette, int pixels = 1) {
        palette.Blanc()->Affiche(30*(x + 1) + (le_tableau == 1 ? 0 : 400), y, ecran);

        switch (orientation) {
            case 0:
                palette.Blanc()->Affiche(30*(x + 1) + (le_tableau == 1 ? 0 : 400), y - 30, ecran);
                break;
            case 1:
                palette.Blanc()->Affiche(30*(x + 1) + 30 + (le_tableau == 1 ? 0 : 400), y, ecran);
                break;
            case 2:
                palette.Blanc()->Affiche(30*(x + 1) + (le_tableau == 1 ? 0 : 400), y + 30, ecran);
                break;
            case 3:
                palette.Blanc()->Affiche(30*(x + 1) - 30 + (le_tableau == 1 ? 0 : 400), y, ecran);
                break;
        }
        switch (direction) {
            case 1:
                x++;
                break;
            case 2:
                x--;
                break;
            case 3:
                y--;
                break;
            case 4:
                y += pixels;
                break;
            case 5:
                y += 30;
                break;
        }
        Afficher(ecran);
    }

    // Direction : 0 droit, 1 gauche
    void Tourne(SDL_Surface *ecran, PaletteCouleur &palette, int direction = 0) {
        switch (orientation) {
            case 0:
                palette.Blanc()->Affiche(30*(x + 1) + (le_tableau == 1 ? 0 : 400), y - 30, ecran);
                break;
            case 1:
                palette.Blanc()->Affiche(30*(x + 1) + 30 + (le_tableau == 1 ? 0 : 400), y, ecran);
                break;
            case 2:
                palette.Blanc()->Affiche(30*(x + 1) + (le_tableau == 1 ? 0 : 400), y + 30, ecran);
                break;
            case 3:
                palette.Blanc()->Affiche(30*(x + 1) - 30 + (le_tableau == 1 ? 0 : 400), y, ecran);
                break;
        }

        if (direction == 0) {
            if (orientation < 3)
                orientation++;
            else
                orientation = 0;
        } else {
            if (orientation > 0)
                orientation--;
            else
                orientation = 3;
        }

        Afficher(ecran);
    }

    int Orientation() {
        return orientation;
    }

    void Calcul() {
        haut = int((y - 30) / 30);
        bas = (int((y - 30) / 30)) + 1;

        if (orientation == 0) {
            haut--;
            if (y % 30 == 0)
                bas--;
        }

        if (orientation == 2 && y % 30)
            bas++;

        // hack
        if (y % 30 == 0 && (orientation == 1 || orientation == 3))
            bas = haut;
    }

    int Haut() {
        return haut;
    }

    int Bas() {
        return bas;
    }

    int Gauche() {
        return orientation == 3 ? x - 1 : x;
    }

    int Droite() {
        return orientation == 1 ? x + 1 : x;
    }

    bool Aligne() {
        return y % 30 == 0;
    }

    string ExporteTombeur(PaletteCouleur &lapalette) {
        int p1, p2;

        if (pointeur == lapalette.Bleu())
            p1 = 1;
        else if (pointeur == lapalette.Vert())
            p1 = 2;
        else if (pointeur == lapalette.Rouge())
            p1 = 3;
        else if (pointeur == lapalette.Jaune())
            p1 = 4;
        else if (pointeur == lapalette.DBleu())
            p1 = 5;
        else if (pointeur == lapalette.DVert())
            p1 = 6;
        else if (pointeur == lapalette.DRouge())
            p1 = 7;
        else if (pointeur == lapalette.DJaune())
            p1 = 8;

        if (pointeur2 == lapalette.Bleu())
            p2 = 1;
        else if (pointeur2 == lapalette.Vert())
            p2 = 2;
        else if (pointeur2 == lapalette.Rouge())
            p2 = 3;
        else if (pointeur2 == lapalette.Jaune())
            p2 = 4;
        else if (pointeur2 == lapalette.DBleu())
            p2 = 5;
        else if (pointeur2 == lapalette.DVert())
            p2 = 6;
        else if (pointeur2 == lapalette.DRouge())
            p2 = 7;
        else if (pointeur2 == lapalette.DJaune())
            p2 = 8;

        return Int_Vers_String(p1) + Int_Vers_String(p2) + Int_Vers_String(type) + Int_Vers_String(type2) + Int_Vers_String(orientation) + Int_Vers_String(x) + Int_Vers_String(y);
    }

    void Importe(string code, PaletteCouleur &lapalette) {
        int p1 = atoi(code.substr(0, 1).c_str());
        int p2 = atoi(code.substr(1, 1).c_str());

        type = atoi(code.substr(2, 1).c_str());
        type2 = atoi(code.substr(3, 1).c_str());

        orientation = atoi(code.substr(4, 1).c_str());

        x = atoi(code.substr(5, 1).c_str());
        y = atoi(code.substr(6).c_str());

        le_tableau = 2;

        switch (p1) {
            case 1:
                pointeur = lapalette.Bleu();
                type = 1;
                break;
            case 2:
                pointeur = lapalette.Vert();
                type = 1;
                break;
            case 3:
                pointeur = lapalette.Rouge();
                type = 1;
                break;
            case 4:
                pointeur = lapalette.Jaune();
                type = 1;
                break;
            case 5:
                pointeur = lapalette.DBleu();
                type = 2;
                break;
            case 6:
                pointeur = lapalette.DVert();
                type = 2;
                break;
            case 7:
                pointeur = lapalette.DRouge();
                type = 2;
                break;
            case 8:
                pointeur = lapalette.DJaune();
                type = 2;
                break;
        }

        switch (p2) {
            case 1:
                pointeur2 = lapalette.Bleu();
                type2 = 1;
                break;
            case 2:
                pointeur2 = lapalette.Vert();
                type2 = 1;
                break;
            case 3:
                pointeur2 = lapalette.Rouge();
                type2 = 1;
                break;
            case 4:
                pointeur2 = lapalette.Jaune();
                type2 = 1;
                break;
            case 5:
                pointeur2 = lapalette.DBleu();
                type2 = 2;
                break;
            case 6:
                pointeur2 = lapalette.DVert();
                type2 = 2;
                break;
            case 7:
                pointeur2 = lapalette.DRouge();
                type2 = 2;
                break;
            case 8:
                pointeur2 = lapalette.DJaune();
                type2 = 2;
                break;
        }
    }
private:
    Image *pointeur;
    Image *pointeur2;
    int orientation; // 0 b, 1 g, 2 h, 3 d
    int type;
    int type2;
    int x; // 0 à 5
    int y; // 0 à 360
    int haut;
    int bas;
    int le_tableau;
};
#endif
