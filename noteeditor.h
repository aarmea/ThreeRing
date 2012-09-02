#ifndef NOTEEDITOR_H
#define NOTEEDITOR_H

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

    enum PenMode {PenPen, PenMouse, PenErase, PenSelect, PenMove};
    enum MouseMode {MouseMouse, MousePen};

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void tabletEvent(QTabletEvent *event);

    void paintEvent(QPaintEvent *event);
    void paint(QPainter &painter);
    void paint(QPainter &painter, const QRect &clip);

    typedef QLinkedList<Curve> drawing_type;
    drawing_type::iterator getNewCurve();
    void addPointToCurve(QPointF point, drawing_type::iterator curve);
    void addBackpointer(QPoint point, drawing_type::iterator curve);
    
signals:
    
public slots:

private:
    bool tabletDown;
    Qt::KeyboardModifiers keyMods;
    PenMode penMode;
    MouseMode mouseMode;

    drawing_type::iterator currentCurve;
    drawing_type drawing;
    Table<drawing_type::iterator> backpointers;
    QPainter painter;
    QPoint ulCorner;

    typedef QMap<drawing_type::iterator, char> selection_type;
    Curve selectionBound;
    bool selectionActive;
    selection_type selection;
    QRect updateSelection();
    void clearSelection();

    void tabletPressEvent(QTabletEvent *event);
    void tabletReleaseEvent(QTabletEvent *event);
    void tabletMoveEvent(QTabletEvent *event);

    drawing_type::iterator getBackpointer(QPoint point) const;
    QRect eraseCurve(QPoint point);
    QRect eraseCurve(drawing_type::iterator curve);
};

bool operator<(const NoteEditor::drawing_type::iterator &itr1,
               const NoteEditor::drawing_type::iterator &itr2);

#endif // NOTEEDITOR_H
