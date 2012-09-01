#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include "noteeditor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    void closeEvent(QCloseEvent *event);
    
signals:
    
public slots:
    
private:
    NoteEditor *noteEdit;

    QMenu *fileMenu;
    QAction *openAction;
    QAction *saveAction;
    QAction *exitAction;

private slots:
    void open();
    void save();
    bool quit();
};

#endif // MAINWINDOW_H
