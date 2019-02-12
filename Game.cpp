#include "Game.hpp"
#include <stdexcept>
#include <iostream>
#include <regex>
#include <LuaBridge/LuaBridge.h>

using namespace luabridge;

template <typename T>
struct EnumWrapper {
    static inline typename std::enable_if<std::is_enum<T>::value, void>::type
    push(lua_State* L, T value)
    {
        lua_pushinteger(L, static_cast<std::size_t>(value));
    }

    static inline typename std::enable_if<std::is_enum<T>::value, T>::type
    get(lua_State* L, int index)
    {
        return static_cast<T>(lua_tointeger(L, index));
    }
};

template <> struct luabridge::Stack<Value>: EnumWrapper<Value> {};
template <> struct luabridge::Stack<Color>: EnumWrapper<Color> {};

void History::saveThrowCards(Cards cards, int playerId)
{
    history.push_back(HistoryElement{playerId, "THROW", cards});
}

void History::savePassedTurn(int playerId)
{
    history.push_back(HistoryElement{playerId, "PASS TURN", Cards{}});
}

std::vector<HistoryElement> History::getHistory()
{
    return history;
}

void History::clear()
{
    history.clear();
}

void Game::registerCardClassInLua()
{
    for (int i = 0; i < numberOfPlayers; i++)
    {
        if (aiStates[i])
        getGlobalNamespace(aiStates[i]).beginNamespace("Peasants")
           .beginClass<Card>("Card")
           .addData("value", &Card::value)
           .addData("color", &Card::color)
           .addData("selected", &Card::selected)
           .endClass();
    }
}

void Game::registerCardsClassInLua()
{
    for (int i = 0; i < numberOfPlayers; i++)
    {
        if (aiStates[i])
        getGlobalNamespace(aiStates[i]).beginNamespace("Peasants")
            .beginClass<std::vector<Card>>("Cards")
            .addFunction<std::vector<Card>::const_reference(std::vector<Card>::*)(std::vector<Card >::size_type) const>("at", &std::vector< Card >::at)
            .addFunction<long unsigned int (std::vector<Card>::*)() const>("numberOfCards", &std::vector<Card>::size)
            .endClass();
    }
}

void Game::registerHistoryClassInLua()
{
    for (int i = 0; i < numberOfPlayers; i++)
    {
        if (aiStates[i])
        {
            getGlobalNamespace(aiStates[i]).beginNamespace("Peasants")
                .beginClass<HistoryElement>("HistoryElement")
                .addData("playerId", &HistoryElement::playerId)
                .addData("action", &HistoryElement::action)
                .addData("cards", &HistoryElement::cards)
                .endClass();

            getGlobalNamespace(aiStates[i]).beginNamespace("Peasants")
                .beginClass<std::vector<HistoryElement>>("History")
                .addFunction<std::vector<HistoryElement>::const_reference(std::vector<HistoryElement>::*)(std::vector<HistoryElement >::size_type) const>("at", &std::vector< HistoryElement >::at)
                .addFunction<long unsigned int (std::vector<HistoryElement>::*)() const>("lengthOfHistory", &std::vector<HistoryElement>::size)
                .endClass();
        }
    }
}

Game::Game(Settings settings) : deck(settings.numberOfPlayers),
                                cardsValidator(deck.getStartingCard())
{
    this->numberOfPlayers = settings.numberOfPlayers;
    for (int i = 0; i < numberOfPlayers; i++)
    {
        players.push_back(Player(i));
        levelsHistory.push_back(LevelsHistory{});

        if (settings.playerTypes[i] == PlayerType_AI)
        {
            aiStates.push_back(luaL_newstate());
            luaL_dofile(aiStates[i], settings.aiTypes[i].c_str());
            luaL_openlibs(aiStates[i]);
            lua_pcall(aiStates[i], 0, 0, 0);

            try
            {
                LuaRef luaAIInit = getGlobal(aiStates[i], "ai_init");
                luaAIInit(settings.numberOfPlayers, i);
            }
            catch(...)
            {
                std::cout << "ERROR IN ai_init FUNCTION" << std::endl;
            }
        }
        else
        {
            aiStates.push_back(NULL);
        }
    }

    resetRound();
    registerCardClassInLua();
    registerCardsClassInLua();
    registerHistoryClassInLua();
}

void Game::distributeCardsFromDeck()
{
    for (unsigned int i = 0; i < players.size(); i++)
    {
        players[i].resetCards();

        for (int j = 0; j < 8; j++)
        {
            players[i].insertCard(deck.takeCard());
        }
    }
}

