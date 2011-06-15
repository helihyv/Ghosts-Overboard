#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QGraphicsView>
#include "orientationcontrolledgraphicspixmapobject.h"
#include "seascene.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool event(QEvent *event);

public slots:
    void initializeBoundaries();
    void restartLevel();
    void about();
    void nextLevel();


private:

SeaScene * pScene_;
QGraphicsView * pView_;
QAction* pPauseAction_;


};

#endif // MAINWINDOW_H
