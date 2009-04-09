#ifndef NET_H
#define NET_H

#include <SDL/SDL_net.h>

class Connection
{
public:
    Connection() {
        int t = 0;
        SDLNet_Init();

        tampon = "";
        tampon_tab = "";
        tampon_tomb = "";
        tampon_point = "";
        maitre_mechant = "";
        pointage = "";
        tampon_inj = "";
        cinj = 0;
        type = t;
        host = "";

        for (int j = 12; j >= 0; j--) {
            for (int i = 0; i < 6; i++) {
                array[j][i] = 0;
                array9[j][i] = 0;
            }
        }

        if (type == 0) {
            // Le port à utiliser : par défaut, 80
            int port = 8081;

            // Ecoute sur le port
            IPaddress adresse;

            if (SDLNet_ResolveHost(&adresse, NULL, port) == -1) {
                fprintf(stderr, "Impossible de résoudre le nom d'hôte : %s\n",
                        SDLNet_GetError());
                exit(1);
            }

            serveur = SDLNet_TCP_Open(&adresse);
            if (!serveur) {
                fprintf(stderr, "Impossible de creer la socket serveur !\n"
                        "Le port %d est-il libre ?\n%s\n", port, SDLNet_GetError());
                SDLNet_TCP_Close(serveur);
                exit(1);
            }

            // Attend une connection
            socket = NULL;
            socSet1 = SDLNet_AllocSocketSet(1);
        } else {
            // Adresse en paramètre
            host = "localhost";
            //host = "172.16.125.69";

            // Connecte sur le port 80
            IPaddress adresse;

            SDLNet_ResolveHost(&adresse, host.c_str(), 8081);

            // Créé la socket client
            socket = SDLNet_TCP_Open(&adresse);

            // Connection échouée
            if (!socket) {
                std::cout << "Impossible de se connecter a " << host << " port 80\n";
                SDLNet_TCP_Close(socket);
                exit(0);
            }
            socSet1 = SDLNet_AllocSocketSet(1);
            SDLNet_TCP_AddSocket(socSet1, socket);
        }

        chuck = 0;
        actif = true;
    }

    void Syncro() {
        if (socket && chuck == 0) {
            int simlag;
            float lag;
            Uint32 /*ipaddr,*/ t[2], base, now;

            // get the desired simulated lag from the commandline
            simlag = 100; //strtol("100", NULL, 0);
//cout << "c1\n";
            if (type == 0) {
                //          cout << "c2\n";
                base = SDL_GetTicks();
                t[0] = SDL_SwapBE32(0);
                t[1] = SDL_SwapBE32(~0L);
                SDLNet_TCP_Send(socket, t, sizeof(t));
//cout << "c4\n";
                SDLNet_TCP_Recv(socket, t, sizeof(t));
                now = SDL_GetTicks() - base;
                t[0] = SDL_SwapBE32(t[0]);
                t[1] = SDL_SwapBE32(t[1]);
                lag = (now - t[0]) / 2.0f;
                printf("S1) now=%u t[0]=%u t[1]=%u (lag=%f)\n",
                       now , t[0], t[1], lag);

                t[0] = SDL_SwapBE32(SDL_GetTicks() - base + (Uint32)lag);
                t[1] = SDL_SwapBE32(t[1]);
                SDLNet_TCP_Send(socket, t, sizeof(t));

                SDLNet_TCP_Recv(socket, t, sizeof(t));
                now = SDL_GetTicks() - base;
                t[0] = SDL_SwapBE32(t[0]);
                t[1] = SDL_SwapBE32(t[1]);
                lag += (Sint32)t[0];
                printf("S2) now=%u t[0]=%d t[1]=%u (lag=%f)\n",
                       now , t[0], t[1], lag);

                t[0] = SDL_GetTicks() - base + (Uint32) lag;
                t[0] = SDL_SwapBE32(t[0]);
                t[1] = ((Sint32)(now - t[1])) / 2;
                t[1] = SDL_SwapBE32(t[1]);
                SDLNet_TCP_Send(socket, t, sizeof(t));
                cout << "chuk " << chuck++ << endl;
            } else {
                SDLNet_TCP_Recv(socket, t, sizeof(t));
                if (simlag) SDL_Delay(simlag);
                base = SDL_GetTicks();
                t[0] = SDL_SwapBE32(t[0]);
                printf("C1) t[0]=%u\n", t[0]);


                t[0] = SDL_SwapBE32(t[0]);
                t[1] = SDL_SwapBE32(SDL_GetTicks() - base);
                if (simlag) SDL_Delay(simlag);
                SDLNet_TCP_Send(socket, t, sizeof(t));

                SDLNet_TCP_Recv(socket, t, sizeof(t));
                if (simlag) SDL_Delay(simlag);
                now = SDL_GetTicks() - base;
                t[0] = SDL_SwapBE32(t[0]);
                t[1] = SDL_SwapBE32(t[1]);
                lag = (now - t[1]) / 2.0f;
                base -= (Uint32) lag;
                now += (Uint32) lag;
                printf("C2) now=%u t[0]=%u t[1]=%u (lag=%f)\n",
                       now, t[0], t[1], lag);

                t[0] = ((Sint32)(now - t[0])) / 2;
                t[0] = SDL_SwapBE32(t[0]);
                t[1] = (SDL_GetTicks() - base + (Uint32) lag);
                t[1] = SDL_SwapBE32(t[1]);
                if (simlag) SDL_Delay(simlag);
                SDLNet_TCP_Send(socket, t, sizeof(t));

                SDLNet_TCP_Recv(socket, t, sizeof(t));
                if (simlag) SDL_Delay(simlag);
                now = SDL_GetTicks() - base;
                t[0] = SDL_SwapBE32(t[0]);
                t[1] = SDL_SwapBE32(t[1]);
                lag += (Sint32)t[1];
                base -= (Sint32)t[1];
                now += (Sint32)t[1];
                printf("C3) now=%u t[0]=%u t[1]=%d (lag=%f)\n",
                       now, t[0], t[1], lag);
                cout << "chuk " << chuck++ << endl;
            }
        }

    }

