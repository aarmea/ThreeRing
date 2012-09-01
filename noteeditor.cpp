#include "noteeditor.h"

// Comparison for iterators for QMap in paint()
bool operator<(const drawing_type::iterator &itr1,
               const drawing_type::iterator &itr2)
{
    return itr1.i < itr2.i;
}

// Default constructor.
NoteEditor::NoteEditor(QWidget *parent) :
    QWidget(parent)
{
    tabletDown = false;
    backpointers.resize(width()*2, height()*2, NULL);
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
            addPointToCurve(
                        event->hiResGlobalPos()-ulCorner, currentCurve);
        } else if (event->pointerType() == QTabletEvent::Eraser) {
            eraseCurve(event->pos());
        }
        break;
    case QEvent::TabletRelease:
        tabletDown = false;
        //update(); // Causes slowdown on large documents, put elsewhere
        break;
    case QEvent::TabletMove:
        if (tabletDown) {
            if (event->pointerType() == QTabletEvent::Pen) {
                addPointToCurve(
                            event->hiResGlobalPos()-ulCorner, currentCurve);
                update(event->x()-16, event->y()-16, 32, 32);
            } else if (event->pointerType() == QTabletEvent::Eraser) {
                eraseCurve(event->pos());
                update();
            }
        }
        break;
    default:
        break;
    }
}

// Redraw the entire screen.
void NoteEditor::paintEvent(QPaintEvent *event)
{
    // TODO: only redraw the QRect region defined by event->rect()
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    paint(painter, event->rect());
    painter.end();
}

// Redraw the ink on the screen.
void NoteEditor::paint(QPainter &painter)
{
    paint(painter, QRect(0, 0, width(), height()));
}

void NoteEditor::paint(QPainter &painter, const QRect &clip)
{
    painter.setClipRect(clip);

    if (clip.height()*clip.width() > size().height()*size().width()/4) {
        // Redraw the whole thing
        drawing_type::iterator itr;
        for (itr = drawing.begin(); itr != drawing.end(); ++itr) {
            Curve &curve = *itr;
            painter.setPen(curve.getPen());
            painter.drawPolyline(curve.getPoints(), curve.size());
        }
    } else {
        // Find and redraw only the curves in the clip region
        QMap<drawing_type::iterator, char> curves;
        for (int i = 0; i < clip.width(); ++i) {
            for (int j = 0; j < clip.height(); ++j) {
                drawing_type::iterator curvePtr =
                        backpointers.get(clip.x()+i, clip.y()+j);
                if (curvePtr.i)
                    curves[curvePtr] = 1;
            }
        }
        QMap<drawing_type::iterator, char>::iterator itr;
        for (itr = curves.begin(); itr != curves.end(); ++itr) {
            Curve &curve = *(itr.key());
            painter.setPen(curve.getPen());
            painter.drawPolyline(curve.getPoints(), curve.size());
        }
    }
}

// Add a new curve to the drawing and get an iterator to it.
drawing_type::iterator NoteEditor::getNewCurve()
{
    drawing.push_back(Curve());
    drawing_type::iterator newCurve = drawing.end();
    --newCurve;
    return newCurve;
}

// Add a point to the given curve.
void NoteEditor::addPointToCurve(
        QPointF point, drawing_type::iterator curve)
{
    curve->addPoint(point);
    curve_raster_type rasterPoints =
            curve->getRasterPoints(curve->size()-1);
    for (curve_size_type i = 0; i < rasterPoints.size(); ++i) {
        addBackpointer(rasterPoints[i], curve);
    }
}

// Add a backpointer to the given curve at the given point.
void NoteEditor::addBackpointer(QPoint point, drawing_type::iterator curve)
{
    if (point.x() < 0 || point.y() < 0) return;
    table_size_type x = point.x();
    table_size_type y = point.y();
    if (x > backpointers.numRows()-1 && y > backpointers.numColumns()-1) {
        backpointers.resize(x*2, y*2, NULL);
    } else if (x > backpointers.numRows()-1) {
        backpointers.resize(x*2, backpointers.numColumns(), NULL);
    } else if (y > backpointers.numColumns()-1) {
        backpointers.resize(backpointers.numRows(), y*2, NULL);
    }
    backpointers.set(x, y, curve);
}

drawing_type::iterator NoteEditor::getBackpointer(QPoint point) const
{
    return backpointers.get(point.x(), point.y());
}

// Erase a curve at the given point.
bool NoteEditor::eraseCurve(QPoint point)
{
    drawing_type::iterator curve;
    for (int x = -2; x <= 2; ++x) {
        if (point.x()+x < 0 || point.x()+x > backpointers.numRows()-1)
            continue;
        for (int y = -2; y <= 2; ++y) {
            if (point.y()+y < 0 ||
                    point.y()+y > backpointers.numColumns()-1)
                continue;
            curve = backpointers.get(point.x()+x, point.y()+y);
            if (curve.i) {
                eraseCurve(curve);
                return true;
            }
        }
    }
    return false;
}

// Erase the curve specified by the iterator.
void NoteEditor::eraseCurve(drawing_type::iterator curve)
{
    curve_size_type numPoints = curve->size();

    for (curve_size_type i = 0; i < numPoints; ++i) {
        curve_raster_type rasterPoints = curve->getRasterPoints(i);
        for (curve_size_type j = 0; j < rasterPoints.size(); ++j) {
            QPoint point = rasterPoints[j];
            if (point.x() < 0 || point.x() > backpointers.numRows()-1)
                continue;
            if (point.y() < 0 || point.y() > backpointers.numColumns()-1)
                continue;
            backpointers.set(point.x(), point.y(), NULL);
        }
    }

    drawing.erase(curve);
}
