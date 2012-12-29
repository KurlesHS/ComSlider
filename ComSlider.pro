#-------------------------------------------------
#
# Project created by QtCreator 2012-12-29T09:30:08
#
#-------------------------------------------------
#D:\Develop\Sources\Qt\ARM-USK\3rdparty\qtserialport
QT       += core gui

################################################
CONFIG(debug, debug|release):{OBJECTS_DIR = $$PWD/../build/integra/debug/obj}
else:{OBJECTS_DIR = $$PWD/../build/integra/release/obj}
RCC_DIR = $$PWD/build/integra/rcc/
UI_DIR = $$PWD/build/integra/ui/
MOC_DIR = $$PWD/build/integra/moc/
DESTDIR = $$PWD/app/

##################################################
### QtSerialPort
greaterThan(QT_MAJOR_VERSION, 4) {QT += serialport}
else:
{
    system("echo SERIALPORT_PROJECT_ROOT = $$PWD/../ARM-USK/3rdparty/qtserialport > $$OUT_PWD/.qmake.cache")
    system("echo SERIALPORT_BUILD_ROOT = $$PWD/../ARM-USK/3rdparty/qtserialport >> $$OUT_PWD/.qmake.cache")
    include($$SERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ComSlider
TEMPLATE = app


SOURCES += main.cpp\
        comslider.cpp

HEADERS  += comslider.h

FORMS    += comslider.ui
