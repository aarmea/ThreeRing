#ifndef RIBBON_H
#define RIBBON_H

#include <QWidget>
#include <QSignalMapper>
#include <QTabWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "penselectbutton.h"
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
    QGroupBox *historySection;
    QVBoxLayout *historySectionLayout;
    QPushButton *undoButton;
    QPushButton *redoButton;

    void initDrawTab();
    QWidget *drawTab;
    QHBoxLayout *drawTabLayout;
    int drawTabId;
    QGroupBox *penSection;
    QHBoxLayout *penSectionLayout;
    QSignalMapper *penSelectMapper;
    PenSelectButton *blackPenSelect;
    PenSelectButton *redPenSelect;
    PenSelectButton *bluePenSelect;
    PenSelectButton *greenPenSelect;
};

#endif // RIBBON_H
