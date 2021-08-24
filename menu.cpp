#include "menu.hpp"
#include "sdl_gl_wrapper.hpp"
#include "constants.hpp"
#include "text.hpp"

MainMenu::MainMenu()
{
    ownId = PollingPlaceId_MainMenu;
    Dimensions defaultButtonDimensions = {0.7, 0.125};
    buttons.push_back(Button(defaultButtonDimensions, {-0.35, 0.425},
                             "New local game", ButtonId_NewLocalGame));
    buttons.push_back(Button(defaultButtonDimensions, {-0.35, 0.225},
                             "New network game", ButtonId_NewNetworkGame));
    buttons.push_back(Button(defaultButtonDimensions, {-0.35, 0.025},
                             "Join network game", ButtonId_JoinNetworkGame));
    buttons.push_back(Button(defaultButtonDimensions, {-0.35, -0.175},
                             "Exit game", ButtonId_ExitGame));
}

PollingPlaceId MainMenu::startEventPoll()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) return PollingPlaceId_Exit;
        else if (event.type == SDL_WINDOWEVENT_ENTER or
                 event.type == SDL_WINDOWEVENT_SHOWN or
                 event.type == SDL_WINDOWEVENT_EXPOSED or
                 event.type == SDL_WINDOW_INPUT_FOCUS)
        {
            updateScreen();
        }
        else if (event.type == SDL_MOUSEMOTION)
        {
            updateButtonsOnMotion(event.motion.x, event.motion.y);
        }
        else if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            updateButtonsClickStatus();
        }
        else if(event.type == SDL_MOUSEBUTTONUP)
        {
            for (auto& button : buttons)
            {
                if (button.isClicked())
                {
                    switch (button.getButtonId())
                    {
                        case ButtonId_NewLocalGame: return PollingPlaceId_Settings;
                        case ButtonId_NewNetworkGame: return PollingPlaceId_NetworkGameCreating;
                        case ButtonId_JoinNetworkGame: return PollingPlaceId_NetworkGameJoining;
                        case ButtonId_ExitGame: return PollingPlaceId_Exit;
                    }
                }
            }
        }
    }
    return PollingPlaceId_MainMenu;
}

void MainMenu::updateScreen()
{
    if(SDL_GetTicks() - lastTicks > 20)
    {
        if (backgroundNeedsDrawing)
        {
            drawBackground();
            drawTitle();
        }
        for (auto& button : buttons)
        {
            button.draw();
        }
        SDL_GL_SwapWindow(getScreen());
        lastTicks = SDL_GetTicks();
        backgroundNeedsDrawing = false;
    }
}

void MainMenu::drawBackground()
{
    Dimensions fullScreen{2.0, 2.0};
    Position rightLeftCorner{-1.0, 1.0};

    glColor3f(0.1f, 0.1f, 0.5);
    drawRectangle(fullScreen,
                  rightLeftCorner);
}

void MainMenu::drawTitle()
{
    TTF_Font* font(TTF_OpenFont("Fonts//font.ttf", 100));
    SDL_Color textColor({255, 255, 255, 0});

    SDL_GL_RenderText("PEASANTS",
                      font,
                      textColor,
                      0.0,
                      0.9,
                      0.2);
}
