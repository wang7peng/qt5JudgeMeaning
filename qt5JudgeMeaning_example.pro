# Project created by QtCreator 2023-06-02
#
#-------------------------------------------------

QT       += core gui widgets xml

TARGET = qt5JudgeMeaning
TEMPLATE = app

# linx 6.0 default GCC only v4.9，MUST append support "-std=c++11"

system(cmake --system-information | grep "CXX_COMPILER_VERSION"  | sed -n '/[4-9]/p')

# TODO  判断当前版本号
message($(QMAKE_GCC_MAJOR_VERSION))

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        home.cpp exam.cpp

HEADERS  += home.h \
    exam.h \
    dataglobal.h

FORMS    += home.ui exam.ui

RESOURCES += \
    res.qrc
