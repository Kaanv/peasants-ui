#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string>
#include <SDL/SDL_opengl.h>

#ifndef textures_h
#define textures_h

SDL_Surface *load_image( std::string filename );
GLuint getTexture(std::string filename);

#endif
