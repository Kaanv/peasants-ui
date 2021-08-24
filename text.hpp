#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include "SDL2/SDL_ttf.h"
#include <math.h>

int nextpoweroftwo(int x);
void SDL_GL_RenderText(const char *text, 
                      TTF_Font *font,
                      SDL_Color color,
                      double x, double y, double plusH);
