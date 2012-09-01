#ifndef NOTEEDITOR_H
#define NOTEEDITOR_H

#include <QWidget>
#include <QTabletEvent>
#include <QPainter>
#include <QLinkedList>
#include "curve.h"
//TODO: make it work with a mouse, "mouseastablet.h"
/*TODO: implement erasing
    New data structure that keeps track of all lines at a given pixel
    QVector<QVector<Curve*> >
    Update size on window resize (but only when it gets bigger)
    Update contents every time a line is drawn
    erase() does something along the lines of:
        find the line by looking it up in the table
        set all lineType pointers that refer to that line to zero
        remove that line from the drawing variable
        update();
  */

typedef QLinkedList<Curve> drawType;

class NoteEditor : public QWidget
{
    Q_OBJECT
public:
    explicit NoteEditor(QWidget *parent = 0);
    void tabletEvent(QTabletEvent *event);
    void paintEvent(QPaintEvent *event);
    
signals:
    
public slots:

private:
    bool tabletDown;

    drawType::iterator currentCurve;
    drawType drawing;
    QPainter painter;
    QPoint ulCorner;

    drawType::iterator getNewCurve();

    void paint(QPainter &painter);
};

#endif // NOTEEDITOR_H