    ~Connection() {
        SDLNet_TCP_Close(socket);
        if (socket)
            SDLNet_TCP_Close(serveur);
    }

    bool Actif() {
        return actif;
    }

    bool Ok() {
        return (socket);
    }

    void Recoie() {
        //Syncro();

        if (type == 0) { // Serveur
            if (socket == NULL) {

                while (!socket)
                    socket = SDLNet_TCP_Accept(serveur);
                SDLNet_TCP_AddSocket(socSet1, socket);
            }
        }

        if (socket) {
            char carac[255];

            if (SDLNet_CheckSockets(socSet1, 0)) {
                if (SDLNet_TCP_Recv(socket, &carac, 255) <= 0) {
                    // Fermeture de la connection
                    /*SDLNet_TCP_Close(serveur);
                    SDLNet_TCP_Close(socket);
                    exit(0);*/
                }

                tampon = carac;
                size_t index = tampon.find('T');
                size_t index1 = tampon.find('Z');
                size_t index2 = tampon.find('A');
                size_t index3 = tampon.find('B');
                size_t index4 = tampon.find('M');
                size_t index5 = tampon.find('N');
                size_t index6 = tampon.find('R');
                size_t index7 = tampon.find('C');

                bool nouv_tomb = false;
                bool nouv_point = false;
                bool nouv_inj = false;

                if (index != string::npos) { // Si ya un T
                    if (index1 != string::npos) // Si ya un Z
                        tampon_tab = tampon.substr(index + 1, index1 - index - 1);
                    else
                        tampon_tab = tampon.substr(index - 1);
                } else if (index1 != string::npos)
                    tampon_tab += tampon.substr(0, index1 - 1);

                if (index2 != string::npos) { // Si ya un A
                    if (index3 != string::npos) { // Si ya un B
                        tampon_tomb = tampon.substr(index2 + 1, index3 - index2 - 1);
                        nouv_tomb = true;
                    } else
                        tampon_tomb = tampon.substr(index2 + 1);
                } else if (index2 != string::npos) {
                    tampon_tomb += tampon.substr(0, index3 - 1);
                    nouv_tomb = true;
                }

                if (index4 != string::npos) { // Si ya un M
                    if (index5 != string::npos) { // Si ya un N
                        tampon_point = tampon.substr(index4 + 1, index5 - index4 - 1);
                        nouv_point = true;
                    } else
                        tampon_point = tampon.substr(index4 + 1);
                } else if (index4 != string::npos) {
                    tampon_point += tampon.substr(0, index5 - 1);
                    nouv_point = true;
                }

                if (tampon_tab.length() == 156) {
                    int j = 0;
                    int k = 0;
                    for (int i = 0; i < 78; i++) {
                        array[j][k] = atoi(tampon_tab.substr(i, 1).c_str());
                        k++;

                        if ((i + 1) % 6 == 0) {
                            j++;
                            k = 0;
                        }
                    }

                    j = 0;
                    k = 0;

                    for (int i = 78; i < 156; i++) {
                        array9[j][k] = atoi(tampon_tab.substr(i, 1).c_str());
                        k++;

                        if ((i + 1) % 6 == 0) {
                            j++;
                            k = 0;
                        }
                    }
                }

                if (index6 != string::npos) { // Si ya un R
                    if (index7 != string::npos) { // Si ya un C
                        tampon_inj = tampon.substr(index6 + 1, index7 - index6 - 1);
                        nouv_inj = true;
                    } else
                        tampon_inj = tampon.substr(index6 - 1);
                } else if (index7 != string::npos) {
                    tampon_inj += tampon.substr(0, index7 - 1);
                    nouv_inj = true;
                }

                if (nouv_tomb)
                    maitre_mechant = tampon_tomb;

                if (nouv_point)
                    pointage = tampon_point;

                if (nouv_inj)
                    cinj += atoi(tampon_inj.c_str());
            }
        }
    }

    int Inj() {
        int tam = cinj;
        cinj = 0;
        return tam;
    }

    void Envoie(string requete, int lequel) {
        if (socket) {
            switch (lequel) {
            case 1:
                requete = "T" + requete + "Z\0";
                break;
            case 2:
                requete = "A" + requete + "B\0";
                break;
            case 3:
                requete = "M" + requete + "N\0";
                break;
            case 4:
                requete = "R" + requete + "C\0";
                break;
                //default:
            }

            SDLNet_TCP_Send(socket, requete.c_str(), strlen(requete.c_str()));
        }
    }

    void ExporteTableau(int array2[13][6], int array8[13][6]) {
        for (int i = 0; i < 13; i++) {
            for (int j = 0; j < 6; j++)
                array2[i][j] = array[i][j];
        }

        for (int i = 0; i < 13; i++) {
            for (int j = 0; j < 6; j++)
                array8[i][j] = array9[i][j];
        }
    }

    string ExporteTomb() {
        return maitre_mechant;
    }

    string ExportePoint() {
        return pointage;
    }
private:
    TCPsocket serveur, socket;
    SDLNet_SocketSet socSet1;
    int array[13][6];
    int array9[13][6];
    string tampon;
    string tampon_tab;
    string tampon_tomb;
    string tampon_point;
    string maitre_mechant;
    string pointage;
    string tampon_inj;
    int cinj;
    int type;
    string host;
    int chuck;
    bool actif;
};
#endif
