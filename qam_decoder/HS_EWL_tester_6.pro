QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        FIRDecimator.cpp \
        HS_EWL_FREQ_ACQ.cpp \
        HS_EWL_FREQ_ACQ_data.cpp \
        HS_EWL_FREQ_ACQ_emxAPI.cpp \
        HS_EWL_FREQ_ACQ_emxutil.cpp \
        HS_EWL_FREQ_ACQ_initialize.cpp \
        HS_EWL_FREQ_ACQ_rtwutil.cpp \
        HS_EWL_FREQ_ACQ_terminate.cpp \
        RaisedCosineReceiveFilter.cpp \
        SystemCore.cpp \
        getSquareConstellation.cpp \
        log2.cpp \
        main.cpp \
        minOrMax.cpp \
        qammod.cpp \
        rtGetInf.cpp \
        rtGetNaN.cpp \
        rt_nonfinite.cpp \
        sum.cpp \
        unsafeSxfun.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    HS_EWL_tester_6.pro.user \
    rtw_proj.tmw

HEADERS += \
    FIRDecimator.h \
    HS_EWL_FREQ_ACQ.h \
    HS_EWL_FREQ_ACQ_data.h \
    HS_EWL_FREQ_ACQ_emxAPI.h \
    HS_EWL_FREQ_ACQ_emxutil.h \
    HS_EWL_FREQ_ACQ_initialize.h \
    HS_EWL_FREQ_ACQ_rtwutil.h \
    HS_EWL_FREQ_ACQ_terminate.h \
    HS_EWL_FREQ_ACQ_types.h \
    RaisedCosineReceiveFilter.h \
    SystemCore.h \
    getSquareConstellation.h \
    log2.h \
    main.h \
    minOrMax.h \
    qammod.h \
    rtGetInf.h \
    rtGetNaN.h \
    rt_defines.h \
    rt_nonfinite.h \
    rtwtypes.h \
    signal.h \
    sum.h \
    tmwtypes.h \
    unsafeSxfun.h

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3

QMAKE_LFLAGS_RELEASE -= -O1

unix {
    INCLUDEPATH += /usr/lib/gcc/x86_64-linux-gnu/9/include
}
