#-------------------------------------------------
#
# Project created by QtCreator 2012-05-09T20:36:16
#
#-------------------------------------------------

QT       += core gui

TARGET = IntervalCalculator
TEMPLATE = app


SOURCES += main.cpp \
    signalingstorage.cpp \
    signalizer.cpp \
    matrixeditingdialog.cpp \
    matrixsavingdialog.cpp \
    numeric_methods.cpp
SOURCES +=         mainwindow.cpp
SOURCES +=     scanner.cpp
SOURCES +=     object_storage.cpp
SOURCES +=     matrix.cpp
SOURCES +=     interval_ext.cpp
SOURCES +=    function_objects.cpp
SOURCES +=    expression_interpreter.cpp

INCLUDEPATH = C:\Programming\boost_1_49_0

HEADERS  += mainwindow.h \
    signalingstorage.h \
    signalizer.h \
    matrixeditingdialog.h \
    matrixsavingdialog.h \
    numeric_methods.h
HEADERS  +=     scanner.h
HEADERS  +=     object_storage.h
HEADERS  +=     matrix.h
HEADERS  +=     interval_ext.h
HEADERS  +=     function_objects.h
HEADERS  +=     expression_interpreter.h
HEADERS  +=     expr_except.h

FORMS    += mainwindow.ui \
    matrixeditingdialog.ui \
    matrixsavingdialog.ui
