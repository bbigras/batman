#ifndef IMAGE_H
#define IMAGE_H
//test
class Image
{
public:
    Image(string fichier = "") {
        if (fichier != "")
            Charger(fichier);
        else
            image = NULL;
    }

    ~Image() {
        SDL_FreeSurface(image);
    }

    void Charger(string filename) {
        //The image that's loaded
        SDL_Surface* loadedImage = NULL;

        //The optimized image that will be used
        SDL_Surface* optimizedImage = NULL;

        //Load the image
        loadedImage = IMG_Load(filename.c_str());

        //If the image loaded
        if (loadedImage != NULL) {
            //Create an optimized image
            optimizedImage = SDL_DisplayFormat(loadedImage);

            //Free the old image
            SDL_FreeSurface(loadedImage);
        }

        //Return the optimized image
        image = optimizedImage;
    }

    void Affiche(int x, int y, SDL_Surface* destination) {
        //Temporary rectangle to hold the offsets
        SDL_Rect offset;

        //Get the offsets
        offset.x = x;
        offset.y = y;

        //Blit the surface
        SDL_BlitSurface(image, NULL, destination, &offset);
    }

private:
    SDL_Surface *image;
};
#endif
