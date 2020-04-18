#include "net_create_menu.hpp"
#include <iostream>

NetCreateMenu::NetCreateMenu(Settings& _settings,
                             NetworkServer& _netServer) : SettingsMenu(_settings), netServer(_netServer)
{
    for (auto& button : buttons)
    {
        switch (button.getButtonId())
        {
            case ButtonId_PlayerType1:
                button.setCaption("Local Human");
                break;
            case ButtonId_PlayerType2:
            case ButtonId_PlayerType3:
            case ButtonId_PlayerType4:
                button.setCaption("Open");
                break;
            case ButtonId_PlayerType5:
            case ButtonId_PlayerType6:
                button.setCaption("None");
                break;
        }
    }
}

PollingPlaceId NetCreateMenu::startEventPoll()
{
    netServer.startServer();
    netServer.sendStringToClient("HI, IM SERVER");
    std::cout << "SENT HI MESSAGE!" << std::endl;
    SDL_Delay(100);
    PollingPlaceId result = SettingsMenu::startEventPoll();

    if (result != PollingPlaceId_Settings) return result;
    return PollingPlaceId_NetworkGameCreating;
}

void NetCreateMenu::addMenuSpecificCaptions(std::vector<std::string>& captions)
{
    captions.push_back("Local Human");
    captions.push_back("Open");
}
