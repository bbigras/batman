#ifndef AUDIO_H
#define AUDIO_H

#include <iostream>
using namespace std;

class Son
{
public:
    Son(string fichier = "", bool jouer = false) {
        if (fichier != "")
            son = Mix_LoadWAV(fichier.c_str());
        else
            son = NULL;

        if (jouer)
            Jouer();
    }

    void Jouer() {
        Mix_PlayChannel(-1, son, 0);
    }

    ~Son() {
        Mix_FreeChunk(son);
    }
private:
    Mix_Chunk *son;
};

class Musique
{
public:
    Musique(string nomfichier, bool jouer = false) {
        Charger(nomfichier);
        if (jouer)
            Jouer();
    }

    void Charger(string nomfichier) {
        myMus = Mix_LoadMUS(nomfichier.c_str()); // nomfichier­.c_str()
    }

    Mix_Music* Jouer() {
        Mix_PlayMusic(myMus, 1);
        return myMus;
    }

    Mix_Music* Etat() {
        return myMus;
    }

    ~Musique() {
        Mix_FreeMusic(myMus);
    }
private:
    Mix_Music* myMus;
};
#endif
