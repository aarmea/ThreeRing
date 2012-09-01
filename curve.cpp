#include "curve.h"

// Default constructor.
Curve::Curve(QObject *parent) :
    QObject(parent),
    pen(QBrush(Qt::black), 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
{
}

// Copy constructor.
Curve::Curve(const Curve &otherCurve) :
    pen(otherCurve.pen),
    points(otherCurve.points)
{
}

// Adds a point to the end of a curve.
void Curve::addPoint(const QPointF &point)
{
    points.push_back(point);
}

// Removes all points from a curve.
void Curve::clear()
{
    points.clear();
}

// Get the nth point in the curve.
QPointF Curve::getPoint(curve_size_type n)
{
    return points[n];
}

// Get a reference to the first point in the curve as a C array.
QPointF* Curve::getPoints()
{
    return points.data();
}

// Get the pixels occupied by the line between the (n-1)th and nth point
// TODO: handle curves wider than 1px
curve_raster_type Curve::getRasterPoints(curve_size_type n)
{
    Q_ASSERT(n < points.size());
    if (points.size() == 0) return curve_raster_type();
    if (n == 0) return curve_raster_type(1, points[0].toPoint());

    curve_raster_type rasterPoints;
    int x0 = int(points[n-1].x());
    int y0 = int(points[n-1].y());
    int x1 = int(points[n].x());
    int y1 = int(points[n].y());

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

// Get the number of points in the curve.
curve_size_type Curve::size() const
{
    return points.size();
}

// Get the pen (style) of the curve.
const QPen& Curve::getPen() const
{
    return pen;
}

// Set the pen (style) of the curve.
void Curve::setPen(const QPen &newPen)
{
    pen = newPen;
}
