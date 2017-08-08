TEMPLATE = subdirs

SUBDIRS += \
    external\SurfaceMesh \
    CoreLib \
    App \

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered
