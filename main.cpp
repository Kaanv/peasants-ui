#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <vector>
#include <memory>
#include "textures.hpp"
#include "text.hpp"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

struct Resolution
{
    int x;
    int y;
};

void init_GL()
{
    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (glGetError() != GL_NO_ERROR) throw std::exception();
}

SDL_Surface* init(int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
    SDL_Surface *screen;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) throw std::exception();

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);

    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_OPENGL);
    if (screen == NULL) throw std::exception();
    init_GL();
    if (TTF_Init() == -1) throw std::exception();
    SDL_WM_SetCaption("Peasants", NULL);
    return screen;
}

SDLKey intToSDLKey(int keyNumber)
{
    switch (keyNumber)
    {
        case 0: return SDLK_FIRST;
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
        case 160: return SDLK_WORLD_0;
        case 161: return SDLK_WORLD_1;
        case 162: return SDLK_WORLD_2;
        case 163: return SDLK_WORLD_3;
        case 164: return SDLK_WORLD_4;
        case 165: return SDLK_WORLD_5;
        case 166: return SDLK_WORLD_6;
        case 167: return SDLK_WORLD_7;
        case 168: return SDLK_WORLD_8;
        case 169: return SDLK_WORLD_9;
        case 170: return SDLK_WORLD_10;
        case 171: return SDLK_WORLD_11;
        case 172: return SDLK_WORLD_12;
        case 173: return SDLK_WORLD_13;
        case 174: return SDLK_WORLD_14;
        case 175: return SDLK_WORLD_15;
        case 176: return SDLK_WORLD_16;
        case 177: return SDLK_WORLD_17;
        case 178: return SDLK_WORLD_18;
        case 179: return SDLK_WORLD_19;
        case 180: return SDLK_WORLD_20;
        case 181: return SDLK_WORLD_21;
        case 182: return SDLK_WORLD_22;
        case 183: return SDLK_WORLD_23;
        case 184: return SDLK_WORLD_24;
        case 185: return SDLK_WORLD_25;
        case 186: return SDLK_WORLD_26;
        case 187: return SDLK_WORLD_27;
        case 188: return SDLK_WORLD_28;
        case 189: return SDLK_WORLD_29;
        case 190: return SDLK_WORLD_30;
        case 191: return SDLK_WORLD_31;
        case 192: return SDLK_WORLD_32;
        case 193: return SDLK_WORLD_33;
        case 194: return SDLK_WORLD_34;
        case 195: return SDLK_WORLD_35;
        case 196: return SDLK_WORLD_36;
        case 197: return SDLK_WORLD_37;
        case 198: return SDLK_WORLD_38;
        case 199: return SDLK_WORLD_39;
        case 200: return SDLK_WORLD_40;
        case 201: return SDLK_WORLD_41;
        case 202: return SDLK_WORLD_42;
        case 203: return SDLK_WORLD_43;
        case 204: return SDLK_WORLD_44;
        case 205: return SDLK_WORLD_45;
        case 206: return SDLK_WORLD_46;
        case 207: return SDLK_WORLD_47;
        case 208: return SDLK_WORLD_48;
        case 209: return SDLK_WORLD_49;
        case 210: return SDLK_WORLD_50;
        case 211: return SDLK_WORLD_51;
        case 212: return SDLK_WORLD_52;
        case 213: return SDLK_WORLD_53;
        case 214: return SDLK_WORLD_54;
        case 215: return SDLK_WORLD_55;
        case 216: return SDLK_WORLD_56;
        case 217: return SDLK_WORLD_57;
        case 218: return SDLK_WORLD_58;
        case 219: return SDLK_WORLD_59;
        case 220: return SDLK_WORLD_60;
        case 221: return SDLK_WORLD_61;
        case 222: return SDLK_WORLD_62;
        case 223: return SDLK_WORLD_63;
        case 224: return SDLK_WORLD_64;
        case 225: return SDLK_WORLD_65;
        case 226: return SDLK_WORLD_66;
        case 227: return SDLK_WORLD_67;
        case 228: return SDLK_WORLD_68;
        case 229: return SDLK_WORLD_69;
        case 230: return SDLK_WORLD_70;
        case 231: return SDLK_WORLD_71;
        case 232: return SDLK_WORLD_72;
        case 233: return SDLK_WORLD_73;
        case 234: return SDLK_WORLD_74;
        case 235: return SDLK_WORLD_75;
        case 236: return SDLK_WORLD_76;
        case 237: return SDLK_WORLD_77;
        case 238: return SDLK_WORLD_78;
        case 239: return SDLK_WORLD_79;
        case 240: return SDLK_WORLD_80;
        case 241: return SDLK_WORLD_81;
        case 242: return SDLK_WORLD_82;
        case 243: return SDLK_WORLD_83;
        case 244: return SDLK_WORLD_84;
        case 245: return SDLK_WORLD_85;
        case 246: return SDLK_WORLD_86;
        case 247: return SDLK_WORLD_87;
        case 248: return SDLK_WORLD_88;
        case 249: return SDLK_WORLD_89;
        case 250: return SDLK_WORLD_90;
        case 251: return SDLK_WORLD_91;
        case 252: return SDLK_WORLD_92;
        case 253: return SDLK_WORLD_93;
        case 254: return SDLK_WORLD_94;
        case 255: return SDLK_WORLD_95;
        case 256: return SDLK_KP0;
        case 257: return SDLK_KP1;
        case 258: return SDLK_KP2;
        case 259: return SDLK_KP3;
        case 260: return SDLK_KP4;
        case 261: return SDLK_KP5;
        case 262: return SDLK_KP6;
        case 263: return SDLK_KP7;
        case 264: return SDLK_KP8;
        case 265: return SDLK_KP9;
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
        case 300: return SDLK_NUMLOCK;
        case 301: return SDLK_CAPSLOCK;
        case 302: return SDLK_SCROLLOCK;
        case 303: return SDLK_RSHIFT;
        case 304: return SDLK_LSHIFT;
        case 305: return SDLK_RCTRL;
        case 306: return SDLK_LCTRL;
        case 307: return SDLK_RALT;
        case 308: return SDLK_LALT;
        case 309: return SDLK_RMETA;
        case 310: return SDLK_LMETA;
        case 311: return SDLK_LSUPER;
        case 312: return SDLK_RSUPER;
        case 313: return SDLK_MODE;
        case 314: return SDLK_COMPOSE;
        case 315: return SDLK_HELP;
        case 316: return SDLK_PRINT;
        case 317: return SDLK_SYSREQ;
        case 318: return SDLK_BREAK;
        case 319: return SDLK_MENU;
        case 320: return SDLK_POWER;
        case 321: return SDLK_EURO;
        case 322: return SDLK_UNDO;
        default: return SDLK_F15;
    }
}

