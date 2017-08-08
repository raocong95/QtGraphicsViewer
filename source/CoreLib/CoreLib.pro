#---Not header-only, so have to be precompiled into a library
TEMPLATE = lib
CONFIG += staticlib

QT += core gui widgets opengl xml

win32 {# Enable debug info in release build
    QMAKE_CXXFLAGS_RELEASE += /Zi
    QMAKE_LFLAGS_RELEASE += /DEBUG

    QMAKE_CXXFLAGS += -bigobj
}

# Libraries
include($$PWD/../external/SurfaceMesh/SurfaceMesh.prf)


# The output
TARGET = CoreLib
include($$PWD/../CFG_BUILD.prf)
DESTDIR = $$PWD/$$BUILD/$$CFG/lib
INCLUDEPATH += $$PWD

HEADERS += \
    MeshModel.h \
    ColorMap.h \
    ParSingleton.h \
    TransfModel.h \
    CoreLibUtility.h \
    ShaderRenderer.h \
    Line.h \
    Plane.h


SOURCES += \
    MeshModel.cpp \
    ColorMap.cpp \
    ParSingleton.cpp \
    TransfModel.cpp \
    ShaderRenderer.cpp \
    Line.cpp \
    Plane.cpp
