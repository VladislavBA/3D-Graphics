#-------------------------------------------------
#
# Project created by QtCreator 2016-04-16T23:10:12
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3D-Graphics
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        window.cpp \
    io.cpp \
    sparse_matrix_op.cpp \
    iter_alg.cpp \
    mesh.cpp \
    multithread.cpp \
    scene.cpp \
    thread_source.cpp \
    matrix_args.cpp

HEADERS  += window.h \
    io.h \
    iter_alg.h \
    sparse_matrix_op.h \
    mesh.h \
    multithread.h \
    matrix_args.h \
    scene.h \
    thread_source.h \
    matrix_args.h
