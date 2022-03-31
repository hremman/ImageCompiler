QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 \
    object_parallel_to_source

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/Algo/ColorFunctions.cpp \
    src/Algo/ImageTransformation.cpp \
    src/Algo/Layer.cpp \
    src/Data/ColorSettings.cpp \
    src/Data/Layer.cpp \
    src/Data/Project.cpp \
    src/File/Formats.cpp \
    src/View/ColorItem.cpp \
    src/View/ColorSettings.cpp \
    src/View/FileList.cpp \
    src/View/Layer.cpp \
    src/View/MainWin.cpp \
    src/View/ProjTab.cpp \
    src/main.cpp

HEADERS += \
    src/Algo/ColorFunctions.hpp \
    src/Algo/ImageTransformation.hpp \
    src/Algo/Layer.hpp \
    src/Data/Color.hpp \
    src/Data/ColorSettings.h \
    src/Data/Layer.hpp \
    src/Data/Project.hpp \
    src/File/Formats.hpp \
    src/View/ColorItem.h \
    src/View/ColorSettings.h \
    src/Algo/ImageTransformationInterface.hpp \
    src/View/FileList.hpp \
    src/View/Layer.h \
    src/View/MainWin.h \
    src/View/ProjTab.h

FORMS += \
    Forms/ColorItem.ui \
    Forms/ColorSettings.ui \
    Forms/FileList.ui \
    Forms/Layer.ui \
    Forms/MainWin.ui \
    Forms/ProjTab.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

INCLUDEPATH += \
    src
