TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lws2_32

SOURCES += \
        main.c \
        sandbird.c

HEADERS += \
    sandbird.h
