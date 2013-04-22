#ifndef PENSELECTBUTTON_H
#define PENSELECTBUTTON_H

#include <QToolButton>
#include <QPoint>
#include <QPainter>
#include <QPen>

class PenSelectButton : public QToolButton
{
    Q_OBJECT
public:
    explicit PenSelectButton(QBrush brush = Qt::black, qreal width = 1.0,
                             QWidget *parent = 0);
    PenSelectButton(const QPen &newPen, QWidget *parent = 0);

signals:
    void clicked(QPen pen);

public slots:

private slots:
    void reemitClicked();

private:
    QPen pen_;
    const static QSize iconSize;
    const static int iconPointsSize;
    const static QPointF iconPoints[];

    void init();
};

#endif // PENSELECTBUTTON_H
