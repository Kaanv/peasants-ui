#pragma once

#include "polling_place.hpp"
#include "settings.hpp"

class SettingsMenu : public PollingPlace
{
public:
    SettingsMenu(Settings&);
protected:
    PollingPlaceId startEventPoll() override;
    virtual void addMenuSpecificCaptions(std::vector<std::string>& captions);
private:
    void updateScreen() override;
    void drawBackground() override;
    std::string nextCaption(std::string caption);
    std::string nextRoundsCaption(std::string caption);
    void setSettingsAccordingToButtons();
    PlayerType convertCaptionToPlayerType(std::string caption);
    std::vector<std::string> createCaptions();
    Settings& settings;
};
