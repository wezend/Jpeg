TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    jpg.cpp \
    jpgread.cpp \
    jpgwrite.cpp \
    tree.cpp

HEADERS += \
    jpg.h \
    jpgread.h \
    jpgwrite.h \
    tree.h
