QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    distrito.cpp \
    main.cpp \
    mainwindow.cpp \
    maputils.cpp \
    popupdialog.cpp \
    pso.cpp \
    warningdialog.cpp

HEADERS += \
    distrito.h \
    mainwindow.h \
    maputils.h \
    popupdialog.h \
    pso.h \
    warningdialog.h

FORMS += \
    mainwindow.ui \
    popup.ui \
    warning.ui

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
