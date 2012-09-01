#include "noteeditor.h"

// Default constructor.
NoteEditor::NoteEditor(QWidget *parent) :
    QWidget(parent)
{
    tabletDown = false;
}

// Handle tablet events.
void NoteEditor::tabletEvent(QTabletEvent *event)
{
    ulCorner = mapToGlobal(QPoint(0,0));

    switch (event->type()) {
    case QEvent::TabletPress:
        tabletDown = true;
        if (event->pointerType() == QTabletEvent::Pen) {
            currentCurve = getNewCurve();
            currentCurve->addPoint(event->hiResGlobalPos() - ulCorner);
        }
        break;
    case QEvent::TabletRelease:
        tabletDown = false;
        break;
    case QEvent::TabletMove:
        if (tabletDown && event->pointerType() == QTabletEvent::Pen) {
            currentCurve->addPoint(event->hiResGlobalPos() - ulCorner);
            update();
        }
        break;
    default:
        break;
    }
}

// Redraw the screen.
void NoteEditor::paintEvent(QPaintEvent *event)
{
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    paint(painter);
    painter.end();
}

// Add a new curve to the drawing and get an iterator to it.
drawType::iterator NoteEditor::getNewCurve()
{
    drawing.push_back(Curve());
    drawType::iterator newCurve = drawing.end();
    --newCurve;
    return newCurve;
}

// Redraw the ink on the screen.
void NoteEditor::paint(QPainter &painter)
{
    painter.setClipRect(QRect(0, 0, width(), height()));

    drawType::iterator itr;
    for (itr = drawing.begin(); itr != drawing.end(); ++itr) {
        Curve &curPolyline = *itr;
        painter.setPen(curPolyline.getPen());
        painter.drawPolyline(curPolyline.getPoints(), curPolyline.size());
    }
}


