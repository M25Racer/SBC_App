QT += widgets serialport
requires(qtConfig(combobox))

TARGET = SBC_App_v2
TEMPLATE = app

SOURCES += \
    crc16.cpp \
    main.cpp \
    mainwindow.cpp \
    message_box.cpp \
    settingsdialog.cpp \
    console.cpp \
    statistics.cpp \
    usb_workthread.cpp

CONFIG += c++11

#-------------------------------------------------
# Add LibUSB library
#-------------------------------------------------
win32 {
    LIBS += -L./ -llibusb-1.0
}
unix {
    LIBS += -L./ -lusb-1.0
}

#-------------------------------------------------
# Make sure to add the required libraries or
# frameoworks for the demo work depending on
# what OS is being used
#-------------------------------------------------
macx: LIBS += -framework CoreFoundation
win32: LIBS += -lSetupAPI

#-------------------------------------------------
# Make sure output directory for object file and
# executable is in the correct subdirectory
#-------------------------------------------------
macx {
    DESTDIR = mac
    OBJECTS_DIR = mac
    MOC_DIR = mac
    UI_DIR = mac
    RCC_DIR = mac
}
unix: !macx {
    DESTDIR = linux
    OBJECTS_DIR = linux
    MOC_DIR = linux
    UI_DIR = linux
    RCC_DIR = linux
}
win32 {
    DESTDIR = windows
    OBJECTS_DIR = windows
    MOC_DIR = windows
    UI_DIR = windows
    RCC_DIR = windows
}

OTHER_FILES +=

win32: LIBS += -L$$PWD/ -llibusb-1.0

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

HEADERS += \
    ../SRP_HS_USB_PROTOCOL/SRP_HS_USB_Protocol.h \
    crc16.h \
    mainwindow.h \
    message_box.h \
    settingsdialog.h \
    console.h \
    statistics.h \
    usb_workthread.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    SBC_App_v2.qrc

unix {
    INCLUDEPATH += /usr/lib/gcc/x86_64-linux-gnu/9/include
}
