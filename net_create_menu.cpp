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
    SDL_Delay(100);

    for (unsigned int clientIndex = 0; clientIndex < 5; clientIndex++)
    {
        std::string lastMessage = netServer.getLastMessageFromClient(clientIndex);
        if (lastMessage == "New Player Joining")
        {
            for (auto& button : buttons)
            {
                if (button.getButtonId() == ButtonId_PlayerType1 or
                    button.getButtonId() == ButtonId_PlayerType2 or
                    button.getButtonId() == ButtonId_PlayerType3 or
                    button.getButtonId() == ButtonId_PlayerType4 or
                    button.getButtonId() == ButtonId_PlayerType5 or
                    button.getButtonId() == ButtonId_PlayerType6)
                {
                    if (button.getCaption() == "Open")
                    {
                        button.setCaption("Network Player");
                        if (button.getButtonId() == ButtonId_PlayerType1) settings.clientId[0] = clientIndex;
                        else if (button.getButtonId() == ButtonId_PlayerType2) settings.clientId[1] = clientIndex;
                        else if (button.getButtonId() == ButtonId_PlayerType3) settings.clientId[2] = clientIndex;
                        else if (button.getButtonId() == ButtonId_PlayerType4) settings.clientId[3] = clientIndex;
                        else if (button.getButtonId() == ButtonId_PlayerType5) settings.clientId[4] = clientIndex;
                        else if (button.getButtonId() == ButtonId_PlayerType6) settings.clientId[5] = clientIndex;
                        break;
                    }
                }
            }
        }
    }

    PollingPlaceId result = SettingsMenu::startEventPoll();

    if (result != PollingPlaceId_Settings) return result;
    return PollingPlaceId_NetworkGameCreating;
}

void NetCreateMenu::addMenuSpecificCaptions(std::vector<std::string>& captions)
{
    captions.push_back("Local Human");
    captions.push_back("Open");
}
