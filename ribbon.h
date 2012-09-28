#ifndef RIBBON_H
#define RIBBON_H

#include <QWidget>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QPushButton>

class Ribbon : public QWidget
{
    Q_OBJECT
public:
    explicit Ribbon(QWidget *parent = 0);

    // TODO: determine the optimal width
    enum {RibbonWidth = 400, RibbonHeight = 84};
    QSize sizeHint() const;
    
signals:
    
public slots:
    
private:
    QTabWidget *tabs;

    void initHomeTab();
    QWidget *homeTab;
    QHBoxLayout *homeTabLayout;
    int homeTabId;
    QPushButton *openButton;
    QPushButton *saveButton;

    void initDrawTab();
    QWidget *drawTab;
    int drawTabId;
};

#endif // RIBBON_H
