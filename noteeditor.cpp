#include "noteeditor.h"

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
    currentCurve = NULL;
    mouseMode = MouseMouse;
    setFocusPolicy(Qt::StrongFocus); // for Mac keyboards
    // grabKeyboard();
    backpointers.resize(width()*2, height()*2, selection_type());
}

// Clear the entire document.
void NoteEditor::clear()
{
    drawing.clear();
    backpointers.resize(0, 0);
    backpointers.resize(width()*2, height()*2, selection_type());
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

// Set the pen
void NoteEditor::setPen(QPen newPen)
{
    currentPen = newPen;
}

// Setters and getters for penMode
void NoteEditor::setPenMode(PenMode newPenMode)
{
    // Should only work if pen is up
    if (tabletDown) return;

    penMode = newPenMode;
    switch (penMode) {
    case PenPen:
        // TODO: use the ":/images/pencil.png" cursor if MousePen
        setCursor(cursorFromPen(currentPen));
        break;
    case PenErase:
        setCursor(QCursor(QPixmap(":/images/eraser.png"), 6, 6));
        break;
    case PenSelect:
        setCursor(Qt::CrossCursor);
        break;
    case PenMove:
        setCursor(QCursor(QPixmap(":/images/move.png")));
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
QCursor NoteEditor::cursorFromPen(const QPen &pen, PenCursorMode mode)
{
    // Create a QPixmap, draw a single point in it,
    // then create a QCursor from that QPixmap
    QPixmap pixmap(32, 32);
    if (mode == Standalone)
        pixmap.fill(Qt::transparent);
    else
        pixmap.load(":/images/pencil.png");
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen newPen = pen;
    newPen.setWidthF(newPen.widthF()+1);
    painter.setPen(newPen);
    painter.drawPoint(QPoint(8, 8));
    return QCursor(pixmap, 8, 8);
}

// Handle tablet events.
void NoteEditor::tabletEvent(QTabletEvent *event)
{
    ulCorner = mapToGlobal(QPoint(0,0));
    grabKeyboard();

    // Check the surrounding points for selected Curves
    selection_type surPoints;
    for (int x = -4; x <= 4; ++x) {
        for (int y = -4; y <= 4; ++y) {
            QPoint point = QPoint(event->pos().x()+x, event->pos().y()+y);
            surPoints = surPoints.unite(getBackpointers(point));
        }
    }
    QList<Curve *> uniqueCurves = surPoints.uniqueKeys();

    // Change the penMode accordingly
    if (uniqueCurves.begin() != uniqueCurves.end() &&
            selection.contains(*uniqueCurves.begin())) {
        setPenMode(PenMove);
    } else if (keyMods & Qt::ShiftModifier) {
        setPenMode(PenSelect);
    } else if (event->pointerType() == QTabletEvent::Eraser ||
            keyMods & Qt::AltModifier) {
        setPenMode(PenErase);
    } else if (event->pointerType() == QTabletEvent::Pen) {
        setPenMode(PenPen);
    }

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
    pressTabletPos = event->hiResGlobalPos()-ulCorner;
    changedCurves.clear();
    if (getPenMode() != PenMove) {
        clearSelection();
    }

    switch (getPenMode()) {
    case PenPen:
        setCursor(cursorFromPen(currentPen, Standalone));
        currentCurve = new Curve(currentPen);
        addPointToCurve(event->hiResGlobalPos()-ulCorner, currentCurve);
        break;
    case PenErase:
        eraseCurve(event->pos());
        break;
    case PenSelect:
        selectionActive = true;
        selectionBound.append(event->hiResGlobalPos()-ulCorner);
        break;
    default:
        break;
    }
}

void NoteEditor::tabletReleaseEvent(QTabletEvent *event)
{
    // Ensure that the tabletReleaseEvent has a corresponding tabletPressEvent
    if (tabletDown == false) return;

    tabletDown = false;
    releaseKeyboard();
    switch (getPenMode()) {
    case PenPen:
        setCursor(cursorFromPen(currentPen, WithPencil));
        addCurve(currentCurve);
        qDebug() << "Curve hash:" << currentCurve->getHash(Curve::Update);
        qDebug() << "    with first point:" << currentCurve->first();
        changedCurves.push_back(*currentCurve);
        history.push(NoteEditorCommand(this, NoteEditorCommand::AddCurves,
                                       changedCurves));
        currentCurve = NULL;
        break;
    case PenErase:
        if (changedCurves.size() > 0)
            history.push(NoteEditorCommand(this,
                                           NoteEditorCommand::RemoveCurves,
                                           changedCurves));
        break;
    case PenSelect:
        updateAround(selectionBound.boundingRect().toAlignedRect());
        selectionActive = false;
        break;
    default:
        break;
    }
}

void NoteEditor::tabletMoveEvent(QTabletEvent *event)
{
    lastTabletPos = curTabletPos;
    curTabletPos = event->hiResGlobalPos();

    if (tabletDown) {
        switch (getPenMode()) {
        case PenPen:
            addPointToCurve(event->hiResGlobalPos()-ulCorner, currentCurve);
            updateAround(currentCurve->boundingRect().toAlignedRect());
            break;
        case PenErase:
            eraseCurve(event->pos());
            break;
        case PenSelect:
            selectionBound.append(event->hiResGlobalPos()-ulCorner);
            updateSelection();
            break;
        case PenMove:
            moveSelection(curTabletPos - lastTabletPos);
            break;
        default:
            break;
        }
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
            Curve *curve = *itr;
            QPen pen = curve->getPen();
            if (selection.contains(curve)) {
                QPen outerPen = pen;
                outerPen.setWidthF(outerPen.widthF()+2);
                painter.setPen(outerPen);
                painter.drawPolyline(*curve);
                pen.setColor(Qt::white);
            }
            painter.setPen(pen);
            painter.drawPolyline(*curve);
        }
        // Separately redraw the currentCurve
        if (currentCurve) {
            painter.setPen(currentCurve->getPen());
            painter.drawPolyline(*currentCurve);
        }
    } else {
        // Find and redraw only the curves in the clip region
        selection_type curves;
        for (int i = 0; i < clip.width(); ++i) {
            if (clip.x()+i >= backpointers.numRows()-1) continue;
            for (int j = 0; j < clip.height(); ++j) {
                if (clip.y()+j >= backpointers.numColumns()-1) continue;
                selection_type curvePtrs =
                        backpointers.get(clip.x()+i, clip.y()+j);
                selection_type::iterator itr;
                for (itr = curvePtrs.begin(); itr != curvePtrs.end(); ++itr) {
                    Curve *curvePtr = itr.key();
                    if (curvePtr)
                        curves[curvePtr] = 1;
                }
            }
        }
        selection_type::iterator itr;
        for (itr = curves.begin(); itr != curves.end(); ++itr) {
            Curve &curve = *(itr.key());
            QPen pen = curve.getPen();
            if (selection.contains(itr.key())) {
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
    // The currentCurve has backpointers in the clip region,
    // so it is implicitly redrawn

    // Draw the selection bound
    if (selectionActive) {
        painter.setPen(QPen(QBrush(Qt::blue), 1.0, Qt::DashLine,
                            Qt::RoundCap, Qt::RoundJoin));
        painter.drawPolyline(selectionBound);
        painter.drawLine(selectionBound.last(), selectionBound.first());
    }
}

void NoteEditor::updateAround(QRect updateRect, int dist)
{
    updateRect.adjust(-dist, -dist, dist, dist);
    update(updateRect);
}

// Add a point to the given curve.
void NoteEditor::addPointToCurve(QPointF point, Curve *curve)
{
    curve->append(point);
    Curve::raster_type rasterPoints =
            curve->getRasterPoints(curve->size()-1);
    for (Curve::size_type i = 0; i < rasterPoints.size(); ++i) {
        addBackpointer(rasterPoints[i], curve);
    }
}

// Add a curve to the representation.
void NoteEditor::addCurve(Curve *curve)
{
    drawing[curve->getHash()] = curve;
}

// Add a backpointer to the given curve at the given point.
void NoteEditor::addBackpointer(QPoint point, Curve *curve)
{
    if (point.x() < 0 || point.y() < 0) return;
    table_size_type x = point.x();
    table_size_type y = point.y();
    if (x > backpointers.numRows()-1 && y > backpointers.numColumns()-1) {
        backpointers.resize(x*2, y*2, selection_type());
    } else if (x > backpointers.numRows()-1) {
        backpointers.resize(x*2, backpointers.numColumns(), selection_type());
    } else if (y > backpointers.numColumns()-1) {
        backpointers.resize(backpointers.numRows(), y*2, selection_type());
    }
    selection_type curves = backpointers.get(x, y);
    curves[curve] = 1;
    backpointers.set(x, y, curves);
}

// Add all of the backpointers to a given curve.
void NoteEditor::addBackpointers(Curve *curve)
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
            addBackpointer(point, curve);
        }
    }
}

// Get all of the backpointers at a given point.
selection_type NoteEditor::getBackpointers(
        QPoint point) const
{
    int x = point.x();
    int y = point.y();
    if (x < 0 || x > width()) return selection_type();
    if (y < 0 || y > height()) return selection_type();
    return backpointers.get(point.x(), point.y());
}

// Remove the backpointers associated with the given curve.
void NoteEditor::removeBackpointers(Curve *curve)
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
            selection_type curves = getBackpointers(point);
            curves.remove(curve);
            backpointers.set(point.x(), point.y(), curves);
        }
    }
}

