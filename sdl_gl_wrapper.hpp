#ifndef SDL_GL_WRAPPER_HPP
#define SDL_GL_WRAPPER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
#include "dimensions.hpp"
#include "position.hpp"

void init_GL();
void init(int SCREEN_WIDTH, int SCREEN_HEIGHT);
void drawRectangle(Dimensions dimensions, Position position);
void turnOnTextureMode(GLuint texture);
void turnOffTextureMode();
void drawTexturedRectangle(Dimensions dimensions, Position position);
SDL_Keycode intToSDLKey(int keyNumber);
SDL_Window* getScreen();

#endif // SDL_GL_WRAPPER_HPP
