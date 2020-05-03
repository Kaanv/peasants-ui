#include "wait_menu.hpp"
#include "sdl_gl_wrapper.hpp"
#include "constants.hpp"
#include "text.hpp"

WaitMenu::WaitMenu(NetworkClient& netClient_) : netClient(netClient_)
{
    ownId = PollingPlaceId_WaitMenu;
    Dimensions defaultButtonDimensions = {0.7, 0.125};
    buttons.push_back(Button(defaultButtonDimensions, {-0.35, -0.6},
                             "Cancel game", ButtonId_CancelGame));
    buttons.push_back(Button(defaultButtonDimensions, {-0.35, -0.75},
                             "Main menu", ButtonId_MainMenu));
}

PollingPlaceId WaitMenu::startEventPoll()
{
    std::string lastMessage = netClient.getLastMessageFromServer();
    if (lastMessage == "GAME_STARTED")
    {
        return PollingPlaceId_ClientGame;
    }

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) return PollingPlaceId_Exit;
        else if (event.type == SDL_ACTIVEEVENT &&
                 event.active.state & SDL_APPACTIVE &&
                 event.active.gain != 0) updateScreen();
        else if (event.type == SDL_VIDEOEXPOSE)
        {
            for (auto& button : buttons)
            {
                button.forceDraw();
            }
            backgroundNeedsDrawing = true;
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
                        case ButtonId_CancelGame: return PollingPlaceId_NetworkGameJoining;
                        case ButtonId_MainMenu: return PollingPlaceId_MainMenu;
                    }
                }
            }
        }
    }
    return PollingPlaceId_WaitMenu;
}

void WaitMenu::updateScreen()
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
        SDL_GL_SwapBuffers();
        lastTicks = SDL_GetTicks();
        backgroundNeedsDrawing = false;
    }
}

void WaitMenu::drawBackground()
{
    Dimensions fullScreen{2.0, 2.0};
    Position rightLeftCorner{-1.0, 1.0};

    glColor3d(0.1, 0.1, 0.5);
    drawRectangle(fullScreen,
                  rightLeftCorner);
}

void WaitMenu::drawTitle()
{
    TTF_Font* font(TTF_OpenFont("Fonts//font.ttf", 100));
    SDL_Color textColor({255, 255, 255, 0});

    SDL_GL_RenderText("Waiting for the host to start the game",
                      font,
                      textColor,
                      0.0,
                      0.9,
                      0.13);
}

