#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <SDL2/SDL_opengl.h>

#ifndef textures_h
#define textures_h

SDL_Surface *load_image( std::string filename );
GLuint getTexture(std::string filename);

#endif