int Game::findStartingPlayer()
{
    Card startingCard = deck.getStartingCard();

    for (unsigned int i = 0; i < players.size(); i++)
    {
        Cards playerCards = players[i].getCards();

        for (unsigned int j = 0; j < playerCards.size(); j++)
        {
            if (playerCards[j] == startingCard)
            {
                return i;
            }
        }
    }

    return 0;
}

bool Game::hasRoundEnded()
{
    return playersThatEnded.size() >= players.size() - 1;
}

Player& Game::getCurrentPlayer()
{
    return players[currentPlayerId];
}

void Game::nextPlayer()
{
    currentPlayerId = (currentPlayerId + 1) % players.size();

    if (players[currentPlayerId].hasEnded())
    {
        nextPlayer();
    }
}

void Game::throwCards(Cards cards)
{
    if (passedTurns >= players.size() - 1 - playersThatEnded.size())
    {
        cardsValidator.checkIfCardsHaveSameValue(cards);
    }
    else
    {
        cardsValidator.checkIfCardsCouldBeThrown(cards, table.getCardsFromTop());
    }

    table.throwCards(cards);
    passedTurns = 0;
    saveThrowCardsInHistory(cards);
    getCurrentPlayer().removeSelectedCards();
    checkIfPlayerHasEnded();
}

const Cards& Game::getCardsFromTableTop() const
{
    return table.getCardsFromTop();
}

void Game::passCurrentPlayerTurn()
{
    if (getCardsFromTableTop().size())
    {
        passedTurns++;
        savePassedTurnInHistory();
    }
    else
    {
        throw std::runtime_error("Cannot pass starting turn");
    }
}

void Game::nextRound()
{
    setPeasantsLevels();
    resetRound();
    numberOfEndedRounds++;
    setStartingPlayer();
    addPeasantsLevelsToLevelsHistory();
}

void Game::checkIfPlayerHasEnded()
{
    if (players[currentPlayerId].hasEnded())
    {
        playersThatEnded.push_back(currentPlayerId);
    }
}

Player &Game::getPlayer(unsigned int id)
{
    return players[id];
}

unsigned int Game::findOppositePlayerId(int peasantLevel)
{
    for (unsigned i = 0; i < players.size(); i++)
    {
        if (players[i].getPeasantLevel() == -peasantLevel)
        {
            return i;
        }
    }

    return 0;
}

void Game::setStartingPlayer()
{
    currentPlayerId = findStartingPlayer();
}

void Game::resetRound()
{
    deck.resetDeck(players.size());
    distributeCardsFromDeck();
    currentPlayerId = findStartingPlayer();
    passedTurns = 0;
    playersThatEnded.clear();
    table.clearTable();
    history.clear();
}

void Game::setPeasantsLevels()
{
    for (unsigned int i = 0; i < players.size(); i++)
    {
        if (not players[i].hasEnded())
        {
            playersThatEnded.push_back(i);
        }
    }

    int peasantLevel;

    if (players.size() == 4 or players.size() == 5)
    {
        peasantLevel = 2;
    }
    else
    {
        peasantLevel = 3;
    }

    for (unsigned i = 0; i < playersThatEnded.size(); i++)
    {
        players[playersThatEnded[i]].setPeasantLevel(peasantLevel);
        peasantLevel--;

        if (peasantLevel == 0 and players.size() != 5) peasantLevel--;
    }
}

void Game::saveThrowCardsInHistory(const Cards& cards)
{
    history.saveThrowCards(cards, currentPlayerId);
}

void Game::savePassedTurnInHistory()
{
    history.savePassedTurn(currentPlayerId);
}

std::vector<std::string> split(const std::string& input, const std::string& regex)
{
    std::regex re(regex);
    std::sregex_token_iterator
        first{input.begin(), input.end(), re, -1},
        last;
    return {first, last};
}

void Game::performAITurnLua()
{
    Cards cards= getCurrentPlayer().getCards();
    Cards tableCards = getCardsFromTableTop();

    LuaRef luaAITrun = getGlobal(aiStates[currentPlayerId], "ai_turn");
    std::string command = luaAITrun(cards, tableCards, history.getHistory());
    std::cout << "AI PLAYER " << currentPlayerId << " EXECUTE: " << command << std::endl;

    if (command == "PASS TURN")
    {
        passCurrentPlayerTurn();
    }
    else
    {
        std::vector<std::string> splittedCommand = split(command, " ");
        getCurrentPlayer().unselectAllCards();
        for (const auto& e : splittedCommand)
        {
            if (e != "THROW")
            {
                getCurrentPlayer().selectCard(std::stoi(e));
            }
        }

        throwCards(getCurrentPlayer().getSelectedCards());
    }

    nextPlayer();
}