// Maintain the selection.
void NoteEditor::updateSelection() {
    // selectionBound must be at least a triangle
    if (selectionBound.size() < 3) return;

    // Populate selection
    QPolygon regionPolygon(3);
    regionPolygon[0] = selectionBound[0].toPoint();
    regionPolygon[1] = selectionBound[selectionBound.size()-2].toPoint();
    regionPolygon[2] = selectionBound[selectionBound.size()-1].toPoint();
    QRegion region(regionPolygon);
    QRegion selectionRegion(selectionBound.toPolygon());
    QVector<QRect> regionRects = region.rects();
    for (QVector<QRect>::size_type i = 0; i < regionRects.size(); ++i) {
        QRect &rect = regionRects[i];
        for (int x = rect.left(); x <= rect.right(); ++x) {
            for (int y = rect.top(); y <= rect.bottom(); ++y) {
                QPoint newPoint(x, y);
                selection_type curvePtrs = getBackpointers(newPoint);
                selection_type::iterator itr;
                for (itr = curvePtrs.begin(); itr != curvePtrs.end(); ++itr) {
                    Curve *curvePtr = &(*itr.key());
                    if (selectionRegion.contains(newPoint)) {
                        selectionGreedy.insert(curvePtr, 1);
                    } else {
                        selectionGreedy.remove(curvePtr);
                    }
                }
            }
        }
    }

    // Remove curves at edge from selection
    selection_type selectionExcludes;
    for (Curve::size_type i = 0; i <= selectionBound.size(); ++i) {
        // yes, "<=" in conditional to check the implicit closing edge
        Curve::raster_type edge = selectionBound.getRasterPoints(i);
        for (Curve::raster_type::size_type j = 0; j < edge.size(); ++j) {
            QPoint point = edge[j];
            for (int x = -1; x <= 1; ++x) {
                for (int y = -1; y <= 1; ++y) {
                    QPoint newPoint(point.x()+x, point.y()+y);
                    selection_type curvePtrs = getBackpointers(newPoint);
                    selection_type::iterator itr;
                    for (itr = curvePtrs.begin(); itr != curvePtrs.end(); ++itr) {
                        Curve *curvePtr = &(*itr.key());
                        selectionExcludes.insert(curvePtr, 1);
                    }
                }
            }
        }
    }

    // Set subtraction
    selection.clear();
    selection_type::iterator itr;
    for (itr = selectionGreedy.begin(); itr != selectionGreedy.end(); ++itr) {
        Curve *curvePtr = &(*itr.key());
        if (!selectionExcludes.contains(curvePtr)) {
            selection.insert(curvePtr, 1);
        }
    }

    updateAround(selectionBound.boundingRect().toAlignedRect());
}

