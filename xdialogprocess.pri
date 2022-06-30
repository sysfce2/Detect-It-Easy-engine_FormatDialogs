INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/xdialogprocess.ui

HEADERS += \
    $$PWD/xdialogprocess.h

SOURCES += \
    $$PWD/xdialogprocess.cpp

!contains(XCONFIG, xoptions) {
    XCONFIG += xoptions
    include($$PWD/../XOptions/xoptions.pri)
}
