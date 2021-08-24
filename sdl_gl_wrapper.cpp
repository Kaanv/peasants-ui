#include "sdl_gl_wrapper.hpp"
#include <exception>

SDL_Window* screen;

void init_GL()
{
    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (glGetError() != GL_NO_ERROR) throw std::exception();
}

void init(int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) throw std::exception();

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);

    screen = SDL_CreateWindow("Peasants", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if (screen == nullptr) throw std::exception();
    init_GL();
    if (TTF_Init() == -1) throw std::exception();
}


void drawRectangle(Dimensions dimensions, Position position)
{
    glBegin(GL_QUADS);
    glVertex3f(position.x, position.y, 0.0);
    glVertex3f(position.x + dimensions.width, position.y, 0.0);
    glVertex3f(position.x + dimensions.width,
               position.y - dimensions.height, 0.0);
    glVertex3f(position.x, position.y - dimensions.height, 0.0);
    glEnd();
}

void turnOnTextureMode(GLuint texture)
{
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void turnOffTextureMode()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable( GL_TEXTURE_2D );
}

void drawTexturedRectangle(Dimensions dimensions, Position position)
{
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(position.x, position.y, 0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(position.x + dimensions.width, position.y, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(position.x + dimensions.width,
               position.y - dimensions.height, 0.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(position.x, position.y - dimensions.height, 0.0);
    glEnd();
}

SDL_Window* getScreen()
{
    return screen;
}

SDL_Keycode intToSDLKey(int keyNumber)
{
    switch (keyNumber)
    {
        case 8: return SDLK_BACKSPACE;
        case 9: return SDLK_TAB;
        case 12: return SDLK_CLEAR;
        case 13: return SDLK_RETURN;
        case 19: return SDLK_PAUSE;
        case 27: return SDLK_ESCAPE;
        case 32: return SDLK_SPACE;
        case 33: return SDLK_EXCLAIM;
        case 34: return SDLK_QUOTEDBL;
        case 35: return SDLK_HASH;
        case 36: return SDLK_DOLLAR;
        case 38: return SDLK_AMPERSAND;
        case 39: return SDLK_QUOTE;
        case 40: return SDLK_LEFTPAREN;
        case 41: return SDLK_RIGHTPAREN;
        case 42: return SDLK_ASTERISK;
        case 43: return SDLK_PLUS;
        case 44: return SDLK_COMMA;
        case 45: return SDLK_MINUS;
        case 46: return SDLK_PERIOD;
        case 47: return SDLK_SLASH;
        case 48: return SDLK_0;
        case 49: return SDLK_1;
        case 50: return SDLK_2;
        case 51: return SDLK_3;
        case 52: return SDLK_4;
        case 53: return SDLK_5;
        case 54: return SDLK_6;
        case 55: return SDLK_7;
        case 56: return SDLK_8;
        case 57: return SDLK_9;
        case 58: return SDLK_COLON;
        case 59: return SDLK_SEMICOLON;
        case 60: return SDLK_LESS;
        case 61: return SDLK_EQUALS;
        case 62: return SDLK_GREATER;
        case 63: return SDLK_QUESTION;
        case 64: return SDLK_AT;
        case 91: return SDLK_LEFTBRACKET;
        case 92: return SDLK_BACKSLASH;
        case 93: return SDLK_RIGHTBRACKET;
        case 94: return SDLK_CARET;
        case 95: return SDLK_UNDERSCORE;
        case 96: return SDLK_BACKQUOTE;
        case 97: return SDLK_a;
        case 98: return SDLK_b;
        case 99: return SDLK_c;
        case 100: return SDLK_d;
        case 101: return SDLK_e;
        case 102: return SDLK_f;
        case 103: return SDLK_g;
        case 104: return SDLK_h;
        case 105: return SDLK_i;
        case 106: return SDLK_j;
        case 107: return SDLK_k;
        case 108: return SDLK_l;
        case 109: return SDLK_m;
        case 110: return SDLK_n;
        case 111: return SDLK_o;
        case 112: return SDLK_p;
        case 113: return SDLK_q;
        case 114: return SDLK_r;
        case 115: return SDLK_s;
        case 116: return SDLK_t;
        case 117: return SDLK_u;
        case 118: return SDLK_v;
        case 119: return SDLK_w;
        case 120: return SDLK_x;
        case 121: return SDLK_y;
        case 122: return SDLK_z;
        case 127: return SDLK_DELETE;
        case 266: return SDLK_KP_PERIOD;
        case 267: return SDLK_KP_DIVIDE;
        case 268: return SDLK_KP_MULTIPLY;
        case 269: return SDLK_KP_MINUS;
        case 270: return SDLK_KP_PLUS;
        case 271: return SDLK_KP_ENTER;
        case 272: return SDLK_KP_EQUALS;
        case 273: return SDLK_UP;
        case 274: return SDLK_DOWN;
        case 275: return SDLK_RIGHT;
        case 276: return SDLK_LEFT;
        case 277: return SDLK_INSERT;
        case 278: return SDLK_HOME;
        case 279: return SDLK_END;
        case 280: return SDLK_PAGEUP;
        case 281: return SDLK_PAGEDOWN;
        case 282: return SDLK_F1;
        case 283: return SDLK_F2;
        case 284: return SDLK_F3;
        case 285: return SDLK_F4;
        case 286: return SDLK_F5;
        case 287: return SDLK_F6;
        case 288: return SDLK_F7;
        case 289: return SDLK_F8;
        case 290: return SDLK_F9;
        case 291: return SDLK_F10;
        case 292: return SDLK_F11;
        case 293: return SDLK_F12;
        case 294: return SDLK_F13;
        case 295: return SDLK_F14;
        case 296: return SDLK_F15;
        case 300: return SDLK_NUMLOCKCLEAR;
        case 301: return SDLK_CAPSLOCK;
        case 302: return SDLK_SCROLLLOCK;
        case 303: return SDLK_RSHIFT;
        case 304: return SDLK_LSHIFT;
        case 305: return SDLK_RCTRL;
        case 306: return SDLK_LCTRL;
        case 307: return SDLK_RALT;
        case 308: return SDLK_LALT;
        case 313: return SDLK_MODE;
        case 315: return SDLK_HELP;
        case 317: return SDLK_SYSREQ;
        case 319: return SDLK_MENU;
        case 320: return SDLK_POWER;
        case 322: return SDLK_UNDO;
        default: return SDLK_F15;
    }
}
