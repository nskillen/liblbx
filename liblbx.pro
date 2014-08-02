#-------------------------------------------------
#
# Project created by QtCreator 2014-08-02T13:54:01
#
#-------------------------------------------------

QT       -= core gui

QMAKE_CXXFLAGS += -std=c++11

TARGET = liblbx
TEMPLATE = lib
CONFIG += staticlib

SOURCES +=

HEADERS +=
unix {
	target.path = /usr/lib
	INSTALLS += target
}
