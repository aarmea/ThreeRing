#include "ribbon.h"

Ribbon::Ribbon(QWidget *parent) :
    QWidget(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

QSize Ribbon::sizeHint() const
{
    return QSize(RibbonWidth, RibbonHeight);
}
