#ifndef CURVE_H
#define CURVE_H

#include <QPolygonF>
#include <QPen>
#include <QCryptographicHash>

class Curve : public QPolygonF
{
public:
    explicit Curve();
    Curve(const Curve &otherCurve);
    Curve(QPen newPen);

    typedef QVector<QPoint> raster_type;
    raster_type getRasterPoints(size_type n);
    const QPen& getPen() const;
    void setPen(const QPen &newPen);

    typedef QByteArray hash_type;
    enum HashUpdate {Update, DontUpdate};
    hash_type getHash(HashUpdate update = DontUpdate);
    void updateHash();

signals:

public slots:

private:
    QPen pen;
    QByteArray hash;
};

#endif // CURVE_H
