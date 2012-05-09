#-------------------------------------------------
#
# Project created by QtCreator 2012-05-09T20:36:16
#
#-------------------------------------------------

QT       += core gui

TARGET = IntervalCalculator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    scanner.cpp \
    object_storage.cpp \
    matrix.cpp \
    interval_ext.cpp \
    function_objects.cpp \
    expression_interpreter.cpp

INCLUDEPATH = C:\Programming\boost_1_49_0

HEADERS  += mainwindow.h \
    scanner.h \
    object_storage.h \
    matrix.h \
    interval_ext.h \
    function_objects.h \
    expression_interpreter.h \
    expr_except.h

FORMS    += mainwindow.ui
