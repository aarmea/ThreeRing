#include "noteeditor.h"

// Comparison for iterators for QMap in selection_type
bool operator<(const NoteEditor::drawing_type::iterator &itr1,
               const NoteEditor::drawing_type::iterator &itr2)
{
    return itr1.i < itr2.i;
}

// Default constructor.
NoteEditor::NoteEditor(QWidget *parent) :
    QWidget(parent)
{
    tabletDown = false;
    selectionActive = false;
    keyMods = Qt::NoModifier;
    penMode = PenPen;
    currentPen = QPen(QBrush(Qt::black), 1.0,
                      Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    mouseMode = MouseMouse;
    setFocusPolicy(Qt::StrongFocus); // for Mac keyboards
    grabKeyboard();
    backpointers.resize(width()*2, height()*2, NULL);
}

// Clear the entire document.
void NoteEditor::clear()
{
    drawing.clear();
    backpointers.resize(0, 0);
    backpointers.resize(width()*2, height()*2, NULL);
}

// Handle keyboard events.
void NoteEditor::keyPressEvent(QKeyEvent *event)
{
    keyMods = event->modifiers();

    // Delete the selection (if present)
    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        for (selection_type::iterator itr = selection.begin();
             itr != selection.end(); ++itr) {
            eraseCurve(itr.key());
        }
        selection.clear();
        clearSelection();
    }
}

void NoteEditor::keyReleaseEvent(QKeyEvent *event)
{
    keyMods = event->modifiers();
}

// Setters and getters for penMode
void NoteEditor::setPenMode(PenMode newPenMode)
{
    // TODO: set pen mode when shift or alt is pressed or eraser detected
    penMode = newPenMode;
    switch (penMode) {
    /* TODO: for case PenPen, make the cursor the current pen
    case PenPen:
        setCursor(cursorFromPen(QPen pen));
        break;
    */
    case PenSelect:
        setCursor(Qt::CrossCursor);
        break;
    default:
        setCursor(Qt::ArrowCursor);
        break;
    }
}

NoteEditor::PenMode NoteEditor::getPenMode() const
{
    return penMode;
}

// Create a cursor from the given pen.
QCursor NoteEditor::cursorFromPen(const QPen &pen)
{
    // TODO: implement
    // Create a QPixmap, draw a single point in it,
    // then create a QCursor from that QPixmap
    return QCursor(); // dummy return
}

// Handle tablet events.
void NoteEditor::tabletEvent(QTabletEvent *event)
{
    ulCorner = mapToGlobal(QPoint(0,0));

    // TODO: handle case (penMode == PenMouse)
    switch (event->type()) {
    case QEvent::TabletPress:
        tabletPressEvent(event);
        break;
    case QEvent::TabletRelease:
        tabletReleaseEvent(event);
        break;
    case QEvent::TabletMove:
        tabletMoveEvent(event);
        break;
    default:
        break;
    }
    event->accept();
}

void NoteEditor::tabletPressEvent(QTabletEvent *event)
{
    tabletDown = true;
    clearSelection();
    if (keyMods & Qt::ShiftModifier) {
        // penMode = PenSelect;
        setPenMode(PenSelect);
        selectionActive = true;
        selectionBound.append(event->pos());
    } else if (event->pointerType() == QTabletEvent::Eraser ||
               keyMods & Qt::AltModifier) {
        // penMode = PenErase;
        setPenMode(PenErase);
        eraseCurve(event->pos());
    } else if (event->pointerType() == QTabletEvent::Pen) {
        // penMode = PenPen;
        setPenMode(PenPen);
        currentCurve = getNewCurve();
        addPointToCurve(event->hiResGlobalPos()-ulCorner, currentCurve);
    }
}

void NoteEditor::tabletReleaseEvent(QTabletEvent *event)
{
    tabletDown = false;
    if (getPenMode() == PenSelect) {
        QRect updateRect = selectionBound.boundingRect().toAlignedRect();
        updateRect.adjust(-5, -5, 5, 5);
        update(updateRect);
        selectionActive = false;
    }
    // penMode = PenPen;
    setPenMode(PenPen); // TODO: handle moving a selection
}

void NoteEditor::tabletMoveEvent(QTabletEvent *event)
{
    QRect updateRect;
    if (tabletDown) {
        switch (getPenMode()) {
        case PenPen:
            addPointToCurve(event->hiResGlobalPos()-ulCorner, currentCurve);
            updateRect = currentCurve->boundingRect().toAlignedRect();
            break;
        case PenErase:
            updateRect = eraseCurve(event->pos());
            break;
        case PenSelect:
            selectionBound.append(event->hiResGlobalPos()-ulCorner);
            updateRect = updateSelection();
            break;
        default:
            break;
        }
    }
    if (!updateRect.isNull()) {
        updateRect.adjust(-5, -5, 5, 5);
        update(updateRect);
    }
}

