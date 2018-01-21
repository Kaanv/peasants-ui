#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "SDL/SDL_ttf.h"
#include <math.h>

#ifndef text_h
#define text_h

int nextpoweroftwo(int x);
void SDL_GL_RenderText(const char *text, 
                      TTF_Font *font,
                      SDL_Color color,
                      float x, float y, float plusH);
#endif
