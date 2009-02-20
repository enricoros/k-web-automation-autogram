# -------------------------------------------------
# Project created by QtCreator 2009-02-20T21:23:37
# -------------------------------------------------
TARGET = WordChallengeCheat
TEMPLATE = app
SOURCES += main.cpp \
    AppWidget.cpp \
    Capture.cpp \
    InputUtils.cpp
HEADERS += AppWidget.h \
    Capture.h \
    InputUtils.h
FORMS += AppWidget.ui
unix: LIBS += -lXtst
