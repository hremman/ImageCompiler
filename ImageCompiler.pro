QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AlgColorFunctions.cpp \
    AlgImageTransformation.cpp \
    AlgLayer.cpp \
    CompilerMainWin.cpp \
    DataLayer.cpp \
    DataProject.cpp \
    FileFormat.cpp \
    LayerView.cpp \
    ProjTab.cpp \
    ViewColorItem.cpp \
    ViewColorSettings.cpp \
    main.cpp

HEADERS += \
    AlgColorFunctions.h \
    AlgImageTransformation.hpp \
    AlgLayer.hpp \
    CompilerMainWin.h \
    DataColor.hpp \
    DataLayer.hpp \
    DataProject.hpp \
    FileFormat.hpp \
    ImageTransformationInterface.hpp \
    LayerView.h \
    ProjTab.h \
    ViewColorItem.h \
    ViewColorSettings.h

FORMS += \
    CompilerMainWin.ui \
    LayerView.ui \
    ProjTab.ui \
    ViewColorItem.ui \
    ViewColorSettings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