// Move the selection in the direction of distance relative to the origin.
void NoteEditor::moveSelection(QPointF distance)
{
    selection_type::iterator itr;
    selectionBound.translate(distance);
    for (itr = selection.begin(); itr != selection.end(); ++itr) {
        Curve *curve = itr.key();
        removeBackpointers(curve);
        QRect oldRect = curve->boundingRect().toAlignedRect();
        drawing.remove(curve->getHash());
        curve->translate(distance);
        curve->updateHash();
        addCurve(curve);
        addBackpointers(curve);
        updateAround(oldRect);
        updateAround(curve->boundingRect().toAlignedRect());
    }
}

// Clear the selection.
void NoteEditor::clearSelection()
{
    QRect selectionRect = selectionBound.boundingRect().toAlignedRect();

    selection.clear();
    selectionGreedy.clear();
    selectionBound.clear();
    selectionActive = false;

    if (!selectionRect.isNull()) {
        updateAround(selectionRect);
    }
}

// Erase a curve near the given point.
void NoteEditor::eraseCurve(QPoint point)
{
    for (int x = -2; x <= 2; ++x) {
        if (point.x()+x < 0 || point.x()+x > backpointers.numRows()-1)
            continue;
        for (int y = -2; y <= 2; ++y) {
            if (point.y()+y < 0 || point.y()+y > backpointers.numColumns()-1)
                continue;
            selection_type curves = backpointers.get(point.x()+x, point.y()+y);
            if (curves.begin() != curves.end()) {
                changedCurves.push_back(*curves.begin().key());
                eraseCurve(curves.begin().key());
            }
        }
    }
}

// Erase the curve specified by the curveHash.
void NoteEditor::eraseCurve(Curve::hash_type curveHash)
{
    Curve *curve = drawing.value(curveHash);
    if (curve) eraseCurve(curve);
}

// Erase the curve specified by the iterator.
void NoteEditor::eraseCurve(Curve *curve)
{
    removeBackpointers(curve);

    QRect rect = curve->boundingRect().toAlignedRect();
    drawing.remove(curve->getHash());
    qDebug() << "eraseCurve() --> " << rect;
    updateAround(rect);

    delete curve;
}

// Manage the history.
void NoteEditor::undo()
{
    if (history.isEmpty()) return;

    history.pop().unexecute();
}

void NoteEditor::redo()
{
    if (history.isEmpty()) return;

    // TODO: implement!
}
