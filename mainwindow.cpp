#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    openAction = new QAction(tr("&Open"), this);
    saveAction = new QAction(tr("&Save"), this);
    exitAction = new QAction(tr("E&xit"), this);

    noteEdit = new NoteEditor;
    setCentralWidget(noteEdit);

    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    setWindowTitle(tr("ThreeRing"));
    resize(800, 600);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(quitConfirm()) event->accept();
    else event->ignore();
}

// Open a document.
void MainWindow::open()
{
    QString path = QFileDialog::getOpenFileName(
                this, tr("Open"), QDir::homePath(), tr("SVG files (*.svg)"));
    svgOpen(path, noteEdit);
}

// Save the current document.
void MainWindow::save()
{
    // TODO: default to the file opened
    QString path = QFileDialog::getSaveFileName(
                this, tr("Save"), QDir::homePath(), tr("SVG files (*.svg)"));
    if (path.isEmpty()) return;
    svgSave(path, noteEdit);
}

// Confirm quit.
bool MainWindow::quitConfirm()
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

void MainWindow::notImplemented()
{
    QMessageBox::critical(this, tr("Error"),
                          tr("This feature is not yet implemented."));
}
