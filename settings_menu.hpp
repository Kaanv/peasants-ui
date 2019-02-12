#pragma once

#include "polling_place.hpp"
#include "settings.hpp"

class SettingsMenu : public PollingPlace
{
public:
    SettingsMenu();
    Settings getSettings();
private:
    PollingPlaceId startEventPoll() override;
    void updateScreen() override;
    void drawBackground() override;
    std::string nextCaption(std::string caption);
    std::string nextRoundsCaption(std::string caption);
    void setSettingsAccordingToButtons();
    PlayerType convertCaptionToPlayerType(std::string caption);
    Settings settings;
};
