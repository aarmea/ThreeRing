#ifndef NOTEEDITOR_H
#define NOTEEDITOR_H

#include <QDebug>

#include <QWidget>
#include <QLinkedList>
#include <QStack>
#include <QMap>
#include "table.h"
#include <QKeyEvent>
#include <QTabletEvent>
#include <QPainter>
#include "noteeditorcommand.h"
#include "curve.h"
// TODO: make it work with a mouse, "mouseastablet.h"?

class NoteEditorCommand;

class NoteEditor : public QWidget
{
    Q_OBJECT
public:
    explicit NoteEditor(QWidget *parent = 0);
    void clear();

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    enum PenMode {PenPen, PenMouse, PenErase, PenSelect, PenMove};
    enum MouseMode {MouseMouse, MousePen};
    void setPenMode(PenMode newPenMode);
    PenMode getPenMode() const;
    enum PenCursorMode {Standalone, WithPencil};
    QCursor cursorFromPen(const QPen &pen, PenCursorMode mode = WithPencil);

    void tabletEvent(QTabletEvent *event);

    void paintEvent(QPaintEvent *event);
    void paint(QPainter &painter);
    void paint(QPainter &painter, const QRect &clip);
    void updateAround(QRect updateRect, int dist = 5);

    typedef QMap<Curve::hash_type, Curve *> drawing_type;
    void addPointToCurve(QPointF point, Curve *curve);
    void addCurve(Curve *curve);

signals:
    
public slots:
    void setPen(QPen newPen);

    void undo();
    void redo();

private:
    typedef QMap<Curve *, char> selection_type;

    friend class NoteEditorCommand;
    QStack<NoteEditorCommand> history;
    QLinkedList<Curve> changedCurves;

    bool tabletDown;
    Qt::KeyboardModifiers keyMods;
    PenMode penMode;
    MouseMode mouseMode;

    QPen currentPen;
    Curve *currentCurve;
    drawing_type drawing;
    Table<selection_type> backpointers;
    QPainter painter;
    QPoint ulCorner;
    QPointF pressTabletPos;
    QPointF curTabletPos;
    QPointF lastTabletPos;

    Curve selectionBound;
    bool selectionActive;
    selection_type selection;
    selection_type selectionGreedy;
    void updateSelection();
    void moveSelection(QPointF distance);
    void clearSelection();

    void tabletPressEvent(QTabletEvent *event);
    void tabletReleaseEvent(QTabletEvent *event);
    void tabletMoveEvent(QTabletEvent *event);

    void addBackpointer(QPoint point, Curve *curve);
    void addBackpointers(Curve *curve);
    selection_type getBackpointers(QPoint point) const;
    void removeBackpointers(Curve *curve);
    void eraseCurve(QPoint point);
    void eraseCurve(Curve::hash_type curveHash);
    void eraseCurve(Curve *curve);
};

#endif // NOTEEDITOR_H
