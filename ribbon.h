#ifndef RIBBON_H
#define RIBBON_H

#include <QWidget>
#include <QSignalMapper>
#include <QTabWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "noteeditor.h"

class Ribbon : public QWidget
{
    Q_OBJECT
public:
    explicit Ribbon(QWidget *parent = 0, NoteEditor *noteEdit_ = 0);

    QSize sizeHint() const;
    
signals:
    
public slots:
    
private:
    QTabWidget *tabs;
    NoteEditor *noteEdit;

    void initHomeTab();
    QWidget *homeTab;
    QHBoxLayout *homeTabLayout;
    int homeTabId;
    QGroupBox *fileSection;
    QVBoxLayout *fileSectionLayout;
    QPushButton *openButton;
    QPushButton *saveButton;

    void initDrawTab();
    QWidget *drawTab;
    QHBoxLayout *drawTabLayout;
    int drawTabId;
    QGroupBox *penSection;
    QHBoxLayout *penSectionLayout;
    QSignalMapper *penSelectMapper;
    QPushButton *blackPenSelect;
    QPushButton *redPenSelect;
    QPushButton *bluePenSelect;
    QPushButton *greenPenSelect;
};

#endif // RIBBON_H
