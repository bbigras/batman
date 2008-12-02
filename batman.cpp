/*
 * Programme de fou!
 *
 * par Louis-Jo�l et Bruno
 *
adresse : https://burnbox.net/hg/batman/

                            ,-.
       ___,---.__          /'|`\          __,---,___
    ,-'    \`    `-.____,-'  |  `-.____,-'    //    `-.
  ,'        |           ~'\     /`~           |        `.
 /      ___//              `. ,'          ,  , \___      \
|    ,-'   `-.__   _         |        ,    __,-'   `-.    |
|   /          /\_  `   .    |    ,      _/\          \   |
\  |           \ \`-.___ \   |   / ___,-'/ /           |  /
 \  \           | `._   `\\  |  //'   _,' |           /  /
  `-.\         /'  _ `---'' , . ``---' _  `\         /,-'
     ``       /     \    ,='/ \`=.    /     \       ''
             |__   /|\_,--.,-.--,--._/|\   __|
             /  `./  \\`\ |  |  | /,//' \,'  \
            /   /     ||--+--|--+-/-|     \   \
           |   |     /'\_\_\ | /_/_/`\     |   |
            \   \__, \_     `~'     _/ .__/   /
             `-._,-'   `-._______,-'   `-._,-'

*/

/* A FAIRE:
    - Tourner vers la gauche
    - Regler le bogue des deux destructeurs jaunes...

    - Echanges de bloc entres les deux tableaux

    - Musique (playliste...) et sons (explosions, mouvements...)
    - Megabloc
    - AI
    - Reseau
*/

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <sstream>

#include "audio.h"
#include "tableau.h"
#include "palette.h"
#include "bd.h"
#include "debug.h"
#include "pointage.h"
#include "net.h"
#include "menu.h"
//#include "video.h"

//#define PLEIN_ECRAN

//#define TEST_TABLEAU
#define DEUX_JOUEURS

//#define NET

//#define debok

// 0: Serveur, 1: Client
#define CLIENT_OU_SERVEUR 0

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

SDL_Surface *ecran = NULL;

//The event structure that will be used
SDL_Event event;

bool init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
        return false;

    TTF_Init();

    SDL_ShowCursor(SDL_DISABLE); // D�sactive le pointeur de la souris

#ifdef PLEIN_ECRAN
    ecran = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_FULLSCREEN);
#else
    ecran = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
#endif

    if (ecran == NULL)
        return false;

    // Nom de la fen�tre
    SDL_WM_SetCaption("Super mega projet de la muerte", NULL);

    return true;
}

