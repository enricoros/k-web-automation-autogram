# -------------------------------------------------
# Project created by QtCreator 2009-02-20T21:23:37
# -------------------------------------------------
TARGET = WordChallengeCheat
TEMPLATE = app
SOURCES += main.cpp \
    AppWidget.cpp \
    Capture.cpp \
    InputUtils.cpp \
    Scrambler.cpp \
    GameState.cpp \
    ocr/Ocr.cpp \
    ocr/OcrCompare.cpp
HEADERS += AppWidget.h \
    Capture.h \
    InputUtils.h \
    Scrambler.h \
    GameState.h \
    ocr/Ocr.h \
    ocr/OcrCompare.h
FORMS += AppWidget.ui
unix:LIBS += -lXtst
OBJECTS_DIR = .obj
MOC_DIR = .obj
RCC_DIR = .obj
UI_DIR = .obj