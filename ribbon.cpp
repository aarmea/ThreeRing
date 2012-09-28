#include "ribbon.h"

Ribbon::Ribbon(QWidget *parent) :
    QWidget(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // TODO: fix size/margins
    tabs = new QTabWidget(this);

    initFileTab();
    initDrawTab();

    tabs->show();
}

QSize Ribbon::sizeHint() const
{
    return QSize(RibbonWidth, RibbonHeight);
}

void Ribbon::initFileTab()
{
    fileTab = new QWidget;
    fileTabLayout = new QHBoxLayout;

    openButton = new QPushButton(tr("&Open"));
    saveButton = new QPushButton(tr("&Save"));

    fileTabLayout->addWidget(openButton);
    fileTabLayout->addWidget(saveButton);

    connect(openButton, SIGNAL(clicked()), parent(), SLOT(open()));
    connect(saveButton, SIGNAL(clicked()), parent(), SLOT(save()));

    fileTab->setLayout(fileTabLayout);
    fileTabId = tabs->addTab(fileTab, tr("&File"));
}

void Ribbon::initDrawTab()
{
    // TODO: implement!
}
