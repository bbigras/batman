#ifndef DEBUG_H
#define DEBUG_H

#include <sstream>

class Grille {
public:
    Grille(SDL_Surface *ecran) {
        TTF_Font *font = TTF_OpenFont("fonts/lazy.ttf", 28);
        SDL_Color textColor = { 150, 150, 150 };

        for (int j = 0; j < 13; j++) {
            ostringstream b;
            b << j;

            SDL_Rect offset;

            //Get the offsets
            offset.x = 5 * 30 + 30;
            offset.y = j * 30 + 30;

            SDL_Surface *message = TTF_RenderText_Solid(font, b.str().c_str(), textColor);

            SDL_BlitSurface(message, NULL, ecran, &offset);
        }

        TTF_CloseFont(font);
    }
private:
};

class Debogue {
public:
    Debogue(SDL_Surface *scr) : ecran(scr) {
        font = TTF_OpenFont("fonts/lazy.ttf", 28);
        if (font == NULL)
            cerr << "ne peut charger le font lazy.ttf!\n";
    }

    ~Debogue() {
        TTF_CloseFont(font);
    }

    int getpos(string id) {
        for (unsigned int i = 0; i < lespositions.size(); i++) {
            if (lespositions[i] == id)
                return i;
        }
        lespositions.push_back(id);
        return lespositions.size() - 1;
    }

    void Affiche(string id, string msg, PaletteCouleur &palette) {
        int position = getpos(id);
        msg = id + " : " + msg;

        SDL_Color textColor = { 150, 150, 150 };

        message = TTF_RenderText_Solid(font, msg.c_str(), textColor);

        SDL_Rect offset;

        offset.x = 400;
        offset.y = 1 + (position * 30);

        for (int i = 0; i < 10; i++)
            palette.Blanc()->Affiche(offset.x + (i*30), offset.y, ecran);

        SDL_BlitSurface(message, NULL, ecran, &offset);
    }
private:
    TTF_Font *font;
    SDL_Surface *message;

    vector<string> lespositions;
    SDL_Surface *ecran;
};
#endif
