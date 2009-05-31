# -------------------------------------------------
# Project created by QtCreator 2009-02-20T21:23:37
# -------------------------------------------------
TARGET = autogram
TEMPLATE = app
SOURCES += main.cpp \
    AppWidget.cpp \
    ScreenCapture.cpp \
    InputUtils.cpp \
    Scrambler.cpp \
    AbstractGame.cpp \
    ocr/Ocr.cpp \
    ocr/OcrCompare.cpp \
    WCGame.cpp
HEADERS += AppWidget.h \
    ScreenCapture.h \
    InputUtils.h \
    Scrambler.h \
    AbstractGame.h \
    ocr/Ocr.h \
    ocr/OcrCompare.h \
    WCGame.h
FORMS += AppWidget.ui
unix:LIBS += -lXtst
OBJECTS_DIR = .obj
MOC_DIR = .obj
RCC_DIR = .obj
UI_DIR = .obj
