#include "net_join_menu.hpp"
#include "sdl_gl_wrapper.hpp"
#include "constants.hpp"
#include "text.hpp"

NetJoinMenu::NetJoinMenu(NetworkClient& netClient_) : netClient(netClient_)
{
    ownId = PollingPlaceId_NetworkGameJoining;
    Dimensions defaultButtonDimensions = {0.7, 0.125};
    buttons.push_back(Button(defaultButtonDimensions, {-0.35, -0.6},
                             "Join game", ButtonId_JoinNetworkGame));
    buttons.push_back(Button(defaultButtonDimensions, {-0.35, -0.75},
                             "Main menu", ButtonId_MainMenu));

    buttons.push_back(Button(defaultButtonDimensions, {-0.75, 0.4},
                             "Name", ButtonId_PlayerType2));
    buttons.push_back(Button(defaultButtonDimensions, {-0.75, 0.25},
                             "IP address", ButtonId_PlayerType1));
    buttons.push_back(Button(defaultButtonDimensions, {-0.75, 0.1},
                             "Port", ButtonId_PlayerType2));

    buttons.push_back(Button(defaultButtonDimensions, {0.1, 0.4},
                             "Player", ButtonId_PlayerType1));
    buttons.push_back(Button(defaultButtonDimensions, {0.1, 0.25},
                             "127.0.0.1", ButtonId_PlayerType2));
    buttons.push_back(Button(defaultButtonDimensions, {0.1, 0.1},
                             "22222", ButtonId_PlayerType2));
}

PollingPlaceId NetJoinMenu::startEventPoll()
{
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
                        case ButtonId_JoinNetworkGame:
                            try
                            {
                                netClient.connectToHost("127.0.0.1", "22222");
                                netClient.sendMessage("I am player one!");
                                return PollingPlaceId_WaitMenu;
                            }
                            catch(const NetworkException& e)
                            {
                                std::cout << "Network error: " << e.what() << std::endl;
                                return PollingPlaceId_NetworkGameJoining;
                            }
                        case ButtonId_MainMenu: return PollingPlaceId_MainMenu;
                    }
                }
            }
        }
    }
    return PollingPlaceId_NetworkGameJoining;
}

void NetJoinMenu::updateScreen()
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

void NetJoinMenu::drawBackground()
{
    Dimensions fullScreen{2.0, 2.0};
    Position rightLeftCorner{-1.0, 1.0};

    glColor3d(0.1, 0.1, 0.5);
    drawRectangle(fullScreen,
                  rightLeftCorner);
}

void NetJoinMenu::drawTitle()
{
    TTF_Font* font(TTF_OpenFont("Fonts//font.ttf", 100));
    SDL_Color textColor({255, 255, 255, 0});

    SDL_GL_RenderText("Join network game",
                      font,
                      textColor,
                      0.0,
                      static_cast<float>(0.9),
                      static_cast<float>(0.13));
}

