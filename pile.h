#ifndef PILE_H
#define PILE_H

class ElemPile {
public:
    ElemPile() {
        bloc1 = (rand() % 4) + 1;
        if (((rand() % 10) + 1) > 7) {
            bloc1 += 4;
            type1 = 2;
        } else
            type1 = 1;

        bloc2 = (rand() % 4) + 1;
        if (((rand() % 10) + 1) > 7) {
            bloc2 += 4;
            type2 = 2;
        } else
            type2 = 1;
    }

    int Bloc1() {
        return bloc1;
    }

    int Bloc2() {
        return bloc2;
    }

    int Type1() {
        return type1;
    }

    int Type2() {
        return type2;
    }
private:
    int bloc1;
    int bloc2;
    int type1;
    int type2;
};

class Pile {
public:
    Pile() {
        Init_Pile();
    }

    void Init_Pile() {
        le_vecteur.clear();
        le_vecteur.push_back(ElemPile());
    }

    Pile(const Pile& v) {
        le_vecteur = v.le_vecteur;
    }

    Pile& operator=(const Pile& v) {
        le_vecteur = v.le_vecteur;
        return *this;
    }

    ElemPile Donne(unsigned int no) {
        if (no >= le_vecteur.size() - 1)
            le_vecteur.push_back(ElemPile());
        return le_vecteur[no];
    }
private:
    vector<ElemPile> le_vecteur;
};
#endif
