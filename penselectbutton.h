#ifndef PENSELECTBUTTON_H
#define PENSELECTBUTTON_H

#include <QPen>
#include <QPushButton>

class PenSelectButton : public QPushButton
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
};

#endif // PENSELECTBUTTON_H
