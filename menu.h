#ifndef MENU_H
#define MENU_H

#include <vector>

class Menu {
public:
    void Afficher(SDL_Surface *ecran) {
        TTF_Font *font = TTF_OpenFont("fonts/GUNSHIP2.TTF", 44);
        SDL_Color textColor = { 0, 0, 0 };

        string b = "bleh";
        SDL_Rect offset;

        offset.x = 200;
        offset.y = 30;
        //ostringstream b;
        //b << points;
        //rectPointage.Affiche(offset.x - 4, offset.y - 2, ecran);
        SDL_Surface *message = TTF_RenderText_Solid(font, b.c_str(), textColor);
        SDL_BlitSurface(message, NULL, ecran, &offset);
        TTF_CloseFont(font);
    }

    void menuOptionsAfficher(SDL_Surface *ecran, vector<int>& lesOptions, int SCREEN_WIDTH, int SCREEN_HEIGHT, int SCREEN_BPP, SDL_Rect posOpt[50], int options[5], string strOption[50]) {
        int alignGauche = 30;
        Image effacer("images/effacerTexte.bmp");
        Image fond("images/fondNoir.bmp");
        Image selecteur("images/selecteur.bmp");
        TTF_Font *font = TTF_OpenFont("fonts/GUNSHIP2.TTF", 20);
        TTF_Font *fontTitre = TTF_OpenFont("fonts/GUNSHIP2.TTF", 28);
        TTF_Font *fontBas = TTF_OpenFont("fonts/GUNSHIP2.TTF", 12);
        SDL_Color textColor = { 255, 0, 0 };

        fond.Affiche(0, 0, ecran);

        strOption[0] = "Options";
        strOption[1] = "Affichage";
        strOption[11] = "Plein ecran";
        strOption[12] = "Fenetre";
        strOption[2] = "Volume";
        strOption[3] = "Utiliser ma musique";
        strOption[4] = "Difficulte";
        strOption[10] = "Appuyez sur <Echap> pour revenir au menu principale";

        posOpt[0].x = 210;
        posOpt[0].y = 20;
        posOpt[1].x = alignGauche;
        posOpt[1].y = 70;
        posOpt[11].x = 350;
        posOpt[11].y = 70;
        posOpt[2].x = alignGauche;
        posOpt[2].y = 140;
        posOpt[3].x = alignGauche;
        posOpt[3].y = 210;
        posOpt[4].x = alignGauche;
        posOpt[4].y = 280;
        posOpt[5].x = alignGauche;
        posOpt[5].y = 350;
        posOpt[10].x = 30;
        posOpt[10].y = 440;

        SDL_Surface *titre = TTF_RenderText_Solid(fontTitre, strOption[0].c_str(), textColor);
        SDL_BlitSurface(titre, NULL, ecran, &posOpt[0]);
        SDL_Surface *option1 = TTF_RenderText_Solid(font, strOption[1].c_str(), textColor);
        SDL_BlitSurface(option1, NULL, ecran, &posOpt[1]);
        SDL_Surface *option11;
        if (options[1] == 1)
            option11 = TTF_RenderText_Solid(font, strOption[11].c_str(), textColor);
        else
            option11 = TTF_RenderText_Solid(font, strOption[12].c_str(), textColor);
        SDL_BlitSurface(option11, NULL, ecran, &posOpt[11]);
        SDL_Surface *option2 = TTF_RenderText_Solid(font, strOption[2].c_str(), textColor);
        SDL_BlitSurface(option2, NULL, ecran, &posOpt[2]);
        SDL_Surface *option3 = TTF_RenderText_Solid(font, strOption[3].c_str(), textColor);
        SDL_BlitSurface(option3, NULL, ecran, &posOpt[3]);
        SDL_Surface *option4 = TTF_RenderText_Solid(font, strOption[4].c_str(), textColor);
        SDL_BlitSurface(option4, NULL, ecran, &posOpt[4]);
        SDL_Surface *option10 = TTF_RenderText_Solid(fontBas, strOption[10].c_str(), textColor);
        SDL_BlitSurface(option10, NULL, ecran, &posOpt[10]);
        selecteur.Affiche(10, posOpt[1].y, ecran);
    }

