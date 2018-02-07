TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += console c++11
LIBS += -lSDL
LIBS += -lSDL_ttf
LIBS += -lSDL_net
LIBS += -lSDL_image
LIBS += -lGLEW
LIBS += -lGLU
LIBS += -lGL

SOURCES += main.cpp \
    textures.cpp \
    text.cpp \
    button.cpp \
    menu.cpp \
    sdl_gl_wrapper.cpp \
    game.cpp

HEADERS += \
    textures.hpp \
    text.hpp \
    dimensions.hpp \
    position.hpp \
    sdl_gl_wrapper.hpp \
    polling_place.hpp \
    resolution.hpp \
    constants.hpp \
    button.hpp \
    menu.hpp \
    game.hpp
