#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <GL/glew.h>
#include <QMainWindow>

// add to namespace Ui
namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class
 *
 * This class handles the QMainWindow that is defined
 * in the mainwindow.ui
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow constructor
     * @param parent the parent widget
     */
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private:
    Ui::MainWindow *ui; /**< references all ui elements */

private slots:

    /*
     * Define slots to connect the ui elements with
     */

    void setAI(bool value);
    void setLocalResolution(int value);
    void setLocalResolution2(int value);

    void startDemo();
    void resetGame();
    void undoTurn();

protected:
    virtual void keyPressEvent(QKeyEvent* event);
};

#endif // MAINWINDOW_H
