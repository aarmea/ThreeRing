#ifndef NOTEEDITOR_H
#define NOTEEDITOR_H

#include <QDebug>

#include <QWidget>
#include <QLinkedList>
#include <QMap>
#include "table.h"
#include <QKeyEvent>
#include <QTabletEvent>
#include <QPainter>
#include "curve.h"
// TODO: make it work with a mouse, "mouseastablet.h"?

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
    QCursor cursorFromPen(const QPen &pen);

    void tabletEvent(QTabletEvent *event);

    void paintEvent(QPaintEvent *event);
    void paint(QPainter &painter);
    void paint(QPainter &painter, const QRect &clip);
    void updateAround(QRect updateRect, int dist = 5);

    typedef QLinkedList<Curve> drawing_type;
    drawing_type::iterator getNewCurve();
    void addPointToCurve(QPointF point, drawing_type::iterator curve);
    void addBackpointer(QPoint point, drawing_type::iterator curve);
    
signals:
    
public slots:

private:
    typedef QMap<drawing_type::iterator, char> selection_type;

    bool tabletDown;
    Qt::KeyboardModifiers keyMods;
    PenMode penMode;
    MouseMode mouseMode;

    QPen currentPen;
    drawing_type::iterator currentCurve;
    drawing_type drawing;
    Table<selection_type> backpointers;
    QPainter painter;
    QPoint ulCorner;
    QPointF curTabletPos;
    QPointF lastTabletPos;

    Curve selectionBound;
    bool selectionActive;
    selection_type selection;
    void updateSelection();
    void moveSelection(QPointF distance);
    void clearSelection();

    void tabletPressEvent(QTabletEvent *event);
    void tabletReleaseEvent(QTabletEvent *event);
    void tabletMoveEvent(QTabletEvent *event);

    void addBackpointers(drawing_type::iterator curve);
    selection_type getBackpointers(QPoint point) const;
    void removeBackpointers(drawing_type::iterator curve);
    void eraseCurve(QPoint point);
    void eraseCurve(drawing_type::iterator curve);
};

bool operator<(const NoteEditor::drawing_type::iterator &itr1,
               const NoteEditor::drawing_type::iterator &itr2);

#endif // NOTEEDITOR_H
