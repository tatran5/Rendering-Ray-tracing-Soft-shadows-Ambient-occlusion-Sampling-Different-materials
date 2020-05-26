#-------------------------------------------------
#
# Project created by QtCreator 2019-01-23T19:25:31
#
#-------------------------------------------------


QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rayTracerCIS461
TEMPLATE = app
include(materials/materials.pri)

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
    primitive.cpp \
    shape.cpp \
    scene.cpp \
    sphere.cpp \
    squareplane.cpp \
    light.cpp \
    pointlight.cpp \
    disc.cpp \
    arealight.cpp \
    warpfunctions.cpp \
    sampler.cpp \
    ambientocclusion.cpp \
    utils.cpp

HEADERS += \
        mainwindow.h \
        globalincludes.h \
    ray.h \
    camera.h \
    transform.h \
    intersection.h \
    primitive.h \
    shape.h \
    scene.h \
    sphere.h \
    squareplane.h \
    light.h \
    pointlight.h \
    disc.h \
    arealight.h \
    warpfunctions.h \
    pcg32.h \
    sampler.h \
    enums.h \
    ambientocclusion.h \
    enums.h \
    pcg32.h \
    utils.h

FORMS += \
        mainwindow.ui

CONFIG += c++17

INCLUDEPATH += include

DEPENDPATH += include

DISTFILES += \
    lights/lights.pri
