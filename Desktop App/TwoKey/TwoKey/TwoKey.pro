QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

linux {
    LIBS += -ludev
}

windows {
    LIBS += -lws2_32
    LIBS += "C:\Program Files\OpenSSL-Win64\lib\libssl.lib" "C:\Program Files\OpenSSL-Win64\lib\libcrypto.lib"
    LIBS += -L"C:\Program Files\OpenSSL-Win64\lib"
    INCLUDEPATH += "C:\Program Files\OpenSSL-Win64\include"
}
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    backend_client.cpp \
    browser_extension_communicator.cpp \
    main.cpp \
    passworddialog.cpp \
    passwordgenerator.cpp \
    sandbird.c \
    twokey.cpp \
    usb_communicator.cpp \
    usb_event_handler.cpp

HEADERS += \
    backend_client.h \
    browser_extension_communicator.h \
    passworddialog.h \
    passwordgenerator.h \
    sandbird.h \
    twokey.h \
    usb_communicator.h \
    usb_event_handler.h

FORMS += \
    passworddialog.ui \
    twokey.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = Icons/Branding/icon.ico

RESOURCES += \
    icons.qrc
