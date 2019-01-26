TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lSDL
LIBS += -lSDL_ttf
LIBS += -lSDL_net
LIBS += -lSDL_image
LIBS += -lGLEW
LIBS += -lGLU
LIBS += -lGL
LIBS += -llua

INCLUDEPATH += LuaBridge/Source/

SOURCES += main.cpp \
    textures.cpp \
    text.cpp \
    button.cpp \
    menu.cpp \
    sdl_gl_wrapper.cpp \
    polling_place.cpp \
    game_ui.cpp \
    Card.cpp \
    CardsValidator.cpp \
    Deck.cpp \
    Game.cpp \
    Player.cpp \
    Table.cpp \
    settings.cpp \
    settings_menu.cpp

HEADERS += \
    textures.hpp \
    text.hpp \
    dimensions.hpp \
    position.hpp \
    sdl_gl_wrapper.hpp \
    resolution.hpp \
    constants.hpp \
    button.hpp \
    menu.hpp \
    polling_place_id.hpp \
    polling_place.hpp \
    game_ui.hpp \
    Card.hpp \
    CardsValidator.hpp \
    Deck.hpp \
    Game.hpp \
    Player.hpp \
    Table.hpp \
    settings.hpp \
    settings_menu.hpp
