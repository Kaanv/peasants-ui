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

SOURCES += main.cpp
