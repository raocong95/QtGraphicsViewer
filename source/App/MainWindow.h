#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

#include <QString>
#include <QOpenGLWidget>

#include "GraphicsScene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
	GraphicsScene* scene;
	QOpenGLWidget* glwidget;
};