enum PollingPlace
{
    PollingPlace_MainMenu,
    PollingPlace_Exit
};

struct Position
{
    double x, y;
};

struct Dimensions
{
    double width, height;
};

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

class Button
{
public:
    Button(Dimensions dimensions, Position positon);
    void update();
    void handleMotion(const Position &position);
private:
    void drawBorder();
    void drawVerticalBorders();
    void drawHorizontalBorders();
    bool isInside(const Position &position);

    Dimensions dimensions;
    Position position;
    TTF_Font* font;
    SDL_Color textColor;
    GLuint lineTexture;
    bool isUnderMouseMotion;
};

Button::Button(Dimensions dimensions, Position positon)
    : font(TTF_OpenFont("Fonts//font.ttf", 40)),
      textColor({255, 255, 255, 0}),
      lineTexture(getTexture("Images//buttonline.png")),
      isUnderMouseMotion(false)
{
    this->dimensions = dimensions;
    this->position = positon;
}

void Button::update()
{
    if (isUnderMouseMotion) glColor3f(0.0, 0.2, 0.4);
    else glColor3f(0.0, 0.2, 0.6);

    drawRectangle({dimensions.width, dimensions.height},
                  {position.x, position.y});

    drawBorder();

    SDL_GL_RenderText("New game",
                      font,
                      textColor,
                      position.x + dimensions.width/2.0,
                      position.y - dimensions.height/4.0,
                      dimensions.height);
}

