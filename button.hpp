#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "dimensions.hpp"
#include "position.hpp"
#include <string>
#include "sdl_gl_wrapper.hpp"

enum MenuButtonId
{
    ButtonId_NewGame,
    ButtonId_MainMenu,
    ButtonId_ExitGame,
    ButtonId_PassTurn,
    ButtonId_ThrowCards
};

class Button
{
public:
    Button(Dimensions dimensions,
           Position positon,
           std::string caption,
           int buttonId = 0);
    void draw();
    void updateMotion(const Position &position);
    void updateUnderClick();
    bool isClicked();
    int getButtonId();
    void forceDraw();
private:
    void drawBorder();
    void drawVerticalBorders();
    void drawHorizontalBorders();
    void renderCaption();
    bool isInside(const Position &position);

    Dimensions dimensions;
    Position position;
    std::string caption;
    int buttonId;
    TTF_Font* font;
    SDL_Color textColor;
    GLuint lineTexture;
    bool isUnderMouseMotion;
    bool isUnderClick;
    bool needsDrawing;
};

#endif // BUTTON_HPP
