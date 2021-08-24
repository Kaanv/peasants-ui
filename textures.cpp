#include "textures.hpp"
#include "sdl_gl_wrapper.hpp"

using namespace std;

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    SDL_Surface *window_surface = SDL_GetWindowSurface(getScreen());

    //If the image loaded
    if( loadedImage != nullptr )
    {
        //Create an optimized surface
        optimizedImage = SDL_ConvertSurface( loadedImage, window_surface->format, 0 );

        //Free the old surface
        SDL_FreeSurface( loadedImage );
    }
    //Return the optimized surface
    return optimizedImage;
}
GLuint getTexture(std::string filename)
{
    GLuint texture;			// This is a handle to our texture object
    SDL_Surface *surface;	// This surface will tell us the details of the image
    GLenum texture_format;
    GLint  nOfColors;

    if ( (surface = load_image(filename.c_str())) )
    {

        // get the number of channels in the SDL surface
        nOfColors = surface->format->BytesPerPixel;
        if (nOfColors == 4)     // contains an alpha channel
        {
            if (surface->format->Rmask == 0x000000ff)
                texture_format = GL_RGBA;
            else
                texture_format = GL_BGRA;
        }
        else if (nOfColors == 3)       // no alpha channel
        {
            if (surface->format->Rmask == 0x000000ff)
                texture_format = GL_RGB;
            else
                texture_format = GL_BGR;
        }
        else
        {
            printf("warning: the image is not truecolor..  this will probably break\n");
            // this error should not go unhandled
        }

        // Have OpenGL generate a texture object handle for us
        glGenTextures( 1, &texture );

        // Bind the texture object
        glBindTexture( GL_TEXTURE_2D, texture );

        // Set the texture's stretching properties
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        // Edit the texture object's image data using the information SDL_Surface gives us
        glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                      texture_format, GL_UNSIGNED_BYTE, surface->pixels );
    }
    else
    {
        printf("SDL could not load image: %s\n", SDL_GetError());
    }
// Free the SDL_Surface only if it was successfully created
    if ( surface )
    {
        SDL_FreeSurface( surface );
    }

    return texture;
}
