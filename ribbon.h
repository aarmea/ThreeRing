#ifndef RIBBON_H
#define RIBBON_H

#include <QWidget>
#include <QTabWidget>

class Ribbon : public QWidget
{
    Q_OBJECT
public:
    explicit Ribbon(QWidget *parent = 0);
    // TODO: determine the optimal width
    enum {RibbonWidth = 400, RibbonHeight = 100};

    QSize sizeHint() const;
    
signals:
    
public slots:
    
};

#endif // RIBBON_H
