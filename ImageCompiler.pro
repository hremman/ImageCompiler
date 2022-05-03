QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 \
    object_parallel_to_source

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/Algo/ColorFunctions.cpp \
    src/Algo/Compiler.cpp \
    src/Algo/CompilerContext.cpp \
    src/Algo/CompilerCursor.cpp \
    src/Algo/CompilerEvent.cpp \
    src/Algo/CompilerTask.cpp \
    src/Algo/ImageProcessing.cpp \
    src/Algo/ImageStorage.cpp \
    src/Algo/PreviewBuilder.cpp \
    src/Data/ColorSettings.cpp \
    src/Data/Layer.cpp \
    src/Data/Project.cpp \
    src/Data/ProjectStory.cpp \
    src/View/ColorItem.cpp \
    src/View/ColorSettings.cpp \
    src/View/FileList.cpp \
    src/View/Layer.cpp \
    src/View/MainWin.cpp \
    src/View/ProjTab.cpp \
    src/View/dialogprogress.cpp \
    src/main.cpp
    #src/View/colordialog.cpp \


HEADERS += \
    src/Algo/ColorFunctions.h \
    src/Algo/Compiler.hpp \
    src/Algo/CompilerContext.hpp \
    src/Algo/CompilerCursor.hpp \
    src/Algo/CompilerEvent.hpp \
    src/Algo/CompilerTask.hpp \
    src/Algo/ImageProcessing.hpp \
    src/Algo/ImageStorage.hpp \
    src/Algo/PreviewBuilder.hpp \
    src/Algo/ProcessingTypes.h \
    src/Algo/SpinLock.hpp \
    src/Algo/json.hpp \
    src/Data/Color.hpp \
    src/Data/ColorSettings.h \
    src/Data/Layer.hpp \
    src/Data/Project.hpp \
    src/Data/ProjectStory.hpp \
    src/Exceptions.hpp \
    src/View/ColorItem.h \
    src/View/ColorSettings.h \
    src/View/FileList.hpp \
    src/View/Layer.h \
    src/View/MainWin.h \
    src/View/ProjTab.h \
    src/View/dialogprogress.h
    #src/View/colordialog.h \

FORMS += \
    Forms/ColorItem.ui \
    Forms/ColorSettings.ui \
    Forms/FileList.ui \
    Forms/Layer.ui \
    Forms/MainWin.ui \
    Forms/ProjTab.ui \
 \    #Forms/colordialog.ui
    Forms/dialogprogress.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

INCLUDEPATH += \
    src
