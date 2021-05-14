QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    code.cpp \
    expressiontree.cpp \
    main.cpp \
    mainwindow.cpp \
    simulator.cpp \
    utils.cpp

HEADERS += \
    Simulator.h \
    code.h \
    expressiontree.h \
    mainwindow.h \
    utils.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Basic Interpreter.assets/image-20210413193105381.png \
    Basic Interpreter.assets/image-20210413193105381.png \
    Basic Interpreter.assets/image-20210413193138093.png \
    Basic Interpreter.assets/image-20210413193138093.png \
    README.md \
    code examples/test01.txt \
    code examples/test02.txt \
    test log.assets/wps10.jpg \
    test log.assets/wps11.jpg \
    test log.assets/wps12.jpg \
    test log.assets/wps13.jpg \
    test log.assets/wps14.jpg \
    test log.assets/wps8.jpg \
    test log.assets/wps9.jpg \
    test/fibonacci sequence.txt \
    test/inputs test.txt \
    test/printf test.txt \
    test/simple add.txt
