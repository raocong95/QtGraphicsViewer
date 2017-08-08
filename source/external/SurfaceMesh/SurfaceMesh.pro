#---Not header-only, so have to be precompiled into a library
TEMPLATE = lib
CONFIG += staticlib
QT += opengl

# Warnings
win32{QMAKE_CXXFLAGS *= /wd4800 /wd4244 /wd4267 /wd4477}

# Config and build
include($$PWD/../../CFG_BUILD.prf)

# Eigen library
include($$PWD/../Eigen/Eigen.prf)

# Library name and destination
TARGET = SurfaceMesh
DESTDIR = $$PWD/$$BUILD/$$CFG/lib
INCLUDEPATH += surface_mesh

SOURCES += \
    SurfaceMeshModel.cpp
HEADERS += \
    SurfaceMeshModel.h
