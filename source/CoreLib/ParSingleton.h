#pragma once

#include <QSharedPointer>
#include <QColor>


class ParSingleton
{
public:
	typedef QSharedPointer<ParSingleton> Ptr;

    static Ptr instance();
    void reset2Default();

private:
    // stop the compiler generating methods of copy the object
    ParSingleton(const ParSingleton&) = delete;
    ParSingleton& operator=(const ParSingleton&) = delete;

    // private constructor
    ParSingleton();

public:
	// ui
	int leftPanelWidth;
	double bkgBrightness;
    QColor colorMesh;

	// path
	QString dataPath;
	QString meshFilename;
	QString meshBaseName;
};
