TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    test/main.cpp \
    src/INIConverter.cpp \
    src/Tokenizer.cpp

HEADERS += \
    public/Constraints.h \
    public/INI/INIConverter.h \
    public/Util/Tokenizer.h

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/public

LIBS += -pthread

