#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "dimensions.hpp"
#include "position.hpp"
#include <string>
#include "sdl_gl_wrapper.hpp"

enum MenuButtonId
{
    ButtonId_NewLocalGame,
    ButtonId_NewNetworkGame,
    ButtonId_JoinNetworkGame,
    ButtonId_StartGame,
    ButtonId_MainMenu,
    ButtonId_ExitGame,
    ButtonId_PassTurn,
    ButtonId_ThrowCards,
    ButtonId_PopupOk,
    ButtonId_PlayerType1,
    ButtonId_PlayerType2,
    ButtonId_PlayerType3,
    ButtonId_PlayerType4,
    ButtonId_PlayerType5,
    ButtonId_PlayerType6,
    ButtonId_PlayerName1,
    ButtonId_PlayerName2,
    ButtonId_PlayerName3,
    ButtonId_PlayerName4,
    ButtonId_PlayerName5,
    ButtonId_PlayerName6,
    ButtonId_GiveAway,
    ButtonId_Rounds
};

class Button
{
public:
    Button(Dimensions dimensions,
           Position positon,
           std::string caption,
           int buttonId = 0,
           bool isVisible = true);
    void draw();
    void updateMotion(const Position &position);
    void updateUnderClick();
    bool isClicked();
    int getButtonId();
    void forceDraw();
    void setCaption(std::string caption);
    std::string getCaption();
    void immediatelyDraw();

    bool isVisible;
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
