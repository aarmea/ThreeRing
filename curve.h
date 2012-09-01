#ifndef CURVE_H
#define CURVE_H

#include <QObject>
#include <QVector>
#include <QPointF>
#include <QPen>

typedef QVector<QPointF> curve_type;
typedef curve_type::size_type curve_size_type;
typedef QVector<QPoint> curve_raster_type;

class Curve : public QObject
{
    Q_OBJECT
public:
    explicit Curve(QObject *parent = 0);
    Curve(const Curve &otherCurve);

    void addPoint(const QPointF &point);
    void clear();
    QPointF getPoint(curve_size_type n);
    QPointF* getPoints();
    curve_raster_type getRasterPoints(curve_size_type n);
    curve_size_type size() const;

    const QPen& getPen() const;
    void setPen(const QPen &newPen);
    
signals:
    
public slots:

private:
    QPen pen;
    curve_type points;
    
};

#endif // CURVE_H
