#include "penselectbutton.h"

PenSelectButton::PenSelectButton(QBrush brush, qreal width, QWidget *parent) :
    pen_(brush, width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin),
    QToolButton(parent)
{
    connect(this, SIGNAL(clicked()), this, SLOT(reemitClicked()));
}

PenSelectButton::PenSelectButton(const QPen &newPen, QWidget *parent) :
    pen_(newPen), QToolButton(parent)
{
    connect(this, SIGNAL(clicked()), this, SLOT(reemitClicked()));
}

void PenSelectButton::reemitClicked()
{
    emit clicked(pen_);
}

// TODO: reimplement drawing so that it displays the pen
// TODO: set the size of the button to a fixed size