int main(int argc, char* args[]) {
    srand(time(NULL)); // hasard
    Pile la_pile;

#ifdef NET
    Connection une_connection;
#endif
    Bd labd;

    // ---------------------------------------------------
    // Creer les tables
    labd.Init_Bd();

    // Ecrit et lit une option dans la BD
    labd.Ecrit("Volume", "50");
    cout << "Volume: " << labd.Lit("Volume") << endl;

    // Affiche le nom et le score du meilleur compteur (il y en aura toujours 10
    Personne blehsti[10];
    labd.Top10(blehsti);
    cout << "Meilleur: " << blehsti[0].Nom() << " -> " << blehsti[0].Pointage() << endl;

    // Verifier si 100 est un records et si oui, il l'ajoute
    ostringstream pp;
    pp << 100;
    if (labd.Check_Record(100)) {
        string nom = "Hercule";

        labd.Faitle("INSERT INTO Pointages VALUES ('" + nom + "', " + pp.str() + ");");
    }

    // ---------------------------------------------------

    bool quit = false;

    // Initialisation
    if (init() == false)
        return 1;

    // � FAIRE : ajuster pour que �a sonne bien!
    //Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
    Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);

    /*Musique unemusique("the.mp3", true);
    if (!unemusique.Etat())
        cout << "Probleme avec le fichier chose la la!\n";*/

    Menu menu;
    int choixMenu = menu.menuPrincipale(ecran);
    vector <int> options;

    // Tant que l'utilisateur choisi le menu d'option
    while (choixMenu == 4) {
        menu.menuOptions(ecran, options, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
        choixMenu = menu.menuPrincipale(ecran);
    }
    cout << "choix menu: " << choixMenu << endl;
    // Tant que l'utilisateur n'a pas choisi de quitter
    while (choixMenu != 5) {

#ifdef debok
        Debogue debogue(ecran);
#endif

        Tableau tableau_joueur1(1, ecran, la_pile);
        Tableau tableau_joueur2(2, ecran, la_pile);

        Image chuck("images/ChuckNorris.png");

        chuck.Affiche(0, 0, ecran);

        tableau_joueur1.Afficher(26, 26, 1);
        tableau_joueur2.Afficher(426, 26, 2);

        Son chuck2("Chuck-Norris.wav");

#ifdef TEST_TABLEAU

        int tableau_test[13][6] = {{0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 4, 0, 0},
            {0, 0, 4, 4, 8, 0},
        };

        // Les compteurs
        int tableau_test2[13][6] = {{0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
        };

        //Bug étrange, si ya 2 blocs DVert ki tombe dans le trou c po tro bon
        /*int tableau_test[13][6] = {{0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 4, 0, 0, 0, 1},
            {0, 4, 0, 0, 0, 1},
            {0, 3, 0, 0, 0, 4},
            {0, 3, 0, 0, 0, 4},
            {0, 2, 0, 0, 0, 3},
            {8, 2, 0, 0, 5, 3},
            {7, 1, 0, 0, 8, 2},
            {6, 1, 0, 0, 7, 2},
            {5, 4, 0, 0, 6, 4},
            {4, 4, 0, 0, 4, 4}
        };*/

        /* Bug, les 2 blocs jaunes a droite ne devraient pas se détruirent
        int tableau_test[13][6] = {{0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 0},
            {0, 0, 0, 2, 0, 2},
            {1, 1, 0, 2, 7, 2},
            {4, 4, 0, 3, 6, 3},
            {2, 4, 0, 3, 3, 2},
            {2, 4, 0, 4, 4, 7},
            {2, 4, 0, 4, 4, 4}
        };*/

        // Pour tester, ca remplit le tableau selon le array tableau_test
        tableau_joueur1.ImporteTableau(tableau_test, tableau_test2);
        tableau_joueur2.ImporteTableau(tableau_test, tableau_test2);

        //tableau_joueur1.Position(1, 12)->plusCompteur(3);
        //tableau_joueur1.AffichePos(1, 12);
        //tableau_joueur2.Importe(tableau_test);
#endif


#ifdef NET
        int array[13][6] = {};
        int array2[13][6] = {};
#endif
        bool fin = false;



        while (quit == false) {

#ifdef NET
            //if (tableau_joueur1.A_Change())
            une_connection.Envoie(tableau_joueur1.ExporteTableau(), 1); // Le tableau

            une_connection.Envoie(tableau_joueur1.ExporteTombeur(), 2);  // Le bloc qui tombe

            if (tableau_joueur1.Change_points())
                une_connection.Envoie(tableau_joueur1.ExportePointage(), 3); // Le pointage

            une_connection.Envoie(Int_Vers_String(tableau_joueur1.Compteur_detruit()), 4); // Le nombre de bloc détruits

            une_connection.Recoie(); // Recoit les données par la connection réseau
#endif

            SDL_Delay(30);
            while (SDL_PollEvent(&event)) {
                if (fin == false) {
                    if (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_p || event.key.keysym.sym == SDLK_PAUSE)) {
                        tableau_joueur1.Pause();
                        tableau_joueur2.Pause();
                        Menu bleh;
                        bleh.Afficher(ecran);
                    }

                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F1) {
                        tableau_joueur1.Init_Tableau();
                        tableau_joueur2.Init_Tableau();
                        la_pile.Init_Pile();
                    }

                    // TODO Intégré ceci dans Tableau() pour éliminer le code en double
                    // Joueur 1
                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == labd.Controle("1", 3))
                        tableau_joueur1.TDroite(true);
                    else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == labd.Controle("1", 2))
                        tableau_joueur1.TGauche(true);
                    else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == labd.Controle("1", 4))
                        tableau_joueur1.TBas(true);
                    else if (event.type == SDL_KEYUP && event.key.keysym.sym == labd.Controle("1", 4))
                        tableau_joueur1.TBas(false);
                    else if (event.type == SDL_KEYUP && event.key.keysym.sym == labd.Controle("1", 3))
                        tableau_joueur1.TDroite(false);
                    else if (event.type == SDL_KEYUP && event.key.keysym.sym == labd.Controle("1", 2))
                        tableau_joueur1.TGauche(false);
                    else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == labd.Controle("1", 6))
                        tableau_joueur1.TTourneDroite(true);
                    else if (event.type == SDL_KEYUP && event.key.keysym.sym == labd.Controle("1", 6))
                        tableau_joueur1.TTourneDroite(false);
                    else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == labd.Controle("1", 5))
                        tableau_joueur1.TTourneGauche(true);
                    else if (event.type == SDL_KEYUP && event.key.keysym.sym == labd.Controle("1", 5))
                        tableau_joueur1.TTourneGauche(false);

                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F2)
                        tableau_joueur2.Injecte(2);

                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F3)
                        tableau_joueur2.Init_tab_injection();


                    // Joueur 2
                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == labd.Controle("2", 3))
                        tableau_joueur2.TDroite(true);
                    else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == labd.Controle("2", 2))
                        tableau_joueur2.TGauche(true);
                    else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == labd.Controle("2", 4))
                        tableau_joueur2.TBas(true);
                    else if (event.type == SDL_KEYUP && event.key.keysym.sym == labd.Controle("2", 4))
                        tableau_joueur2.TBas(false);
                    else if (event.type == SDL_KEYUP && event.key.keysym.sym == labd.Controle("2", 3))
                        tableau_joueur2.TDroite(false);
                    else if (event.type == SDL_KEYUP && event.key.keysym.sym == labd.Controle("2", 2))
                        tableau_joueur2.TGauche(false);
                    else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == labd.Controle("2", 6))
                        tableau_joueur2.TTourneDroite(true);
                    else if (event.type == SDL_KEYUP && event.key.keysym.sym == labd.Controle("2", 6))
                        tableau_joueur2.TTourneDroite(false);
                    else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == labd.Controle("2", 5))
                        tableau_joueur2.TTourneGauche(true);
                    else if (event.type == SDL_KEYUP && event.key.keysym.sym == labd.Controle("2", 5))
                        tableau_joueur2.TTourneGauche(false);
                }

                if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                    int optionJeu = menu.menuJeu(ecran, options, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
                    if (optionJeu == 3) {
                        quit = true;
                    }
                    if (optionJeu == 1) {
                        // Réafficher toute le jeu comme
                        chuck.Affiche(0, 0, ecran);
                        //tableau_joueur1.Afficher(26, 26, 1);
                        tableau_joueur1.AfficherTout(26, 26);
                        tableau_joueur2.AfficherTout(426, 26);
                        tableau_joueur1.Affiche_Prochain();
                        tableau_joueur2.Affiche_Prochain();
                    }
                    while (optionJeu == 2) {
                        menu.menuOptions(ecran, options, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
                        optionJeu = menu.menuJeu(ecran, options, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
                    }
                }
            }

            if (!fin) {
                bool joueur1_mort = false;
                bool joueur2_mort = false;


                if (1==4) { //une_connection.Actif() && !une_connection.Ok()) {

                } else {

                    joueur1_mort = tableau_joueur1.Faitle();

#ifdef DEUX_JOUEURS
                    joueur2_mort = tableau_joueur2.Faitle();
#else
                    joueur2_mort = false;
#endif

                    joueur1_mort = false;
                    joueur2_mort = false;

                }
                /*if (pp > 0)
                    array[pp--][5] = 1;*/

                // copie le tableau1 dans 2
                /*int array9[13][6];
                tableau_joueur1.Exporte(array9);
                tableau_joueur2.Importe(array9);

                string test_tombeur=tableau_joueur1.ExporteTombeur();
                tableau_joueur2.ImporteTombeur(test_tombeur);*/

#ifdef NET
                une_connection.ExporteTableau(array, array2);
                tableau_joueur2.ImporteTableau(array, array2);

                if (une_connection.ExporteTomb() != "")
                    tableau_joueur2.ImporteTombeur(une_connection.ExporteTomb());

                if (une_connection.ExportePoint() != "")
                    tableau_joueur2.ImportePointage(une_connection.ExportePoint());
#endif

                // Envoie de bloc dans le tableau de l'autre joueur
                tableau_joueur2.Injecte(tableau_joueur1.Compteur_detruit());
                tableau_joueur1.Injecte(tableau_joueur2.Compteur_detruit());


                if (joueur1_mort && !joueur2_mort) {
                    fin = true;

#ifdef debok
                    debogue.Affiche("fin = ", "Joueur2 a gagner");
#endif
                } else if (joueur2_mort && !joueur1_mort) {
                    fin = true;

#ifdef debok
                    debogue.Affiche("fin = ", "Joueur1 a gagner");
#endif
                } else if (joueur1_mort && joueur2_mort) {
                    fin = true;

#ifdef debok
                    debogue.Affiche("fin = ", "Egalite!");
#endif
                }

#ifdef debok
                ostringstream sa;
                sa << diff;
                debogue.Affiche("m1", sa.str());
#endif
            }

#ifdef debok
            Grille bleh(ecran);
#endif

            SDL_Flip(ecran);
        }
    }

    // Dompe le tableau
    /*for (int j = 12; j < 13; j++) {
        for (int i = 0; i < 6; i++)
            cout << tableau_joueur1.Position(i, j) << " "; // ->Couleur()
        cout << endl;
    }*/

    // Dompe le tableau
    /*for (int j = 0; j < 13; j++) {
        for (int i = 0; i < 6; i++)
            cout << tableau_joueur1.Position(i, j)->Couleur() << " ";
        cout << endl;
    }*/
    //tableau_joueur1.Test();

    SDL_Quit();

    return 0;
}
