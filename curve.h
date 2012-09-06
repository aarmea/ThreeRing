#ifndef CURVE_H
#define CURVE_H

#include <QPolygonF>
#include <QPen>

// TODO: implement getters and setters for selected
// select(), deselect(), isSelected()
class Curve : public QPolygonF
{
public:
    explicit Curve();
    Curve(const Curve &otherCurve);
    Curve(QPen newPen);

    void select();
    void deselect();
    bool isSelected();

    typedef QVector<QPoint> raster_type;
    raster_type getRasterPoints(size_type n);
    const QPen& getPen() const;
    void setPen(const QPen &newPen);
    
signals:
    
public slots:

private:
    bool selected;
    QPen pen;
    
};

#endif // CURVE_H
