#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <GL/glew.h>
#include <QKeyEvent>

#include "gui/config.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //this->setFixedSize(1600,900);

    // setup ui elements
    ui->setupUi(this);

	// init the value elements
	this->ui->lcdResolution->display(Config::geoResolution1);
	this->ui->lcdResolution2->display(Config::geoResolution2);
    this->ui->lcdGeoResolution->display(Config::shadowResolution);

    // buttons
    connect(this->ui->newGameButton, SIGNAL(released()), this, SLOT(resetGame()));
    connect(this->ui->undoTurnButton, SIGNAL(released()), this, SLOT(undoTurn()));
    connect(this->ui->startDemoButton, SIGNAL(released()), this, SLOT(startDemo()));
    //options
    connect(this->ui->checkBoxAI, SIGNAL(clicked(bool)), this, SLOT(setAI(bool)));

	//resolutions
	connect(this->ui->sliderLocalResolution, SIGNAL(valueChanged(int)), this, SLOT(setGeoResolution(int)));
	connect(this->ui->sliderLocalResolution2, SIGNAL(valueChanged(int)), this, SLOT(setGeoResolution2(int)));
    connect(this->ui->sliderGeoResolution, SIGNAL(valueChanged(int)), this, SLOT(setShadowResolution(int)));

    // Default to fullscreen
    showFullScreen();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::resetGame() {
    Config::newGame = true;
}

void MainWindow::undoTurn() {
    Config::undoTurn = true;
}

void MainWindow::startDemo() {
    Config::demo = true;
}

void MainWindow::setGeoResolution(int value){
    Config::changed = true;
	Config::geoResolution1 = value;
	this->ui->lcdResolution->display(value);
}

void MainWindow::setGeoResolution2(int value){
    Config::changed = true;
	Config::geoResolution2 = value;
	this->ui->lcdResolution2->display(value);
}

void MainWindow::setShadowResolution(int value) {
    Config::changed = true;
    Config::shadowResolution = value;
    this->ui->lcdGeoResolution->display(value);
}

void MainWindow::setAI(bool value){
    Config::ai = value;
}

/* Qt mouse and keyboard events */

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    // enable control via keyboard
    switch (event->key()) {
    case Qt::Key_F:
        if (isFullScreen()) {
            showNormal();
        } else {
            showFullScreen();
        }
        break;
    case Qt::Key_Escape:
    case Qt::Key_Q:
        close();
        break;
    case Qt::Key_Z:
        Config::undoTurn = true;
        break;
    case Qt::Key_T:
        Config::changeTheme = true;
        break;
    case Qt::Key_Delete:
        Config::newGame = true;
        break;
    case Qt::Key_D:
        Config::demo = true;
        break;
    }
}


