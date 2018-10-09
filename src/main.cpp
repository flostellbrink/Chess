#include <QApplication>

#include "gui/glwidget.hpp"
#include "gui/mainwindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // set gl format
    GLWidget::setGLFormat();

    // load the main window
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
