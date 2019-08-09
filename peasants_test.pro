TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += mainGT.cpp \
    GameGT.cpp \
    Player.cpp \
    Card.cpp \
    Deck.cpp \
    Game.cpp \
    DeckGT.cpp \
    PlayerGT.cpp \
    Table.cpp \
    TableGT.cpp \
    CardsValidator.cpp \
    CardsValidatorGT.cpp

HEADERS += \
    Player.hpp \
    Card.hpp \
    Deck.hpp \
    Game.hpp \
    Table.hpp \
    CardsValidator.hpp

INCLUDEPATH += LuaBridge/Source/
LIBS += -lgtest
LIBS += -llua
