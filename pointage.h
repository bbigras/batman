#ifndef POINTAGE_H
#define POINTAGE_H

#include <sstream>

class Pointage {
public:
    Pointage(int noJoueur, SDL_Surface *scr) : points(0), rectPointage("images/rectPointage.bmp"), ecran(scr) {
        switch (noJoueur) {
            case 1:
                offset.x = 60;
                offset.y = 425;
                break;
            case 2:
                offset.x = 460;
                offset.y = 425;
                break;
        }

        changer = false;
    }

    bool Changer() {
        return changer;
    }

    int Points() {
        return points;
    }

    void Marquer(int marque) {
        points += marque;
        changer = true;
    }

    void Afficher() {
        TTF_Font *font = TTF_OpenFont("fonts/GUNSHIP2.TTF", 14);
        SDL_Color textColor = { 0, 0, 0 };
        ostringstream b;
        b << points;
        rectPointage.Affiche(offset.x - 4, offset.y - 2, ecran);
        SDL_Surface *message = TTF_RenderText_Solid(font, b.str().c_str(), textColor);
        SDL_BlitSurface(message, NULL, ecran, &offset);
        TTF_CloseFont(font);
    }

    void Importe(string p) {
        points = atoi(p.c_str());
    }

    string Exporte() {
        return Int_Vers_String(points);
        changer = false;
    }
private:
    int points;
    Image rectPointage;
    SDL_Rect offset;
    SDL_Surface *ecran;
    bool changer;
};
#endif
