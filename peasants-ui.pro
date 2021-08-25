TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lSDL2
LIBS += -lSDL2_ttf
LIBS += -lSDL2_net
LIBS += -lSDL2_image
LIBS += -lGLEW
LIBS += -lGLU
LIBS += -lGL
LIBS += -llua
LIBS += -lboost_filesystem

INCLUDEPATH += LuaBridge/Source/

SOURCES += main.cpp \
    base_ui.cpp \
    client_ui.cpp \
    network_server.cpp \
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
    settings_menu.cpp \
    results_menu.cpp \
    net_join_menu.cpp \
    wait_menu.cpp \
    network_client.cpp \
    net_create_menu.cpp \
    network_common.cpp

HEADERS += \
    LuaBridge/List.h \
    LuaBridge/LuaBridge.h \
    LuaBridge/Map.h \
    LuaBridge/RefCountedObject.h \
    LuaBridge/RefCountedPtr.h \
    LuaBridge/UnorderedMap.h \
    LuaBridge/Vector.h \
    LuaBridge/detail/CFunctions.h \
    LuaBridge/detail/ClassInfo.h \
    LuaBridge/detail/Config.h \
    LuaBridge/detail/Constructor.h \
    LuaBridge/detail/FuncTraits.h \
    LuaBridge/detail/Iterator.h \
    LuaBridge/detail/LuaException.h \
    LuaBridge/detail/LuaHelpers.h \
    LuaBridge/detail/LuaRef.h \
    LuaBridge/detail/Namespace.h \
    LuaBridge/detail/Security.h \
    LuaBridge/detail/Stack.h \
    LuaBridge/detail/TypeList.h \
    LuaBridge/detail/TypeTraits.h \
    LuaBridge/detail/Userdata.h \
    LuaBridge/detail/dump.h \
    base_ui.hpp \
    client_ui.hpp \
    network_server.hpp \
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
    settings_menu.hpp \
    results_menu.hpp \
    score.hpp \
    net_join_menu.hpp \
    network_client.hpp \
    wait_menu.hpp \
    net_create_menu.hpp \
    network_common.hpp
