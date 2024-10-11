QT       += core gui multimedia multimediawidgets
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camra.cpp \
    main.cpp \
    mainwindow.cpp \

HEADERS += \
    camra.h \
    mainwindow.h

FORMS += \
    camra.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



INCLUDEPATH +=$$PWD/../include
LIBS += -L$$PWD/../lib/opencvLib/ -lopencv_core455  -lopencv_imgcodecs455 -lopencv_objdetect455 \
-lopencv_imgproc455 -lopencv_videoio455 -lopencv_highgui455

