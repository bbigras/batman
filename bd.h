#ifndef BD_H
#define BD_H

#include <sqlite3.h>

class Personne {
public:
    Personne() {}

    Personne(string n, int p) : nom(n), pointage(p) {}

    string Nom() {
        return nom;
    }

    int Pointage() {
        return pointage;
    }
private:
    string nom;
    int pointage;
};


class Bd {
public:
    Bd() {
        rc = sqlite3_open("labd.bd", &db);
        if (rc != SQLITE_OK)
	    cerr << "Error opening SQLite3 database: labd.bd\n";
        zErrMsg = 0;
    }

    void Init_Bd() {
        Faitle("DROP TABLE Pointages");
        Faitle("DROP TABLE Options");
        Faitle("DROP TABLE Controles");

        Faitle("CREATE TABLE Pointages (nom varchar(32), pointage int);");
        Faitle("CREATE TABLE Options (variable varchar(32), valeur varchar(32));");

        Faitle("CREATE TABLE Controles (joueur int, touche int, valeur int);");

        Faitle("INSERT INTO Pointages VALUES ('Chuck1', 10)");
        Faitle("INSERT INTO Pointages VALUES ('Chuck2', 20)");
        Faitle("INSERT INTO Pointages VALUES ('Chuck3', 30)");
        Faitle("INSERT INTO Pointages VALUES ('Chuck4', 40)");
        Faitle("INSERT INTO Pointages VALUES ('Chuck5', 50)");
        Faitle("INSERT INTO Pointages VALUES ('Chuck6', 60)");
        Faitle("INSERT INTO Pointages VALUES ('Chuck7', 70)");
        Faitle("INSERT INTO Pointages VALUES ('Chuck8', 80)");
        Faitle("INSERT INTO Pointages VALUES ('Chuck9', 90)");
        Faitle("INSERT INTO Pointages VALUES ('Chuck10', 100)");
        Faitle("INSERT INTO Pointages VALUES ('Chuck11', 110)");


        Faitle("INSERT INTO Controles VALUES (1, 1, " + Int_Vers_String(SDLK_w) + ")");
        Faitle("INSERT INTO Controles VALUES (1, 2, " + Int_Vers_String(SDLK_a) + ")");
        Faitle("INSERT INTO Controles VALUES (1, 3, " + Int_Vers_String(SDLK_d) + ")");
        Faitle("INSERT INTO Controles VALUES (1, 4, " + Int_Vers_String(SDLK_s) + ")");
        Faitle("INSERT INTO Controles VALUES (1, 5, " + Int_Vers_String(SDLK_g) + ")");
        Faitle("INSERT INTO Controles VALUES (1, 6, " + Int_Vers_String(SDLK_h) + ")");
        Faitle("INSERT INTO Controles VALUES (1, 7, " + Int_Vers_String(SDLK_RETURN) + ")");

        Faitle("INSERT INTO Controles VALUES (2, 1, " + Int_Vers_String(SDLK_UP) + ")");
        Faitle("INSERT INTO Controles VALUES (2, 2, " + Int_Vers_String(SDLK_LEFT) + ")");
        Faitle("INSERT INTO Controles VALUES (2, 3, " + Int_Vers_String(SDLK_RIGHT) + ")");
        Faitle("INSERT INTO Controles VALUES (2, 4, " + Int_Vers_String(SDLK_DOWN) + ")");
        Faitle("INSERT INTO Controles VALUES (2, 5, " + Int_Vers_String(SDLK_m) + ")");
        Faitle("INSERT INTO Controles VALUES (2, 6, " + Int_Vers_String(SDLK_RCTRL) + ")");
        Faitle("INSERT INTO Controles VALUES (2, 7, " + Int_Vers_String(SDLK_PAGEDOWN) + ")");
    }

    int Controle(string joueur, int touche) {
        char *zErrMsg;
        char **result;
        int nrow, ncol;

        string b = "SELECT valeur FROM Controles WHERE joueur = '" + joueur + "' and touche=" + Int_Vers_String(touche);

        do {
            rc = sqlite3_get_table(
                     db,              /* An open database */
                     b.c_str(),       /* SQL to be executed */
                     &result,       /* Result written to a char *[]  that this points to */
                     &nrow,             /* Number of result rows written here */
                     &ncol,          /* Number of result columns written here */
                     &zErrMsg          /* Error msg written here */
                 );
        } while (rc == SQLITE_BUSY);
        if (rc == SQLITE_OK && nrow > 0)
            return atoi(result[1]);
        return -1;
    }

    string Lit(string variable) {
        char *zErrMsg;
        char **result;
        int nrow, ncol;

        string b = "SELECT valeur FROM Options WHERE variable = '" + variable + "'";

        do {
            rc = sqlite3_get_table(
                     db,              /* An open database */
                     b.c_str(),       /* SQL to be executed */
                     &result,       /* Result written to a char *[]  that this points to */
                     &nrow,             /* Number of result rows written here */
                     &ncol,          /* Number of result columns written here */
                     &zErrMsg          /* Error msg written here */
                 );
        } while (rc == SQLITE_BUSY);
        if (rc == SQLITE_OK && nrow > 0)
            return result[1];
        return "";
    }

    void Ecrit(string variable, string valeur) {
        Faitle("INSERT OR REPLACE INTO Options VALUES ('" + variable + "', '" + valeur + "')");
    }

    void Faitle(string requete) {
        rc = sqlite3_exec(db, requete.c_str(), NULL, 0, &zErrMsg);

        // while != SQLITE_BUSY, mais il ne faut pas que ça arrive pendant le jeux!
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Erreur SQL: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
    }

    // retourne vrai si un record a ete battu!
    bool Check_Record(int pointage) {
        char *zErrMsg;
        char **result;
        int nrow, ncol;

        ostringstream pp;
        pp << pointage;

        string b = "SELECT * FROM Pointages WHERE Pointage >= " + pp.str() + " ORDER BY Pointage DESC;";

        do {
            rc = sqlite3_get_table(
                     db,              /* An open database */
                     b.c_str(),       /* SQL to be executed */
                     &result,       /* Result written to a char *[]  that this points to */
                     &nrow,             /* Number of result rows written here */
                     &ncol,          /* Number of result columns written here */
                     &zErrMsg          /* Error msg written here */
                 );
        } while (rc == SQLITE_BUSY);
        if (rc == SQLITE_OK && nrow > 0) {
            if (nrow < 10)
                return true;
        }

        return false;
    }

    void Top10(Personne chuck[10]) {
        char *zErrMsg;
        char **result;
        int nrow, ncol;

        string b = "SELECT * FROM Pointages ORDER BY Pointage DESC LIMIT 10;";
        do {
            rc = sqlite3_get_table(
                     db,              /* An open database */
                     b.c_str(),       /* SQL to be executed */
                     &result,       /* Result written to a char *[]  that this points to */
                     &nrow,             /* Number of result rows written here */
                     &ncol,          /* Number of result columns written here */
                     &zErrMsg          /* Error msg written here */
                 );
        } while (rc == SQLITE_BUSY);
        if (rc == SQLITE_OK && nrow > 0) {

            for (int i = 1; i < nrow + 1; ++i) {
                string na = result[(i*ncol)+0];
                int pa = atoi(result[(i*ncol)+1]);
                //cout << "l " << na << " . " << pa << endl;
                chuck[i-1] = Personne(na, pa);
            }
        }
    }

    ~Bd() {
        sqlite3_close(db);
    }
private:
    int rc;
    sqlite3 *db;
    char *zErrMsg;
};
#endif
