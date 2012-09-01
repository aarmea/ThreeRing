#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    openAction = new QAction(tr("&Open"), this);
    saveAction = new QAction(tr("&Save"), this);
    exitAction = new QAction(tr("E&xit"), this);

    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    noteEdit = new NoteEditor;
    setCentralWidget(noteEdit);

    setWindowTitle(tr("ThreeRing"));
    resize(800, 600);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(quit()) event->accept();
    else event->ignore();
}

void MainWindow::open()
{
    //TODO: implement
    QMessageBox::critical(this, tr("Open"),
                          tr("Opening and saving files is not yet implemented."));
    return;
}

void MainWindow::save(){
    //TODO: implement
    QMessageBox::critical(this, tr("Save"),
                          tr("Opening and saving files is not yet implemented."));
    return;
}

bool MainWindow::quit()
{
    //TODO: check for document changes and ask to save
    return (QMessageBox::question(
                this,
                tr("Quit?"),
                tr("Do you really want to quit ThreeRing?"),
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No
                ) == QMessageBox::Yes
            );
}