void Game::giveCardsToPeasantAsAI(int playerId)
{
    std::vector<int> cardsToGiveAwayIndiecies;
    Cards cards= players[playerId].getCards();

    try
    {
        LuaRef luaAIGiveCardsToPeasant =
            getGlobal(aiStates[playerId], "give_cards_to_peasant");
        std::string command =luaAIGiveCardsToPeasant(cards,
                                                     getPlayer(playerId).getPeasantLevel());
        std::vector<std::string> splittedCommand = split(command, " ");
        for (int i = 0; i < getPlayer(playerId).getPeasantLevel(); i++)
        {
            cardsToGiveAwayIndiecies.push_back(std::stoi(splittedCommand[i]));
        }
    }
    catch(...)
    {
        std::cout << "ILLEGAL COMMAND IN give_cards_to_peasant FUNCTION\n"
                     "GIVING AWAY RANDOM CARDS" << std::endl;
        cardsToGiveAwayIndiecies.clear();
        for (int i = 0; i < getPlayer(playerId).getPeasantLevel(); i++)
        {
            cardsToGiveAwayIndiecies.push_back(i);
        }
    }
    if (static_cast<int>(cardsToGiveAwayIndiecies.size()) <
            getPlayer(playerId).getPeasantLevel())
    {
        std::cout << "NOT ENOUGH CARDS IN give_cards_to_peasant FUNCTION\n"
                     "GIVING AWAY ADDITIONAL RANDOM CARDS" << std::endl;
        for (int i = cardsToGiveAwayIndiecies.size(); i < getPlayer(playerId).getPeasantLevel(); i++)
        {
            cardsToGiveAwayIndiecies.push_back(i);
        }
    }

    std::sort(cardsToGiveAwayIndiecies.begin(), cardsToGiveAwayIndiecies.end());
    Cards cardsToGiveAway;
    for (int i = cardsToGiveAwayIndiecies.size() - 1; i >= 0 ; i--)
    {
        cardsToGiveAway.push_back(getPlayer(playerId).takeCard(cardsToGiveAwayIndiecies[i]));
    }
    unsigned int peasantId = findOppositePlayerId(getPlayer(playerId).getPeasantLevel());

    for (unsigned int i = 0; i < cardsToGiveAway.size(); i++)
    {
        getPlayer(peasantId).insertCard(cardsToGiveAway[i]);
    }
}

void Game::giveCardsToPeasantAsHuman(int playerId)
{
    validateNumberOfCardsToGiveAway(playerId);
    Cards cardsToGiveAway = players[playerId].getSelectedCards();
    unsigned int peasantId = findOppositePlayerId(getPlayer(playerId).getPeasantLevel());
    for (unsigned int i = 0; i < cardsToGiveAway.size(); i++)
    {
        getPlayer(peasantId).insertCard(cardsToGiveAway[i]);
    }
    players[playerId].removeSelectedCards();
}

void Game::validateNumberOfCardsToGiveAway(int playerId)
{
    if (players[playerId].getSelectedCards().size() !=
            static_cast<unsigned int>(players[playerId].getPeasantLevel()))
    {
        throw std::runtime_error("Wrong number of cards. Proper: " +
                                 std::to_string(players[playerId].getPeasantLevel()));
    }
}

unsigned int Game::getNumberOfEndedRounds()
{
    return numberOfEndedRounds;
}

void Game::addPeasantsLevelsToLevelsHistory()
{
    for (int i = 0; i < numberOfPlayers; i++)
    {
        levelsHistory[i].push_back(players[i].getPeasantLevel());
    }
}

const std::vector<LevelsHistory>& Game::getLevelsHistory()
{
    return levelsHistory;
}

History& Game::getHistory()
{
    return history;
}

void Game::indicatePeasantLevel(int playerIndex)
{
    try
    {
        LuaRef indicateLevel =
            getGlobal(aiStates[playerIndex], "indicate_peasant_level");
        indicateLevel(getPlayer(playerIndex).getPeasantLevel());
    }
    catch(...)
    {
        std::cout << "ERROR IN indicate_peasant_level FUNCTION" << std::endl;
    }
}
