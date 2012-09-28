#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QMessageBox>
#include "ribbon.h"
#include "noteeditor.h"
#include "svgfileio.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    void closeEvent(QCloseEvent *event);
    
signals:
    
public slots:
    void open();
    void save();
    
private:
    QWidget *mainWidget;
    QVBoxLayout *mainLayout;
    Ribbon *ribbon;
    NoteEditor *noteEdit;

    /*
    QMenu *fileMenu;
    QAction *openAction;
    QAction *saveAction;
    QAction *exitAction;
    */

    bool quitConfirm();

private slots:
    void notImplemented();
};

#endif // MAINWINDOW_H