// Redraw a portion of all layers of the screen.
void NoteEditor::paintEvent(QPaintEvent *event)
{
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

    if (clip.height()*clip.width() > height()*width()/4) {
        // Redraw the whole thing
        drawing_type::iterator itr;
        for (itr = drawing.begin(); itr != drawing.end(); ++itr) {
            Curve &curve = *itr;
            QPen pen = curve.getPen();
            if (curve.isSelected()) {
                QPen outerPen = pen;
                outerPen.setWidthF(outerPen.widthF()+2);
                painter.setPen(outerPen);
                painter.drawPolyline(curve);
                pen.setColor(Qt::white);
            }
            painter.setPen(pen);
            painter.drawPolyline(curve);
        }
    } else {
        // Find and redraw only the curves in the clip region
        selection_type curves;
        for (int i = 0; i < clip.width(); ++i) {
            if (clip.x()+i >= backpointers.numRows()-1) continue;
            for (int j = 0; j < clip.height(); ++j) {
                if (clip.y()+j >= backpointers.numColumns()-1) continue;
                drawing_type::iterator curvePtr =
                        backpointers.get(clip.x()+i, clip.y()+j);
                if (curvePtr.i)
                    curves[curvePtr] = 1;
            }
        }
        selection_type::iterator itr;
        for (itr = curves.begin(); itr != curves.end(); ++itr) {
            Curve &curve = *(itr.key());
            QPen pen = curve.getPen();
            if (curve.isSelected()) {
                QPen outerPen = pen;
                outerPen.setWidthF(outerPen.widthF()+2);
                painter.setPen(outerPen);
                painter.drawPolyline(curve);
                pen.setColor(Qt::white);
            }
            painter.setPen(pen);
            painter.drawPolyline(curve);
        }
    }

    // Draw the selection bound
    if (selectionActive) {
        painter.setPen(QPen(QBrush(Qt::blue), 1.0, Qt::DashLine,
                            Qt::RoundCap, Qt::RoundJoin));
        painter.drawPolyline(selectionBound);
        painter.drawLine(selectionBound.last(), selectionBound.first());
    }
}

// Add a new curve to the drawing and get an iterator to it.
NoteEditor::drawing_type::iterator NoteEditor::getNewCurve()
{
    drawing.push_back(Curve(currentPen));
    drawing_type::iterator newCurve = drawing.end();
    --newCurve;
    return newCurve;
}

// Add a point to the given curve.
void NoteEditor::addPointToCurve(
        QPointF point, drawing_type::iterator curve)
{
    curve->append(point);
    Curve::raster_type rasterPoints =
            curve->getRasterPoints(curve->size()-1);
    for (Curve::size_type i = 0; i < rasterPoints.size(); ++i) {
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

NoteEditor::drawing_type::iterator NoteEditor::getBackpointer(
        QPoint point) const
{
    int x = point.x();
    int y = point.y();
    if (x < 0 || x > width()) return drawing_type::iterator();
    if (y < 0 || y > height()) return drawing_type::iterator();
    return backpointers.get(point.x(), point.y());
}

// Maintain the selection.
QRect NoteEditor::updateSelection() {
    // TODO: speed up by only checking the new area at each point

    selection_type newSelection;

    // Populate selection
    QRegion region(selectionBound.toPolygon());
    QVector<QRect> regionRects = region.rects();
    for (QVector<QRect>::size_type i = 0; i < regionRects.size(); ++i) {
        QRect &rect = regionRects[i];
        for (int x = rect.left(); x <= rect.right(); ++x) {
            for (int y = rect.top(); y <= rect.bottom(); ++y) {
                QPoint newPoint(x, y);
                drawing_type::iterator curvePtr = getBackpointer(newPoint);
                if (curvePtr.i) {
                    newSelection.insert(curvePtr, 1);
                    curvePtr->select();
                }
            }
        }
    }

    // Remove curves at edge from selection
    for (Curve::size_type i = 0; i <= selectionBound.size(); ++i) {
        // yes, "<=" in conditional to check the implicit closing edge
        Curve::raster_type edge = selectionBound.getRasterPoints(i);
        for (Curve::raster_type::size_type j = 0; j < edge.size(); ++j) {
            // newSelection.remove(getBackpointer(edge[j]));
            QPoint point = edge[j];
            for (int x = -1; x <= 1; ++x) {
                for (int y = -1; y <= 1; ++y) {
                    QPoint newPoint(point.x()+x, point.y()+y);
                    drawing_type::iterator curvePtr = getBackpointer(newPoint);
                    if (newSelection.remove(curvePtr)) {
                        curvePtr->deselect();
                    }
                }
            }
        }
    }

    selection = newSelection;

    return selectionBound.boundingRect().toAlignedRect();
}

// Clear the selection.
void NoteEditor::clearSelection()
{
    QRect selectionRect = selectionBound.boundingRect().toAlignedRect();

    for (selection_type::iterator itr = selection.begin();
         itr != selection.end(); ++itr) {
        itr.key()->deselect();
    }
    selection.clear();
    selectionBound.clear();
    selectionActive = false;

    if (!selectionRect.isNull()) {
        selectionRect.adjust(-5, -5, 5, 5);
        update(selectionRect);
    }
}

// Erase a curve at the given point.
QRect NoteEditor::eraseCurve(QPoint point)
{
    drawing_type::iterator curve;
    for (int x = -2; x <= 2; ++x) {
        if (point.x()+x < 0 || point.x()+x > backpointers.numRows()-1)
            continue;
        for (int y = -2; y <= 2; ++y) {
            if (point.y()+y < 0 || point.y()+y > backpointers.numColumns()-1)
                continue;
            curve = backpointers.get(point.x()+x, point.y()+y);
            if (curve.i) {
                return eraseCurve(curve);
            }
        }
    }
    return QRect();
}

// Erase the curve specified by the iterator.
QRect NoteEditor::eraseCurve(drawing_type::iterator curve)
{
    Curve::size_type numPoints = curve->size();

    for (Curve::size_type i = 0; i < numPoints; ++i) {
        Curve::raster_type rasterPoints = curve->getRasterPoints(i);
        for (Curve::size_type j = 0; j < rasterPoints.size(); ++j) {
            QPoint point = rasterPoints[j];
            if (point.x() < 0 || point.x() > backpointers.numRows()-1)
                continue;
            if (point.y() < 0 || point.y() > backpointers.numColumns()-1)
                continue;
            backpointers.set(point.x(), point.y(), NULL);
        }
    }
    QRect rect = curve->boundingRect().toAlignedRect();
    drawing.erase(curve);
    return rect;
}
