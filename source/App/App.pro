QT          += core gui opengl widgets xml

TARGET      = Rt3DVisionApp
TEMPLATE    = app
DESTDIR     = $$PWD/../../bin
CONFIG(debug, debug|release) {TARGET = Rt3DVisionAppD}

# Enable debuging in release mode
win32 {
    QMAKE_CXXFLAGS_RELEASE += /Zi
    QMAKE_LFLAGS_RELEASE += /DEBUG
}

# Libraries
include($$PWD/../CoreLib/CoreLib.prf)
include($$PWD/../external/SurfaceMesh/SurfaceMesh.prf)

SOURCES += main.cpp\
    MainWindow.cpp \
    GraphicsScene.cpp \
    GraphicsView.cpp \ 
    ModifiersPanel.cpp \
    Document.cpp \
    Tools/Tool.cpp \
    Tools/MainTool.cpp \
    Tools/MainToolView.cpp


HEADERS  += MainWindow.h \
    GraphicsScene.h \
    GraphicsView.h \
    ModifiersPanel.h \
    Document.h \
    Tools/Camera.h \
    Tools/Tool.h \
    Tools/MainTool.h \
    Tools/MainToolView.h

FORMS  += MainWindow.ui \
    ModifiersPanel.ui \
    Tools/MainToolPanel.ui

RESOURCES += \
    resources.qrc
