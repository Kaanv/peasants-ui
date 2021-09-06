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
                             "127.0.0.1", ButtonId_IP));
    buttons.push_back(Button(defaultButtonDimensions, {0.1, 0.1},
                             "22222", ButtonId_PlayerType2));
}

std::string convertKeyToString(SDL_KeyboardEvent key)
{
    return numericOrDotToString(key.keysym.sym);
}

PollingPlaceId NetJoinMenu::startEventPoll()
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
                        case ButtonId_JoinNetworkGame:
                            try
                            {
                                netClient.connectToHost("127.0.0.1", "22222");
                                netClient.sendMessage("New Player Joining");
                                return PollingPlaceId_WaitMenu;
                            }
                            catch(const NetworkException& e)
                            {
                                std::cout << "Network error: " << e.what() << std::endl;
                                return PollingPlaceId_NetworkGameJoining;
                            }
                        case ButtonId_IP:
                            enteringIp = true;
                            break;
                        case ButtonId_MainMenu: return PollingPlaceId_MainMenu;
                    }
                }
            }
        }
        else if (event.type == SDL_KEYUP)
        {
            if (enteringIp)
            {
                for (auto& button : buttons)
                {
                    switch (button.getButtonId())
                    {
                        case ButtonId_IP:
                            if (event.key.keysym.sym == SDLK_BACKSPACE)
                            {
                                std::string captionWithoutLastElement = button.getCaption() ;
                                captionWithoutLastElement = captionWithoutLastElement.substr(0, captionWithoutLastElement.size() - 1);
                                button.setCaption(captionWithoutLastElement);
                            }
                            else
                            {
                                button.setCaption(button.getCaption() + convertKeyToString(event.key));
                            }
                            button.forceDraw();
                            updateScreen();
                            break;
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
        SDL_GL_SwapWindow(getScreen());
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

