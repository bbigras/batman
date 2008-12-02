#ifndef TABLEAU_H
#define TABLEAU_H
#include <vector>

#include "bloc.h"
#include "palette.h"
#include "pointage.h"
#include "pile.h"
#include "animation.h"
#define TICK_DESTRUCTION 200

// Un tableau est une aire de jeu (un par joueur)
class Tableau {
public:
    Tableau(int letab, SDL_Surface *scr, Pile &lap) : ecran(scr), le_tableau(letab), ptJoueur(letab, ecran), la_pile(&lap) {
        Init_Tableau();
        c4 = 0;
    }

    // Initialisation du tableau
    void Init_Tableau() {
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 13; j++) {
                tableau[j][i] = Bloc(palette.Blanc(), 1, (le_tableau == 2 ? true : false));
                tableau[j][i].SetPosition(i, j);
                tableau[j][i].Afficher(ecran, palette);
            }
        }
        touche_bas = false;
        tourne_droite = false;
        tourne_gauche = false;
        fin = false;

        a_change = false;

        // Timer
        redestruc1_tick = 0;
        redestruc1_actif = false;

        redestruc2_tick = 0;
        redestruc2_actif = false;

        pile_compteur = 0;

        un_bloc = BlocTombeur(le_tableau, palette.DVert(), 2, palette.Rouge(), 1);
        //un_bloc = BlocTombeur().Pila(la_pile, pile_compteur++, palette, le_tableau);
        un_bloc.Afficher(ecran);

        en_destruction = false;

        etat = 0;
        pauseDestruction = 0;
        noExplosion = 1;
        leDelais = SDL_GetTicks();
        Affiche_Prochain();

        compteur_detruit = 0;
        Init_tab_injection();
        compteur_injection = 0;
    }

    void Init_tab_injection() {
        for (int j = 0; j < 13; j++) {
            for (int i = 0; i < 6; i++) {
                tableau_injection[j][i] = Bloc(palette.Blanc(), 1, (le_tableau == 2 ? true : false));
            }
        }
    }

    int Compteur_detruit() {
        int temp = compteur_detruit;
        compteur_detruit = 0;
        return temp;
    }

    void Injecte(int nombre) {
        compteur_injection += nombre;
    }

    bool Afficher(int x, int y, int no) {
        palette.Tableau()->Affiche(x, y, ecran);
        Affiche_Prochain();
        return 0;
    }

    void AfficherTout(int x, int y) {
        Image tab("images/tab.jpg");
        tab.Affiche(x, y, ecran);
        for (int i = 0;i < 6;i++) {
            for (int i2 = 0;i2 < 13;i2++)
                Position(i, i2).Afficher(ecran, palette);
        }
    }

    void Pause() {
        if (etat != 1)
            etat = 1;
        else
            etat = 0;
    }

    string ExporteTombeur() {
        return un_bloc.ExporteTombeur(palette);
    }

    void ImporteTombeur(string code) {
        if (code.substr(0, 1) != "0") {
            BlocTombeur prochain_bloc = BlocTombeur();
            prochain_bloc.Importe(code, palette);
            prochain_bloc.Afficher(ecran);
        }
    }

    // retourne vrai si le joueur est mort
    bool Faitle() {
        /*cout << c4++ << endl;
        if (c4 == 20)
            c4 = 0;*/

        //etat = 2;
        CorrigeCompteurs();

        Animation boom;
        if (etat == 1) {
            Injecte2();

            bool au_moins_un = false;

            // Pour ne pas avoir a reinitialiser le tableau, on peux mettre un bloc blanc
            for (int j = 12; j >= 0; j--) {
                for (int i = 0; i < 6; i++) {
                    if (tableau_injection[j][i].Couleur() != palette.Blanc() && tableau_injection[j][i].Couleur() != palette.Noir()) {

                        if (tableau_injection[j][i].Y() < -1)
                            tableau_injection[j][i].Y(tableau_injection[j][i].Y() + 1);
                        else {
                            if (tableau_injection[j][i].Y() < 12) {
                                if (Position(tableau_injection[j][i].X(), tableau_injection[j][i].Y() + 1).Couleur() == palette.Blanc() || Position(tableau_injection[j][i].X(), tableau_injection[j][i].Y() + 1).Couleur() == palette.Noir()) {
                                    Bloc bloc_blanc(palette.Blanc(), 1, (le_tableau == 2 ? true : false));

                                    if (tableau_injection[j][i].Y() >= 0) {
                                        bloc_blanc.SetPosition(tableau_injection[j][i].X(), tableau_injection[j][i].Y());
                                        bloc_blanc.Afficher(ecran, palette);
                                    }

                                    tableau_injection[j][i].Y(tableau_injection[j][i].Y() + 1);
                                    au_moins_un = true;
                                } else {
                                    SetPosition(tableau_injection[j][i].X(), tableau_injection[j][i].Y(), tableau_injection[j][i]);
                                    Position(tableau_injection[j][i].X(), tableau_injection[j][i].Y()).Afficher(ecran, palette);
                                    tableau_injection[j][i] = Bloc(palette.Blanc(), 1, (le_tableau == 2 ? true : false));
                                }
                            } else {
                                SetPosition(tableau_injection[j][i].X(), tableau_injection[j][i].Y(), tableau_injection[j][i]);
                                Position(tableau_injection[j][i].X(), tableau_injection[j][i].Y()).Afficher(ecran, palette);
                                tableau_injection[j][i] = Bloc(palette.Blanc(), 1, (le_tableau == 2 ? true : false));
                            }

                            if (tableau_injection[j][i].Y() >= 0)
                                tableau_injection[j][i].Afficher(ecran, palette);
                        }
                    }
                }
            }
            if (!au_moins_un) {
                if (Position(3, 0).Couleur() != palette.Blanc()) {
                    cout << "fin\n";
                    Perdu();
                    return true;
                    //(un_bloc.X() == 3 && un_bloc.Y() == 30)
                } else
                    etat = 0;
            }
        }

        if (etat == 0 && pauseDestruction == 0) {
            ptJoueur.Afficher();

            // Tasse à gauche
            if (touche_droite) {
                bool ok = true;

                for (int i = un_bloc.Haut(); i <= un_bloc.Bas(); i++) {
                    if (un_bloc.Gauche() + 1 <= 5) {
                        if (Position(un_bloc.Droite() + 1, i).Couleur() != palette.Blanc())
                            ok = false;
                    }
                }

                if (un_bloc.Droite() < 5 && ok) {
                    un_bloc.Deplacer(1, ecran, palette);
                    touche_droite = false;
                }
            } else if (touche_gauche) { // Tasse à droite
                bool ok = true;

                for (int i = un_bloc.Haut(); i <= un_bloc.Bas(); i++) {
                    if (un_bloc.Gauche() - 1 >= 0) {
                        if (Position(un_bloc.Gauche() - 1, i).Couleur() != palette.Blanc())
                            ok = false;
                    }
                }

                if (un_bloc.Gauche() > 0 && ok) {
                    un_bloc.Deplacer(2, ecran, palette);
                    touche_gauche = false;
                }
            }

            bool ok_bas = true;
            int diff = -1;

            for (int i = un_bloc.Gauche(); i <= un_bloc.Droite(); i++) {
                if (un_bloc.Bas() + 1 < 13) {
                    if (Position(i, un_bloc.Bas() + 1).Couleur() != palette.Blanc())
                        diff = (un_bloc.Bas() + 1) * 30 - un_bloc.Y4();
                } else
                    diff = 390 - un_bloc.Y4();

                if (un_bloc.Bas() + 1 < 13 && un_bloc.Aligne()) {
                    if (Position(i, un_bloc.Bas() + 1).Couleur() != palette.Blanc())
                        ok_bas = false;
                }
            }

#ifdef debok
            ostringstream sa;
            sa << diff;
            debogue.Affiche("m1", sa.str());
#endif

            // Ici le .Y() fait que si le group est a l'envers, il dépasse
            if (un_bloc.Y4() < 390 && ok_bas) {
                if (touche_bas)
                    un_bloc.Deplacer(4, ecran, palette, (diff >= 10 || diff == -1) ? 10 : diff);
                else
                    un_bloc.Deplacer(4, ecran, palette, 1);
            } else {
                // Les blocs sont à leur position finale, on les met dans un_tableau
                SetPosition(un_bloc.X(), un_bloc.Y2(), un_bloc.UnBloc());
                SetPosition(un_bloc.X9(), un_bloc.Y9(), un_bloc.UnBloc2());

                // On decrémente tous les blocs qui ont un compteur
                for (int j = 0; j < 13; j++) {
                    for (int i = 0; i < 6; i++) {
                        if (tableau[j][i].Compteur() > 0) {
                            tableau[j][i].Compteur(tableau[j][i].Compteur() - 1);
                            tableau[j][i].Afficher(ecran, palette);

                            if (tableau[j][i].Compteur() == 0) {
                                destruction = Destruction(Position(i, j));
                                if (destruction == true) {
                                    redestruc1_actif = true;
                                    redestruc1_tick = SDL_GetTicks();
                                }
                            }
                        }
                    }
                }

                etat = 1;

                // Pour que le bloc tombe s'il se retrouve dans le vide
                Bloc bloc1 = un_bloc.UnBloc();
                Bloc bloc2 = un_bloc.UnBloc2();
                for (int i4 = bloc1.Y(); i4 < 12; i4++) {
                    Bloc lebloc = Position(bloc1.X(), i4 + 1);
                    if (lebloc.Couleur() == palette.Blanc()) {
                        Bloc bloc_null(palette.Blanc(), 1, (le_tableau == 2 ? true : false));
                        bloc1.SetPosition(bloc1.X(), i4 + 1);
                        SetPosition(bloc1.X(), i4, bloc_null);
                        SetPosition(bloc1.X(), i4 + 1, bloc1);
                    } else
                        break;
                }

                for (int i4 = bloc2.Y(); i4 < 12; i4++) {
                    Bloc lebloc2 = Position(bloc2.X(), i4 + 1);
                    if (lebloc2.Couleur() == palette.Blanc()) {
                        Bloc bloc_null2(palette.Blanc(), 1, (le_tableau == 2 ? true : false));
                        bloc2.SetPosition(bloc2.X(), i4 + 1);
                        SetPosition(bloc2.X(), i4, bloc_null2);
                        SetPosition(bloc2.X(), i4 + 1, bloc2);
                    } else
                        break;
                }

                // PERDU!
                if (un_bloc.X() == 3 && un_bloc.Y() == 30) {
                    Perdu();
                    fin = true;
                } else {

                    // Premier bloc
                    destruction = Destruction(Position(bloc1.X(), bloc1.Y()));
                    if (destruction == true) {
                        redestruc1_actif = true;
                        redestruc1_tick = SDL_GetTicks();
                    }

                    // Deuxième bloc du groupe (celui qui tourne autour de l'autre)
                    destruction2 = Destruction(Position(bloc2.X(), bloc2.Y()));
                    if (destruction2 == true) {
                        redestruc2_actif = true;
                        redestruc2_tick = SDL_GetTicks();
                    }

                    //un_bloc = BlocTombeur().Pila(la_pile, pile_compteur++, palette, le_tableau);
                    // (int tab, Image *couleur, int t, Image *couleur2, int t2)
                    if (redestruc1_actif || redestruc2_actif) {
                        un_bloc = BlocTombeur(le_tableau, palette.Blanc(), 1, palette.Blanc(), 1);
                        en_destruction = true;
                    } else
                        un_bloc = BlocTombeur().Pila(la_pile, pile_compteur++, palette, le_tableau);

                    un_bloc.Afficher(ecran);
                    touche_bas = false;

                    Affiche_Prochain();
                }
            }

            if (en_destruction && !redestruc1_actif && !redestruc2_actif) {
                un_bloc = BlocTombeur().Pila(la_pile, pile_compteur++, palette, le_tableau);
                un_bloc.Afficher(ecran);
                en_destruction = false;
                Affiche_Prochain();
            }

            // redestruction
            if (redestruc1_actif && redestruc1_tick + TICK_DESTRUCTION < SDL_GetTicks()) {
                destruction = ReDestruction();
                if (destruction)
                    redestruc1_tick = SDL_GetTicks();
                else
                    redestruc1_actif = false;
            }

            if (redestruc2_actif && redestruc2_tick + TICK_DESTRUCTION < SDL_GetTicks()) {
                destruction2 = ReDestruction();
                if (destruction2)
                    redestruc2_tick = SDL_GetTicks();
                else
                    redestruc2_actif = false;
            }

            // TODO : Possibilitée de regrouper tourne gauche et droite
            if (tourne_droite) {
                bool ok_tourne = true;
                switch (un_bloc.Orientation()) {
                    case 0:
                        if (un_bloc.Droite() + 1 <= 5) {
                            for (int i = un_bloc.Haut(); i <= un_bloc.Bas(); i++) {
                                if (Position(un_bloc.Droite() + 1, i).Couleur() != palette.Blanc())
                                    ok_tourne = false;
                            }
                        } else
                            ok_tourne = false;
                        break;
                    case 1:
                        if (un_bloc.Bas() + 1 < 13) {
                            if (Position(un_bloc.Droite(), un_bloc.Bas() + 1).Couleur() != palette.Blanc())
                                ok_tourne = false;
                            if (Position(un_bloc.Gauche(), un_bloc.Bas() + 1).Couleur() != palette.Blanc())
                                ok_tourne = false;
                        } else
                            ok_tourne = false;
                        break;
                    case 2:
                        if (un_bloc.Gauche() - 1 >= 0) {
                            for (int i = un_bloc.Haut(); i <= un_bloc.Bas(); i++) {
                                if (Position(un_bloc.Droite() - 1, i).Couleur() != palette.Blanc())
                                    ok_tourne = false;
                            }
                        } else
                            ok_tourne = false;
                        break;
                    case 3:
                        if (un_bloc.Haut() - 1 >= 0) {
                            if (Position(un_bloc.Droite(), un_bloc.Haut() - 1).Couleur() != palette.Blanc())
                                ok_tourne = false;
                            if (Position(un_bloc.Gauche(), un_bloc.Haut() - 1).Couleur() != palette.Blanc())
                                ok_tourne = false;
                        } else
                            ok_tourne = false;
                        break;
                };

                if (ok_tourne)
                    un_bloc.Tourne(ecran, palette);

                tourne_droite = false;

                if (un_bloc.Orientation() == 0 && !ok_tourne) {
                    if (un_bloc.Gauche() - 1 >= 0) {
                        if (Position(un_bloc.Gauche() - 1, un_bloc.Bas()).Couleur() == palette.Blanc()) {
                            un_bloc.Deplacer(2, ecran, palette);
                            un_bloc.Tourne(ecran, palette);
                        }
                    }
                } else if (un_bloc.Orientation() == 2 && !ok_tourne) {
                    if (un_bloc.Droite() + 1 <= 5) {
                        if (Position(un_bloc.Droite() + 1, un_bloc.Bas()).Couleur() == palette.Blanc()) {
                            un_bloc.Deplacer(1, ecran, palette);
                            un_bloc.Tourne(ecran, palette);
                        }
                    }
                }
            }


// 0= tête en bas
            if (tourne_gauche) {
                bool ok_tourne = true;
                switch (un_bloc.Orientation()) {
                    case 2:
                        if (un_bloc.Droite() + 1 <= 5) {
                            for (int i = un_bloc.Haut(); i <= un_bloc.Bas(); i++) {
                                if (Position(un_bloc.Droite() + 1, i).Couleur() != palette.Blanc())
                                    ok_tourne = false;
                            }
                        } else
                            ok_tourne = false;
                        break;
                    case 3:
                        if (un_bloc.Bas() + 1 < 13) {
                            if (Position(un_bloc.Droite(), un_bloc.Bas() + 1).Couleur() != palette.Blanc())
                                ok_tourne = false;
                            if (Position(un_bloc.Gauche(), un_bloc.Bas() + 1).Couleur() != palette.Blanc())
                                ok_tourne = false;
                        } else
                            ok_tourne = false;
                        break;
                    case 0:
                        if (un_bloc.Gauche() - 1 >= 0) {
                            for (int i = un_bloc.Haut(); i <= un_bloc.Bas(); i++) {
                                if (Position(un_bloc.Droite() - 1, i).Couleur() != palette.Blanc())
                                    ok_tourne = false;
                            }
                        } else
                            ok_tourne = false;
                        break;
                    case 1:
                        if (un_bloc.Haut() - 1 >= 0) {
                            if (Position(un_bloc.Droite(), un_bloc.Haut() - 1).Couleur() != palette.Blanc())
                                ok_tourne = false;
                            if (Position(un_bloc.Gauche(), un_bloc.Haut() - 1).Couleur() != palette.Blanc())
                                ok_tourne = false;
                        } else
                            ok_tourne = false;
                        break;
                };

                if (ok_tourne)
                    un_bloc.Tourne(ecran, palette, 1);

                tourne_gauche = false;

                if (un_bloc.Orientation() == 0 && !ok_tourne) {
                    if (un_bloc.Droite() + 1 <= 5) {
                        if (Position(un_bloc.Droite() + 1, un_bloc.Bas()).Couleur() == palette.Blanc()) {
                            un_bloc.Deplacer(1, ecran, palette);
                            un_bloc.Tourne(ecran, palette, 1);
                        }
                    }
                } else if (un_bloc.Orientation() == 2 && !ok_tourne) {
                    if (un_bloc.Gauche() - 1 >= 0) {
                        if (Position(un_bloc.Gauche() - 1, un_bloc.Bas()).Couleur() == palette.Blanc()) {
                            un_bloc.Deplacer(2, ecran, palette);
                            un_bloc.Tourne(ecran, palette, 1);
                        }
                    }
                }
            }
            return fin;
        } /*else if (pauseDestruction == 1) {
            if (noExplosion<17) {
                if (leDelais < SDL_GetTicks()) {
                    AfficherTout(26+(le_tableau-1)*400, 26);
                    boom.Afficher((blocsDetruits.front().X()*30+(le_tableau-1)*400+26), (blocsDetruits.front().Y()*30),ecran,noExplosion);

                    leDelais = SDL_GetTicks();
                    noExplosion+=3;
                }
            }
            else if (blocsDetruits.size() > 0) {
                Bloc *un_bloc = new Bloc(lapalette.Blanc(), 1, (le_tableau == 2 ? true : false));
                SetPosition(blocsDetruits.front().X(), blocsDetruits.front().Y(), un_bloc);
                blocsDetruits.erase(blocsDetruits.begin());
                noExplosion = 1;
            }
            else {
                blocsDetruits.erase(blocsDetruits.begin());
                Tombez();
                AfficherTout(26+(le_tableau-1)*400, 26);
                pauseDestruction = 0;
                noExplosion = 1;
            }
            return false;

        }*/
        else
            return false;
    }

    void Affiche_Prochain() {
        BlocTombeur prochain_bloc = BlocTombeur().Pila(la_pile, pile_compteur, palette, le_tableau);
        prochain_bloc.Afficher2(ecran);
    }

    void TDroite(bool val) {
        touche_droite = val;
    }

    void TGauche(bool val) {
        touche_gauche = val;
    }

    void TBas(bool val) {
        touche_bas = val;
    }

    void TTourneDroite(bool val) {
        tourne_droite = val;
    }

    void TTourneGauche(bool val) {
        tourne_gauche = val;
    }


    bool A_Change() {
        return a_change;
    }

    bool Change_points() {
        return ptJoueur.Changer();
    }

    string ExporteTableau() {
        int array[13][6];

        for (int i = 0; i < 13; i++) {
            for (int j = 0; j < 6; j++) {
                if (Position(j, i).Couleur() == palette.Bleu())
                    array[i][j] = 1;
                else if (Position(j, i).Couleur() == palette.Jaune())
                    array[i][j] = 2;
                else if (Position(j, i).Couleur() == palette.Rouge())
                    array[i][j] = 3;
                else if (Position(j, i).Couleur() == palette.Vert())
                    array[i][j] = 4;
                else if (Position(j, i).Couleur() == palette.DBleu())
                    array[i][j] = 5;
                else if (Position(j, i).Couleur() == palette.DJaune())
                    array[i][j] = 6;
                else if (Position(j, i).Couleur() == palette.DRouge())
                    array[i][j] = 7;
                else if (Position(j, i).Couleur() == palette.DVert())
                    array[i][j] = 8;
                else
                    array[i][j] = 0;
            }
        }


        for (int j = 12; j >= 0; j--) {
            for (int i = 0; i < 6; i++) {
                if (tableau_injection[j][i].Couleur() != palette.Blanc() && tableau_injection[j][i].Couleur() != palette.Noir()) {
                    if (tableau_injection[j][i].Couleur() == palette.Bleu())
                        array[tableau_injection[j][i].Y()][tableau_injection[j][i].X()] = 1;
                    else if (tableau_injection[j][i].Couleur() == palette.Jaune())
                        array[tableau_injection[j][i].Y()][tableau_injection[j][i].X()] = 2;
                    else if (tableau_injection[j][i].Couleur() == palette.Rouge())
                        array[tableau_injection[j][i].Y()][tableau_injection[j][i].X()] = 3;
                    else if (tableau_injection[j][i].Couleur() == palette.Vert())
                        array[tableau_injection[j][i].Y()][tableau_injection[j][i].X()] = 4;
                    else if (tableau_injection[j][i].Couleur() == palette.DBleu())
                        array[tableau_injection[j][i].Y()][tableau_injection[j][i].X()] = 5;
                    else if (tableau_injection[j][i].Couleur() == palette.DJaune())
                        array[tableau_injection[j][i].Y()][tableau_injection[j][i].X()] = 6;
                    else if (tableau_injection[j][i].Couleur() == palette.DRouge())
                        array[tableau_injection[j][i].Y()][tableau_injection[j][i].X()] = 7;
                    else if (tableau_injection[j][i].Couleur() == palette.DVert())
                        array[tableau_injection[j][i].Y()][tableau_injection[j][i].X()] = 8;
                    else
                        array[tableau_injection[j][i].Y()][tableau_injection[j][i].X()] = 0;
                }
            }
        }

        string tampon = "";
        for (int j = 0; j < 13; j++) {
            for (int i = 0; i < 6; i++) {
                ostringstream b;
                b << array[j][i];
                tampon += b.str();
            }
        }
        a_change = false;

        string tampon2 = "";
        for (int j = 0; j < 13; j++) {
            for (int i = 0; i < 6; i++) {
                ostringstream b2;
                b2 << Position(i, j).Compteur();
                tampon2 += b2.str();
            }
        }

        return tampon + tampon2;
    }

    void ImporteTableau(int array[13][6], int array11[13][6]) {
        bool larap = (le_tableau == 2 ? true : false);

        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 13; j++) {
                switch (array[j][i]) {
                    case 1:
                        SetPosition(i, j, Bloc(palette.Bleu(), 1, larap, array11[j][i]));
                        break;
                    case 2:
                        SetPosition(i, j, Bloc(palette.Jaune(), 1, larap, array11[j][i]));
                        break;
                    case 3:
                        SetPosition(i, j, Bloc(palette.Rouge(), 1, larap, array11[j][i]));
                        break;
                    case 4:
                        SetPosition(i, j, Bloc(palette.Vert(), 1, larap, array11[j][i]));
                        break;
                    case 5:
                        SetPosition(i, j, Bloc(palette.DBleu(), 2, larap, array11[j][i]));
                        break;
                    case 6:
                        SetPosition(i, j, Bloc(palette.DJaune(), 2, larap, array11[j][i]));
                        break;
                    case 7:
                        SetPosition(i, j, Bloc(palette.DRouge(), 2, larap, array11[j][i]));
                        break;
                    case 8:
                        SetPosition(i, j, Bloc(palette.DVert(), 2, larap, array11[j][i]));
                        break;
                    default:
                        SetPosition(i, j, Bloc(palette.Blanc(), 1, larap, array11[j][i]));
                        break;
                }

                //SetPosition(i, j, un_bloc);
                //Position(i, j).Compteur();
                Position(i, j).Afficher(ecran, palette);
            }
        }
    }

    string ExportePointage() {
        return ptJoueur.Exporte();
    }

    void ImportePointage(string p) {
        ptJoueur.Importe(p);
        ptJoueur.Afficher();
    }
