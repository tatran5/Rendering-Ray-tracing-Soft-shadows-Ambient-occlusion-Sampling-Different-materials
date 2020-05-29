#-------------------------------------------------
#
# Project created by QtCreator 2019-01-23T19:25:31
#
#-------------------------------------------------

include(lights/lights.pri)
include(materials/materials.pri)
include(sampling/sampling.pri)
include(shapes/shapes.pri)
include(terrain/terrain.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rayTracerCIS461
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ray.cpp \
    camera.cpp \
    transform.cpp \
    intersection.cpp \
    scene.cpp \
    ambientocclusion.cpp \
    utils.cpp

HEADERS += \
        mainwindow.h \
        globalincludes.h \
    ray.h \
    camera.h \
    transform.h \
    intersection.h \
    scene.h \
    ambientocclusion.h \
    utils.h

FORMS += \
        mainwindow.ui

CONFIG += c++17

INCLUDEPATH += include

DEPENDPATH += include

