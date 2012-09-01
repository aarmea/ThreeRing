#ifndef NOTEEDITOR_H
#define NOTEEDITOR_H

#include <QWidget>
#include <QLinkedList>
#include <QMap>
#include "table.h"
#include <QTabletEvent>
#include <QPainter>
#include "curve.h"
//TODO: make it work with a mouse, "mouseastablet.h"

typedef QLinkedList<Curve> drawing_type;
typedef Table<drawing_type::iterator> bptr_type;

bool operator<(const drawing_type::iterator &itr1,
               const drawing_type::iterator &itr2);

class NoteEditor : public QWidget
{
    Q_OBJECT
public:
    explicit NoteEditor(QWidget *parent = 0);
    void tabletEvent(QTabletEvent *event);
    void paintEvent(QPaintEvent *event);
    void paint(QPainter &painter);
    void paint(QPainter &painter, const QRect &clip);

    drawing_type::iterator getNewCurve();
    void addPointToCurve(QPointF point, drawing_type::iterator curve);
    void addBackpointer(QPoint point, drawing_type::iterator curve);
    
signals:
    
public slots:

private:
    bool tabletDown;

    drawing_type::iterator currentCurve;
    drawing_type drawing;
    bptr_type backpointers;
    QPainter painter;
    QPoint ulCorner;

    drawing_type::iterator getBackpointer(QPoint point) const;
    bool eraseCurve(QPoint point);
    void eraseCurve(drawing_type::iterator curve);
};

#endif // NOTEEDITOR_H
