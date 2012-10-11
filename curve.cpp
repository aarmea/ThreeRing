#include "curve.h"

// Default constructor.
Curve::Curve() :
    pen(QBrush(Qt::black), 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
{
}

// Construct a curve with the given QPen
Curve::Curve(QPen newPen) :
    pen(newPen)
{
}

// Copy constructor.
Curve::Curve(const Curve &otherCurve) :
    pen(otherCurve.pen)
{
}

// Get the pixels occupied by the line between the (n-1)th and nth point
// TODO: handle curves wider than 1px
Curve::raster_type Curve::getRasterPoints(size_type n)
{
    Q_ASSERT(n <= size());
    if (size() == 0) return raster_type();
    if (n == 0) return raster_type(1, at(0).toPoint());

    raster_type rasterPoints;
    int x0, y0, x1, y1;
    if (n < size()) {
        x0 = int(at(n-1).x());
        y0 = int(at(n-1).y());
        x1 = int(at(n).x());
        y1 = int(at(n).y());
    } else {
        // n==size() returns the pixels occupied by the line between the first
        // and last points; useful for implicitly closed Curves
        x0 = int(first().x());
        y0 = int(first().y());
        x1 = int(last().x());
        y1 = int(last().y());
    }

    // Bresenham's line drawing algorithm
    int dx = qAbs(x1-x0);
    int dy = qAbs(y1-y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        rasterPoints.push_back(QPoint(x0, y0)); // Add point
        if (x0 == x1 && y0 == y1) break;

        int e2 = err * 2;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }

    return rasterPoints;
}

// Get the pen (style) of the curve.
const QPen &Curve::getPen() const
{
    return pen;
}

// Set the pen (style) of the curve.
void Curve::setPen(const QPen &newPen)
{
    pen = newPen;
}

// Return the hash, creating one if necessary
Curve::hash_type Curve::getHash(HashUpdate update)
{
    if (update == Update || hash.isEmpty()) updateHash();
    return hash;
}

// Update the hash
void Curve::updateHash()
{
    QCryptographicHash hashFunction(QCryptographicHash::Md5);
    for (int i = 0; i < size(); ++i) {
        hashFunction.addData(QString::number(at(i).x()).toUtf8());
        hashFunction.addData(QString::number(at(i).y()).toUtf8());
    }
    hash = hashFunction.result();
}
