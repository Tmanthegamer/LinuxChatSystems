#-------------------------------------------------
#
# Project created by QtCreator 2016-03-23T09:53:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Chat
TEMPLATE = app

OUTPUT += Console


SOURCES += main.cpp\
        chat.cpp \
		Client.cpp

HEADERS  += chat.h \
			Client.h \
			PacketUtilities.h

FORMS    += chat.ui
