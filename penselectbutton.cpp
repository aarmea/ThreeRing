#include "penselectbutton.h"

const QSize PenSelectButton::iconSize = QSize(32, 16);
const int PenSelectButton::iconPointsSize = 60;
const QPointF PenSelectButton::iconPoints[] = {
    QPointF(0,16),
    QPointF(0.577769733,15.7958532),
    QPointF(0.707878572,15.7958532),
    QPointF(0.835780072,15.7958532),
    QPointF(1.091588716,15.74480523),
    QPointF(1.413549805,15.69375727),
    QPointF(1.799461643,15.53890126),
    QPointF(2.377231377,15.38575737),
    QPointF(3.149060699,15.23090136),
    QPointF(3.984852061,14.9757066),
    QPointF(4.884554656,14.71870961),
    QPointF(5.848275745,14.41070971),
    QPointF(6.811940381,14.10270981),
    QPointF(7.775605016,13.69265904),
    QPointF(8.739326105,13.2315603),
    QPointF(9.702990741,12.71936855),
    QPointF(10.53873129,12.00131788),
    QPointF(11.30836457,11.28322215),
    QPointF(11.95233191,10.51412351),
    QPointF(12.46611702,9.641171783),
    QPointF(12.97995858,8.821025183),
    QPointF(13.36587607,7.949830619),
    QPointF(13.68783151,7.025830923),
    QPointF(13.94362322,6.205684324),
    QPointF(14.00758526,5.385537724),
    QPointF(14.00758526,4.667441998),
    QPointF(14.00758526,4.051487256),
    QPointF(13.68783151,3.488292591),
    QPointF(13.30191403,3.025391631),
    QPointF(12.8520345,2.7191489),
    QPointF(12.46611702,2.513244931),
    QPointF(12.14416158,2.462196966),
    QPointF(11.75824409,2.462196966),
    QPointF(11.3745283,2.615340859),
    QPointF(10.98861082,3.025391631),
    QPointF(10.66665538,3.590343464),
    QPointF(10.41086367,4.257391226),
    QPointF(10.28073789,4.924438988),
    QPointF(10.28073789,5.693537623),
    QPointF(10.28073789,6.462636258),
    QPointF(10.66665538,7.231734893),
    QPointF(11.24440253,7.949830619),
    QPointF(11.95233191,8.616878381),
    QPointF(12.91599654,9.231121011),
    QPointF(13.87966118,9.692219747),
    QPointF(14.9712499,9.949171681),
    QPointF(16.12900235,10.05126761),
    QPointF(17.2845531,10.05126761),
    QPointF(18.57017318,9.796072844),
    QPointF(19.9197553,9.129025082),
    QPointF(21.33329946,8.205025386),
    QPointF(22.93872974,7.076878888),
    QPointF(24.54636172,5.846636461),
    QPointF(26.28186132,4.565391125),
    QPointF(27.88723515,3.384439495),
    QPointF(29.23681727,2.309098128),
    QPointF(30.39456972,1.436146397),
    QPointF(31.22816503,0.769098635),
    QPointF(31.67804456,0.307999899),
    QPointF(32,0)
};

PenSelectButton::PenSelectButton(QBrush brush, qreal width, QWidget *parent) :
    pen_(brush, width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin),
    QToolButton(parent)
{
    init();
}

PenSelectButton::PenSelectButton(const QPen &newPen, QWidget *parent) :
    pen_(newPen), QToolButton(parent)
{
    init();
}

void PenSelectButton::init()
{
    // Generate the icon
    setIconSize(iconSize);
    QPixmap pixmap(iconSize);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(pen_);
    painter.drawPolyline(iconPoints, iconPointsSize);
    setIcon(pixmap);

    // Set up the click action
    connect(this, SIGNAL(clicked()), this, SLOT(reemitClicked()));
}

void PenSelectButton::reemitClicked()
{
    emit clicked(pen_);
}
