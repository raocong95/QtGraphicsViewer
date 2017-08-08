#include "ParSingleton.h"

#include <QDebug>

ParSingleton::Ptr ParSingleton::instance()
{
    static Ptr _onlyInstance = Ptr(nullptr);

    if (!_onlyInstance)
    {
        _onlyInstance.reset(new ParSingleton());
    }

    return _onlyInstance;
}


ParSingleton::ParSingleton()
{
    reset2Default();
}

void ParSingleton::reset2Default()
{
	// path
	dataPath = "../../";
	meshFilename = "";
	meshBaseName = "";

	// ui
	leftPanelWidth = 320;
	bkgBrightness = 0;
	colorMesh = QColor(103, 157, 232);
}
