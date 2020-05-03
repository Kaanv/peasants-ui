#include "text.hpp"

int nextpoweroftwo(int x)
{
    double logbase2 = log(x) / log(2);
    return static_cast<int>(round(pow(2,ceil(logbase2))));
}

void SDL_GL_RenderText(const char *text,
                       TTF_Font *font,
                       SDL_Color color,
                       double x, double y, double plusH)
{
    SDL_Surface *initial;
    SDL_Surface *intermediary;
    int w,h;
    GLuint texture;
    int width;
    double czarne;

    /* Use SDL_TTF to render our text */
    initial = TTF_RenderText_Blended(font, text, color);

    /* Convert the rendered text to a known format */
    w = nextpoweroftwo(initial->w);
    h = nextpoweroftwo(initial->h);

    double wNaH = static_cast<double>(w)/static_cast<double>(h);
    double plusW = wNaH * plusH ;
    width = w - initial->w;
    czarne = (static_cast<double>(width)/static_cast<double>(w)) * plusW;

    intermediary = SDL_CreateRGBSurface(0, w, h, 32,
                                        0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

    SDL_BlitSurface(initial, nullptr, intermediary, nullptr);

    /* Tell GL about our new texture */
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, intermediary->pixels );

    /* GL_NEAREST looks horrible, if scaled... */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* prepare to render our texture */
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1.0f, 1.0f, 1.0f);

    /* Required if you want alpha-blended textures (for our fonts) */
    glBlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_BLEND);

    /* Draw a quad at location */
    glBegin(GL_QUADS);
    /* Recall that the origin is in the lower-left corner
       That is why the TexCoords specify different corners
       than the Vertex coors seem to. */
    glTexCoord2d(0.0, 1.0);
    glVertex2d(x - (plusW-czarne)/2, y - plusH);
    glTexCoord2d(1.0, 1.0);
    glVertex2d(x + (plusW+czarne)/2, y - plusH);
    glTexCoord2d(1.0, 0.0);
    glVertex2d(x + (plusW+czarne)/2, y);
    glTexCoord2d(0.0, 0.0);
    glVertex2d(x - (plusW-czarne)/2, y);

    glEnd();

    glDisable(GL_BLEND);
    /* Bad things happen if we delete the texture before it finishes */
    glFinish();

    /* Clean up */
    SDL_FreeSurface(initial);
    SDL_FreeSurface(intermediary);
    glDeleteTextures(1, &texture);
}
