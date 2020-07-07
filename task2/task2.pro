#-------------------------------------------------
#
# Project created by QtCreator 2020-06-15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TASK2
TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
INCLUDEPATH +=  D:/openCV3/opencv/necessary_file/include/opencv2 \
                D:/openCV3/opencv/necessary_file/include

LIBS += D:/openCV3/opencv/necessary_file/lib/libopencv_*.a


