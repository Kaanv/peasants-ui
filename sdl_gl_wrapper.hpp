#ifndef SDL_GL_WRAPPER_HPP
#define SDL_GL_WRAPPER_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include "dimensions.hpp"
#include "position.hpp"

void init_GL();
SDL_Surface* init(int SCREEN_WIDTH, int SCREEN_HEIGHT);
void drawRectangle(Dimensions dimensions, Position position);
void turnOnTextureMode(GLuint texture);
void turnOffTextureMode();
void drawTexturedRectangle(Dimensions dimensions, Position position);
SDLKey intToSDLKey(int keyNumber);

#endif // SDL_GL_WRAPPER_HPP
