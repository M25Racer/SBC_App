QT += widgets serialport
requires(qtConfig(combobox))

TARGET = SBC_App_v2
TEMPLATE = app

SOURCES += \
    crc16.cpp \
    main.cpp \
    mainwindow.cpp \
    message_box.cpp \
    qam_decoder/FFTImplementationCallback.cpp \
    qam_decoder/FIRDecimator.cpp \
    qam_decoder/HS_EWL_DEMOD_QAM.cpp \
    qam_decoder/HS_EWL_FREQ_ACQ.cpp \
    qam_decoder/HS_EWL_RECEIVE.cpp \
    qam_decoder/HS_EWL_RECEIVE_data.cpp \
    qam_decoder/HS_EWL_RECEIVE_emxAPI.cpp \
    qam_decoder/HS_EWL_RECEIVE_emxutil.cpp \
    qam_decoder/HS_EWL_RECEIVE_initialize.cpp \
    qam_decoder/HS_EWL_RECEIVE_rtwutil.cpp \
    qam_decoder/HS_EWL_RECEIVE_terminate.cpp \
    qam_decoder/RaisedCosineReceiveFilter.cpp \
    qam_decoder/abs.cpp \
    qam_decoder/exp.cpp \
    qam_decoder/fft.cpp \
    qam_decoder/find.cpp \
    qam_decoder/getSquareConstellation.cpp \
    qam_decoder/ifft.cpp \
    qam_decoder/log2.cpp \
    qam_decoder/minOrMax.cpp \
    qam_decoder/qamdemod.cpp \
    qam_decoder/qammod.cpp \
    qam_decoder/rat.cpp \
    qam_decoder/rtGetInf.cpp \
    qam_decoder/rtGetNaN.cpp \
    qam_decoder/rt_nonfinite.cpp \
    qam_decoder/xcorr.cpp \
    qam_thread.cpp \
    ringbuffer.cpp \
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
    agc_algorithm.h \
    crc16.h \
    mainwindow.h \
    message_box.h \
    qam_decoder/FFTImplementationCallback.h \
    qam_decoder/FIRDecimator.h \
    qam_decoder/HS_EWL_DEMOD_QAM.h \
    qam_decoder/HS_EWL_FREQ_ACQ.h \
    qam_decoder/HS_EWL_RECEIVE.h \
    qam_decoder/HS_EWL_RECEIVE_data.h \
    qam_decoder/HS_EWL_RECEIVE_emxAPI.h \
    qam_decoder/HS_EWL_RECEIVE_emxutil.h \
    qam_decoder/HS_EWL_RECEIVE_initialize.h \
    qam_decoder/HS_EWL_RECEIVE_rtwutil.h \
    qam_decoder/HS_EWL_RECEIVE_terminate.h \
    qam_decoder/HS_EWL_RECEIVE_types.h \
    qam_decoder/RaisedCosineReceiveFilter.h \
    qam_decoder/abs.h \
    qam_decoder/exp.h \
    qam_decoder/fft.h \
    qam_decoder/find.h \
    qam_decoder/getSquareConstellation.h \
    qam_decoder/ifft.h \
    qam_decoder/log2.h \
    qam_decoder/minOrMax.h \
    qam_decoder/qam_decoder.h \
    qam_decoder/qam_decoder.h \
    qam_decoder/qamdemod.h \
    qam_decoder/qammod.h \
    qam_decoder/rat.h \
    qam_decoder/rtGetInf.h \
    qam_decoder/rtGetNaN.h \
    qam_decoder/rt_defines.h \
    qam_decoder/rt_nonfinite.h \
    qam_decoder/rtwtypes.h \
    qam_decoder/signal.h \
    qam_decoder/tmwtypes.h \
    qam_decoder/xcorr.h \
    qam_thread.h \
    ringbuffer.h \
    settingsdialog.h \
    console.h \
    srp_mod_protocol.h \
    statistics.h \
    usb_global.h \
    usb_workthread.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    SBC_App_v2.qrc

unix {
    INCLUDEPATH += /usr/lib/gcc/x86_64-linux-gnu/9/include
}

SUBDIRS += \
    qam_decoder/HS_EWL_tester_6.pro

DISTFILES += \
    qam_decoder/rtw_proj.tmw