    void menuOptions(SDL_Surface *ecran, vector<int>& lesOptions, int SCREEN_WIDTH, int SCREEN_HEIGHT, int SCREEN_BPP) {
        SDL_Event event;
        int options[5];
        if (lesOptions.size() > 0)
            options[1] = lesOptions[1];
        else
            options[1] = 1;
        SDL_Rect posOpt[50];
        string strOption[50];
        Image effacer("images/effacerTexte.bmp");
        menuOptionsAfficher(ecran, lesOptions, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, posOpt, options, strOption);
        Image noir("images/carreNoir.bmp");
        int posSelecteur = 1;
        bool quit = false;
        Image selecteur("images/selecteur.bmp");
        TTF_Font *font = TTF_OpenFont("fonts/GUNSHIP2.TTF", 20);
        SDL_Color textColor = { 255, 0, 0 };
        while (quit == false) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN) {
                    if (posSelecteur == 4) {
                        noir.Affiche(10, posOpt[posSelecteur].y, ecran);
                        posSelecteur = 1;
                        selecteur.Affiche(10, posOpt[posSelecteur].y, ecran);
                    } else {
                        noir.Affiche(10, posOpt[posSelecteur].y, ecran);
                        posSelecteur++;
                        selecteur.Affiche(10, posOpt[posSelecteur].y, ecran);
                    }
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP) {
                    if (posSelecteur == 1) {
                        noir.Affiche(10, posOpt[posSelecteur].y, ecran);
                        posSelecteur = 4;
                        selecteur.Affiche(10, posOpt[posSelecteur].y, ecran);
                    } else {
                        noir.Affiche(10, posOpt[posSelecteur].y, ecran);
                        posSelecteur--;
                        selecteur.Affiche(10, posOpt[posSelecteur].y, ecran);
                    }
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT) {
                    if (posSelecteur == 1) {
                        if (options[1] == 1) {
                            options[1] = 2;
                            effacer.Affiche(posOpt[11].x, posOpt[11].y, ecran);
                            SDL_Surface *option12 = TTF_RenderText_Solid(font, strOption[12].c_str(), textColor);
                            SDL_BlitSurface(option12, NULL, ecran, &posOpt[11]);
                            ecran = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
                            menuOptionsAfficher(ecran, lesOptions, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, posOpt, options, strOption);
                        } else {
                            options[1] = 1;
                            effacer.Affiche(posOpt[11].x, posOpt[11].y, ecran);
                            SDL_Surface *option11 = TTF_RenderText_Solid(font, strOption[11].c_str(), textColor);
                            SDL_BlitSurface(option11, NULL, ecran, &posOpt[11]);
                            ecran = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_FULLSCREEN);
                            menuOptionsAfficher(ecran, lesOptions, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, posOpt, options, strOption);
                        }
                    }
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
            }
            SDL_Flip(ecran);
        }
        lesOptions.clear();
        for (int i = 0;i < 5;i++) {
            lesOptions.push_back(options[i]);
            cout << "lesOptions[" << i << "] = " << lesOptions[i] << endl;
        }
    }

    int menuPrincipale(SDL_Surface *ecran) {
        SDL_Event event;
        int alignGauche = 150;
        Image fond("images/fondNoir.bmp");
        Image selecteur("images/selecteur.bmp");
        Image noir("images/carreNoir.bmp");
        TTF_Font *font = TTF_OpenFont("fonts/GUNSHIP2.TTF", 28);
        SDL_Color textColor = { 255, 0, 0 };
        fond.Affiche(0, 0, ecran);
        string option[6];
        option[1] = "MoDe ArcaDe";
        option[2] = "2 Joueurs";
        option[3] = "Jouer en reseau";
        option[4] = "Options";
        option[5] = "Quitter";
        SDL_Rect posOpt[6];
        posOpt[1].x = alignGauche;
        posOpt[1].y = 60;
        posOpt[2].x = alignGauche;
        posOpt[2].y = 130;
        posOpt[3].x = alignGauche;
        posOpt[3].y = 200;
        posOpt[4].x = alignGauche;
        posOpt[4].y = 270;
        posOpt[5].x = alignGauche;
        posOpt[5].y = 340;

        SDL_Surface *option1 = TTF_RenderText_Solid(font, option[1].c_str(), textColor);
        SDL_BlitSurface(option1, NULL, ecran, &posOpt[1]);
        SDL_Surface *option2 = TTF_RenderText_Solid(font, option[2].c_str(), textColor);
        SDL_BlitSurface(option2, NULL, ecran, &posOpt[2]);
        SDL_Surface *option3 = TTF_RenderText_Solid(font, option[3].c_str(), textColor);
        SDL_BlitSurface(option3, NULL, ecran, &posOpt[3]);
        SDL_Surface *option4 = TTF_RenderText_Solid(font, option[4].c_str(), textColor);
        SDL_BlitSurface(option4, NULL, ecran, &posOpt[4]);
        SDL_Surface *option5 = TTF_RenderText_Solid(font, option[5].c_str(), textColor);
        SDL_BlitSurface(option5, NULL, ecran, &posOpt[5]);
        TTF_CloseFont(font);

        selecteur.Affiche(115, posOpt[1].y, ecran);
        int posSelecteur = 1;
        bool quit = false;
        while (quit == false) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN) {
                    if (posSelecteur == 5) {
                        noir.Affiche(115, posOpt[posSelecteur].y, ecran);
                        posSelecteur = 1;
                        selecteur.Affiche(115, posOpt[posSelecteur].y, ecran);
                    } else {
                        noir.Affiche(115, posOpt[posSelecteur].y, ecran);
                        posSelecteur++;
                        selecteur.Affiche(115, posOpt[posSelecteur].y, ecran);
                    }
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP) {
                    if (posSelecteur == 1) {
                        noir.Affiche(115, posOpt[posSelecteur].y, ecran);
                        posSelecteur = 5;
                        selecteur.Affiche(115, posOpt[posSelecteur].y, ecran);
                    } else {
                        noir.Affiche(115, posOpt[posSelecteur].y, ecran);
                        posSelecteur--;
                        selecteur.Affiche(115, posOpt[posSelecteur].y, ecran);
                    }
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                    quit = true;
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                    posSelecteur = 5;
                }
            }
            SDL_Flip(ecran);
        }
        return posSelecteur;
    }

    void menuJeuAfficher(SDL_Surface *ecran, int posSelecteur) {
        int alignGauche = 110;

        Image fond("images/fondNoir.bmp");
        Image selecteur("images/selecteur.bmp");
        Image noir("images/carreNoir.bmp");
        TTF_Font *font = TTF_OpenFont("fonts/GUNSHIP2.TTF", 28);
        SDL_Color textColor = { 255, 0, 0 };
        fond.Affiche(0, 0, ecran);
        string option[6];
        option[1] = "Retourner au jeu";
        option[2] = "Options";
        option[3] = "Quitter la partie";
        SDL_Rect posOpt[6];
        posOpt[1].x = alignGauche;
        posOpt[1].y = 60;
        posOpt[2].x = alignGauche;
        posOpt[2].y = 130;
        posOpt[3].x = alignGauche;
        posOpt[3].y = 200;

        SDL_Surface *option1 = TTF_RenderText_Solid(font, option[1].c_str(), textColor);
        SDL_BlitSurface(option1, NULL, ecran, &posOpt[1]);
        SDL_Surface *option2 = TTF_RenderText_Solid(font, option[2].c_str(), textColor);
        SDL_BlitSurface(option2, NULL, ecran, &posOpt[2]);
        SDL_Surface *option3 = TTF_RenderText_Solid(font, option[3].c_str(), textColor);
        SDL_BlitSurface(option3, NULL, ecran, &posOpt[3]);
        TTF_CloseFont(font);

        selecteur.Affiche(alignGauche - 25, posOpt[posSelecteur].y, ecran);
    }

    int menuJeu(SDL_Surface *ecran, vector<int>& lesOptions, int SCREEN_WIDTH, int SCREEN_HEIGHT, int SCREEN_BPP) {
        SDL_Event event;
        int alignGauche = 110;
        Image fond("images/fondNoir.bmp");
        Image selecteur("images/selecteur.bmp");
        Image noir("images/carreNoir.bmp");
        fond.Affiche(0, 0, ecran);
        SDL_Rect posOpt[6];
        posOpt[1].x = alignGauche;
        posOpt[1].y = 60;
        posOpt[2].x = alignGauche;
        posOpt[2].y = 130;
        posOpt[3].x = alignGauche;
        posOpt[3].y = 200;
        int posSelecteur = 1;
        menuJeuAfficher(ecran, posSelecteur);
        bool quit = false;
        while (quit == false) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN) {
                    if (posSelecteur == 3) {
                        noir.Affiche(alignGauche - 25, posOpt[posSelecteur].y, ecran);
                        posSelecteur = 1;
                        selecteur.Affiche(alignGauche - 25, posOpt[posSelecteur].y, ecran);
                    } else {
                        noir.Affiche(alignGauche - 25, posOpt[posSelecteur].y, ecran);
                        posSelecteur++;
                        selecteur.Affiche(alignGauche - 25, posOpt[posSelecteur].y, ecran);
                    }
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP) {
                    if (posSelecteur == 1) {
                        noir.Affiche(alignGauche - 25, posOpt[posSelecteur].y, ecran);
                        posSelecteur = 3;
                        selecteur.Affiche(alignGauche - 25, posOpt[posSelecteur].y, ecran);
                    } else {
                        noir.Affiche(alignGauche - 25, posOpt[posSelecteur].y, ecran);
                        posSelecteur--;
                        selecteur.Affiche(alignGauche - 25, posOpt[posSelecteur].y, ecran);
                    }
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                    if (posSelecteur == 2) {
                        menuOptions(ecran, lesOptions, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
                        menuJeuAfficher(ecran, posSelecteur);
                    }
                    if (posSelecteur == 3 || posSelecteur == 1) {
                        quit = true;
                    }
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                    posSelecteur = 3;
                }
            }
            SDL_Flip(ecran);
        }
        return posSelecteur;
    }

private:
};
#endif
