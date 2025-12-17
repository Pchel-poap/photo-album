QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = photo-album
TEMPLATE = app

CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    models/user.cpp \
    models/photo.cpp \
    models/album.cpp \
    models/tag.cpp \
    managers/photomanager.cpp \
    managers/usermanager.cpp \
    managers/datamanager.cpp \
    strategies/tagsearchstrategy.cpp \
    strategies/datesearchstrategy.cpp \
    strategies/descriptionsearchstrategy.cpp \
    ui/mainwindow.cpp \
    ui/logindialog.cpp \
    ui/photodialog.cpp \
    ui/albumdialog.cpp \
    ui/timelinewidget.cpp \
    ui/photoviewer.cpp \
    ui/tagsdialog.cpp

HEADERS += \
    models/user.h \
    models/photo.h \
    models/album.h \
    models/tag.h \
    managers/photomanager.h \
    managers/usermanager.h \
    managers/datamanager.h \
    strategies/searchstrategy.h \
    strategies/tagsearchstrategy.h \
    strategies/datesearchstrategy.h \
    strategies/descriptionsearchstrategy.h \
    ui/mainwindow.h \
    ui/logindialog.h \
    ui/photodialog.h \
    ui/albumdialog.h \
    ui/timelinewidget.h \
    ui/photoviewer.h \
    ui/tagsdialog.h

FORMS += \
    ui/mainwindow.ui \
    ui/logindialog.ui \
    ui/photodialog.ui \
    ui/albumdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
