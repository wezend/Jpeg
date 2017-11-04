TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    jpg.cpp \
    jpgread.cpp \
    jpgwrite.cpp \
    tree.cpp \
    main.cpp \
    ycbcr_rgb.cpp

HEADERS += \
    jpg.h \
    jpgread.h \
    jpgwrite.h \
    tree.h \
    ycbcr_rgb.h
