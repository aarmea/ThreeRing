#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    mainWidget = new QWidget;
    mainLayout = new QVBoxLayout;
    noteEdit = new NoteEditor;
    ribbon = new Ribbon(this, noteEdit);

    mainLayout->addWidget(ribbon);
    mainLayout->addWidget(noteEdit);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

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
    if (!path.isEmpty()) {
        noteEdit->clear();
        svgOpen(path, noteEdit);
    }
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