private:
int c4;
    Bloc tableau[13][6];
    SDL_Surface *ecran;
    bool touche_bas;
    bool touche_droite;
    bool touche_gauche;
    bool tourne_droite;
    bool tourne_gauche;

    Bloc tableau_injection[13][6];

    BlocTombeur un_bloc;
    PaletteCouleur palette;
    int le_tableau;
    bool fin;

    vector<Bloc2> blocsDetruits;
    int noExplosion;
    Pointage ptJoueur; // Pointage
    Uint32 redestruc1_tick;
    bool redestruc1_actif;

    Uint32 redestruc2_tick;
    bool redestruc2_actif;

    bool destruction;
    bool destruction2;

    Pile *la_pile;
    int pile_compteur;

    bool en_destruction;

    int etat;
    int pauseDestruction; //Permet de mettre le jeu en pause
    Uint32 leDelais;
    bool a_change;
    bool a_change_points;

    int compteur_detruit; // Nombre de blocs detruits par le joueur
    int compteur_injection;

    // -----------------
    void CorrigeCompteurs() {
        if (compteur_detruit > compteur_injection && compteur_injection > 0) {
            compteur_detruit -=  compteur_injection;
            compteur_injection = 0;
        } else if (compteur_injection > compteur_detruit && compteur_detruit > 0) {
            compteur_injection -= compteur_detruit;
            compteur_detruit = 0;
        }
        AfficheInjection();
    }

    void Injecte2() {
        for (int j = 0; j < 13; j++) {
            for (int i = 0; i < 6; i++) {
                if (tableau_injection[j][i].Couleur() == palette.Noir())
                    tableau_injection[j][i] = Bloc(palette.Blanc(), 1, (le_tableau == 2 ? true : false), 0);
            }
        }

        for (int j = 0; j < 13; j++) {
            for (int i = 0; i < 6; i++) {
                if (tableau[j][i].Couleur() != palette.Blanc()) {
                    tableau_injection[12-j][i] = Bloc(palette.Noir(), 1, (le_tableau == 2 ? true : false), 0);
                    //cout << 12 - j << " , " << i << endl;
                }
            }
        }

        bool plein = tableau_injection[0][0].Couleur() != palette.Blanc() && tableau_injection[0][1].Couleur() != palette.Blanc() && tableau_injection[0][2].Couleur() != palette.Blanc() && tableau_injection[0][3].Couleur() != palette.Blanc() && tableau_injection[0][4].Couleur() != palette.Blanc() && tableau_injection[0][5].Couleur() != palette.Blanc() && tableau_injection[0][0].Couleur() != palette.Noir() && tableau_injection[0][1].Couleur() != palette.Noir() && tableau_injection[0][2].Couleur() != palette.Noir() && tableau_injection[0][3].Couleur() != palette.Noir() && tableau_injection[0][4].Couleur() != palette.Noir() && tableau_injection[0][5].Couleur() != palette.Noir();

        if (plein)
            cout << "TABLEAU PLEIN!!!\n";

        for (int i = 0; i < compteur_injection && !plein; i++) {

            if (plein) {
                cout << "TABLEAU PLEIN!!!\n";
                SDL_Delay(2000);
            }

            int colonne = (rand() % 6) + 0;
            int couleure = (rand() % 4) + 0;

            bool ok;
            int r;

            int detecteur = 0;

            do {
                r = 12;
                while (r > 0 && (tableau_injection[r][colonne].Couleur() != palette.Blanc() || tableau_injection[r][colonne].Couleur() == palette.Noir()))
                    r--;

                if (tableau_injection[r][colonne].Couleur() != palette.Noir())
                    ok = true;
                else {
                    colonne = (rand() % 6) + 0;
                    ok = false;
                }

                if (plein) {
                    cout << "TABLEAU PLEIN!!\n";
                    ok = true;
                    SDL_Delay(2000);
                    exit(1);
                }

                if (detecteur++ > 30) {
                    cout << "C-C-C-COMBO BREAKER!!!\n";
                    cout << "r: " << r << endl;
                    cout << "ok: " << ok << " true=" << true << endl;
                    cout << "plein: " << plein << endl;
                    plein = tableau_injection[0][0].Couleur() != palette.Blanc() && tableau_injection[0][1].Couleur() != palette.Blanc() && tableau_injection[0][2].Couleur() != palette.Blanc() && tableau_injection[0][3].Couleur() != palette.Blanc() && tableau_injection[0][4].Couleur() != palette.Blanc() && tableau_injection[0][5].Couleur() != palette.Blanc() && tableau_injection[0][0].Couleur() != palette.Noir() && tableau_injection[0][1].Couleur() != palette.Noir() && tableau_injection[0][2].Couleur() != palette.Noir() && tableau_injection[0][3].Couleur() != palette.Noir() && tableau_injection[0][4].Couleur() != palette.Noir() && tableau_injection[0][5].Couleur() != palette.Noir();
                    cout << "plein2: " << plein << endl;
                    exit(1);
                }
            } while (!ok);

            if (couleure == 0)
                tableau_injection[r][colonne] = Bloc(palette.Bleu(), 1, (le_tableau == 2 ? true : false), 3);
            else if (couleure == 1)
                tableau_injection[r][colonne] = Bloc(palette.Vert(), 1, (le_tableau == 2 ? true : false), 3);
            else if (couleure == 2)
                tableau_injection[r][colonne] = Bloc(palette.Jaune(), 1, (le_tableau == 2 ? true : false), 3);
            else if (couleure == 3)
                tableau_injection[r][colonne] = Bloc(palette.Rouge(), 1, (le_tableau == 2 ? true : false), 3);

            tableau_injection[r][colonne].SetPosition(colonne, r - 12);
            compteur_injection--;
            plein = tableau_injection[0][0].Couleur() != palette.Blanc() && tableau_injection[0][1].Couleur() != palette.Blanc() && tableau_injection[0][2].Couleur() != palette.Blanc() && tableau_injection[0][3].Couleur() != palette.Blanc() && tableau_injection[0][4].Couleur() != palette.Blanc() && tableau_injection[0][5].Couleur() != palette.Blanc() && tableau_injection[0][0].Couleur() != palette.Noir() && tableau_injection[0][1].Couleur() != palette.Noir() && tableau_injection[0][2].Couleur() != palette.Noir() && tableau_injection[0][3].Couleur() != palette.Noir() && tableau_injection[0][4].Couleur() != palette.Noir() && tableau_injection[0][5].Couleur() != palette.Noir();
        }
    }

    void AfficheInjection() {
        Image rectPointage("images/rectPointage.bmp");
        SDL_Rect offset;

        offset.x = (le_tableau == 1 ? 60 : 460);
        offset.y = 450;

        TTF_Font *font = TTF_OpenFont("fonts/GUNSHIP2.TTF", 14);
        SDL_Color textColor = { 0, 0, 0 };
        ostringstream b;
        b << compteur_injection;
        rectPointage.Affiche(offset.x - 4, offset.y - 2, ecran);
        SDL_Surface *message = TTF_RenderText_Solid(font, b.str().c_str(), textColor);
        SDL_BlitSurface(message, NULL, ecran, &offset);
        TTF_CloseFont(font);
    }

    void SetPosition(unsigned int x, unsigned int y, Bloc unbloc) {
        tableau[y][x] = unbloc;
        tableau[y][x].SetPosition(x, y);
        tableau[y][x].Afficher(ecran, palette);
        a_change = true;
    }

    void AffichePos(unsigned int x, unsigned int y) {
        tableau[y][x].Afficher(ecran, palette);
    }

    // Retourne un Bloc() pareil à celui à la position y,x dans le tableau
    Bloc Position(unsigned int x, unsigned int y) {
        return tableau[y][x];
    }

    /*void Test() {
        for (int j = 0; j < 13; j++) {
            for (int i = 0; i < 6; i++)
                cout << tableau[j][i].Couleur() << " ";
            cout << endl;
        }
    }*/

    bool Present(int x, int y, vector<Bloc2> &var) {
        for (unsigned int j = 0; j < var.size(); j++) {
            if (var[j].X() == x && var[j].Y() == y)
                return true;
        }
        return false;
    }

    bool Destruction(Bloc un_bloc) {
        vector<Bloc2> var;

        bool uneDestruction = false;
        if (un_bloc.Couleur() == palette.Bleu())
            Check(un_bloc.X(), un_bloc.Y(), palette.DBleu(), palette.DBleu(), var);
        else if (un_bloc.Couleur() == palette.Vert())
            Check(un_bloc.X(), un_bloc.Y(), palette.DVert(), palette.DVert(), var);
        else if (un_bloc.Couleur() == palette.Jaune())
            Check(un_bloc.X(), un_bloc.Y(), palette.DJaune(), palette.DJaune(), var);
        else if (un_bloc.Couleur() == palette.Rouge())
            Check(un_bloc.X(), un_bloc.Y(), palette.DRouge(), palette.DRouge(), var);

        destruction = false;

        if (var.size() > 1 || un_bloc.Type() == 2)
            destruction = true;
        var.clear();
        if (destruction == true) {
            if (un_bloc.Couleur() == palette.DBleu() || un_bloc.Couleur() == palette.Bleu()) {
                Check(un_bloc.X(), un_bloc.Y(), palette.Bleu(), palette.DBleu(), var);
                //Check(un_bloc.X(),un_bloc.Y(),palette.DBleu(), ecran, palette, var);
            } else if (un_bloc.Couleur() == palette.DVert() || un_bloc.Couleur() == palette.Vert()) {
                Check(un_bloc.X(), un_bloc.Y(), palette.Vert(), palette.DVert(), var);
                //Check(un_bloc.X(),un_bloc.Y(),palette.DVert(), ecran, palette, var);
            } else if (un_bloc.Couleur() == palette.DJaune() || un_bloc.Couleur() == palette.Jaune()) {
                Check(un_bloc.X(), un_bloc.Y(), palette.Jaune(), palette.DJaune(), var);
                //Check(un_bloc.X(),un_bloc.Y(),palette.DJaune(), ecran, palette, var);
            } else if (un_bloc.Couleur() == palette.DRouge() || un_bloc.Couleur() == palette.Rouge()) {
                Check(un_bloc.X(), un_bloc.Y(), palette.Rouge(), palette.DRouge(), var);
                //Check(un_bloc.X(),un_bloc.Y(),palette.DRouge(), ecran, palette, var);
            }

            if (var.size() > 1) {
                compteur_detruit += var.size();
                ptJoueur.Marquer((var.size())*10);
                uneDestruction = true;
                for (unsigned int i = 0; i < var.size(); i++) {
                    Bloc un_bloc = Bloc(palette.Blanc(), 1, (le_tableau == 2 ? true : false));
                    SetPosition(var[i].X(), var[i].Y(), un_bloc);
                    //blocsDetruits.push_back(var[i]);
                    //pauseDestruction = 1;
                }
                // Pour que les blocs tombent
                Tombez();
            }
        }
        CorrigeCompteurs();
        return uneDestruction;
    }

    void Tombez() {
        for (int i2 = 0; i2 < 6; i2++) {
            for (int i3 = 1; i3 < 13; i3++) {
                Bloc lebloc = Position(i2, i3);
                if (lebloc.Couleur() == palette.Blanc()) {
                    Bloc bloc_tombe = Bloc(palette.Blanc(), 1, (le_tableau == 2 ? true : false));
                    for (int i4 = i3; i4 > 0; i4--) {
                        bloc_tombe = Position(i2, i4 - 1);
                        if (bloc_tombe.Couleur() != palette.Blanc()) {
                            Bloc un_bloc = Bloc(palette.Blanc(), 1, (le_tableau == 2 ? true : false));
                            SetPosition(i2, i4 - 1, un_bloc);
                            SetPosition(i2, i4, bloc_tombe);
                        }
                    }
                }
            }
        }
    }

    bool ReDestruction() {
        bool uneDestruction = false;
        int x, y;
        vector<Bloc2> var;
        Bloc un_bloc;
        for (int j = 0; j < 6; j++) {
            for (int j2 = 0; j2 < 13; j2++) {
                un_bloc = Position(j, j2);
                x = j;
                y = j2;
                if (un_bloc.Couleur() == palette.DBleu()) {
                    Check(x, y, palette.Bleu(), palette.DBleu(), var);
                    //Check(x,y,palette.DBleu(), ecran, palette, var);
                } else if (un_bloc.Couleur() == palette.DVert()) {
                    Check(x, y, palette.Vert(), palette.DVert(), var);
                    //Check(x,y,palette.DVert(), ecran, palette, var);
                } else if (un_bloc.Couleur() == palette.DJaune()) {
                    Check(x, y, palette.Jaune(), palette.DJaune(), var);
                    //Check(x,y,palette.DJaune(), ecran, palette, var);
                } else if (un_bloc.Couleur() == palette.DRouge()) {
                    Check(x, y, palette.Rouge(), palette.DRouge(), var);
                    //Check(x,y,palette.DRouge(), ecran, palette, var);
                }
                if (var.size() > 1) {
                    compteur_detruit += var.size();
                    ptJoueur.Marquer((var.size())*15);
                    uneDestruction = true;
                    for (unsigned int i = 0; i < var.size(); i++) {
                        int x6 = var[i].X();
                        int y6 = var[i].Y();

                        Bloc un_bloc = Bloc(palette.Blanc(), 1, (le_tableau == 2 ? true : false));
                        SetPosition(x6, y6, un_bloc);
                    }
                    // Pour que les blocs tombent
                    for (int i5 = 0; i5 < 6; i5++) {
                        for (int i3 = 1; i3 < 13; i3++) {
                            Bloc lebloc = Position(i5, i3);
                            if (lebloc.Couleur() == palette.Blanc()) {
                                Bloc bloc_tombe = Bloc(palette.Blanc(), 1, (le_tableau == 2 ? true : false));
                                for (int i4 = i3; i4 > 0; i4--) {
                                    bloc_tombe = Position(i5, i4 - 1);
                                    if (bloc_tombe.Couleur() != palette.Blanc()) {
                                        Bloc un_bloc(palette.Blanc(), 1, (le_tableau == 2 ? true : false));
                                        SetPosition(i5, i4 - 1, un_bloc);
                                        SetPosition(i5, i4, bloc_tombe);
                                    }
                                }
                            }
                        }
                    }
                }
                var.clear();
            }
        }
        CorrigeCompteurs();
        return uneDestruction;
    }

    // Cherche les blocs de couleure « couleure » ou « couleure2 » qui sont reliés au bloc dans la position x4,y4 du tableau
    void Check(int x4, int y4, Image *couleure, Image *couleure2, vector<Bloc2> &var) {
        var.push_back(Bloc2(x4, y4));

        for (unsigned int i = 0; i < var.size(); i++) {
            int x = var[i].X();
            int y = var[i].Y();

            if (y + 1 < 13) {
                if ((tableau[y+1][x].Couleur() == couleure && tableau[y+1][x].Compteur() == 0) || tableau[y+1][x].Couleur() == couleure2) {
                    if (!Present(x, y + 1, var))
                        var.push_back(Bloc2(x, y + 1));
                }
            }

            if (y - 1 >= 0) {
                if ((tableau[y-1][x].Couleur() == couleure && tableau[y-1][x].Compteur() == 0) || tableau[y-1][x].Couleur() == couleure2) {
                    if (!Present(x, y - 1, var))
                        var.push_back(Bloc2(x, y - 1));
                }
            }

            if (x + 1 < 6) {
                if ((tableau[y][x+1].Couleur() == couleure && tableau[y][x+1].Compteur() == 0) || tableau[y][x+1].Couleur() == couleure2) {
                    if (!Present(x + 1, y, var))
                        var.push_back(Bloc2(x + 1, y));
                }
            }

            if (x - 1 >= 0) {
                if ((tableau[y][x-1].Couleur() == couleure && tableau[y][x-1].Compteur() == 0) || tableau[y][x-1].Couleur() == couleure2) {
                    if (!Present(x - 1, y, var))
                        var.push_back(Bloc2(x - 1, y));
                }
            }
        }
    }

    Image *Couleur(int x, int y) {
        return tableau[x][y].Couleur();
    }

    void Perdu() {
        Bloc ab(palette.Noir(), 0, (le_tableau == 2 ? true : false));
        for (int j = 0; j < 13; j++) {
            for (int i = 0; i < 6; i++) {
                ab.SetPosition(i, j);
                ab.Afficher(ecran, palette);
            }
        }
    }
};
#endif