void Button::drawBorder()
{
    turnOnTextureMode(lineTexture);
    drawVerticalBorders();
    drawHorizontalBorders();
    turnOffTextureMode();
}

void Button::drawVerticalBorders()
{
    const int fragments = 20;
    const double borderWidth = dimensions.width / 30;
    const double fragmentHeight = dimensions.height / fragments;

    for (int i = 0; i < fragments; i++)
    {
        drawTexturedRectangle(
            {borderWidth, fragmentHeight},
            {position.x, position.y - i * fragmentHeight});
    }

    for (int i = 0; i < fragments; i++)
    {
        drawTexturedRectangle(
            {borderWidth, fragmentHeight},
            {position.x + dimensions.width - borderWidth,
             position.y - i * fragmentHeight});
    }
}

void Button::drawHorizontalBorders()
{
    const int fragments = 40;
    const double borderHeight = dimensions.height / 7;
    const double fragmentWidth = dimensions.width / fragments;

    for (int i = 0; i < fragments; i++)
    {
        drawTexturedRectangle(
            {fragmentWidth, borderHeight},
            {position.x + i * fragmentWidth, position.y});
    }

    for (int i = 0; i < fragments; i++)
    {
        drawTexturedRectangle(
            {fragmentWidth, borderHeight},
            {position.x + i * fragmentWidth,
             position.y - dimensions.height + borderHeight});
    }
}

void Button::handleMotion(const Position &position)
{
    isUnderMouseMotion = isInside(position);
}

bool Button::isInside(const Position &position)
{
    return position.x >= this->position.x and
           position.x <= this->position.x + this->dimensions.width and
           position.y <= this->position.y and
           position.y >= this->position.y - this->dimensions.height;
}

class Menu
{
public:
    virtual PollingPlace enter() = 0;
};

class MainMenu : public Menu
{
public:
    MainMenu(int x, int y);
    PollingPlace enter();
private:
    PollingPlace startEventPoll();
    void updateScreen();
    void updateButtonsOnMotion(int x, int y);

    std::vector<Button> buttons;
    SDL_Event event;
    int x, y;
};

MainMenu::MainMenu(int x, int y)
{
    this->x = x;
    this->y = y;
    buttons.push_back(Button({0.7, 0.125}, {-0.35, 0.425}));
}

PollingPlace MainMenu::enter()
{
    PollingPlace currentPlace = PollingPlace_MainMenu;
    while (currentPlace == PollingPlace_MainMenu)
    {
        currentPlace = startEventPoll();
    }
    return currentPlace;
}

PollingPlace MainMenu::startEventPoll()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) return PollingPlace_Exit;
        else if (event.type == SDL_ACTIVEEVENT &&
                 event.active.state & SDL_APPACTIVE &&
                 event.active.gain != 0) updateScreen();
        else if (event.type == SDL_VIDEOEXPOSE) updateScreen();
        else if (event.type == SDL_MOUSEMOTION)
        {
            updateButtonsOnMotion(event.motion.x, event.motion.y);
        }
    }
    return PollingPlace_MainMenu;
}

void MainMenu::updateScreen()
{
    for (auto& button : buttons)
    {
        button.update();
    }

    SDL_GL_SwapBuffers();
}

void MainMenu::updateButtonsOnMotion(int x, int y)
{
    for (auto& button : buttons)
    {
        button.handleMotion({static_cast<float>(x) * 2.0 / SCREEN_WIDTH - 1.0,
                             static_cast<float>(-y) * 2.0 / SCREEN_HEIGHT + 1.0});
    }
    updateScreen();
}

int main()
{
    Resolution resolution{800, 600};
    SDL_Surface *screen = init(resolution.x, resolution.y);
    PollingPlace currentPlace = PollingPlace_MainMenu;
    MainMenu mainMenu(10, 10);

    while (currentPlace != PollingPlace_Exit)
    {
        currentPlace = mainMenu.enter();
    }

    delete screen;
    return 0;
}
