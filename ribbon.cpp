#include "ribbon.h"

Ribbon::Ribbon(QWidget *parent) :
    QWidget(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // TODO: fix size/margins
    tabs = new QTabWidget(this);

    initHomeTab();
    initDrawTab();

    tabs->show();
}

QSize Ribbon::sizeHint() const
{
    return QSize(RibbonWidth, RibbonHeight);
}

void Ribbon::initHomeTab()
{
    homeTab = new QWidget;
    homeTabLayout = new QHBoxLayout;

    openButton = new QPushButton(tr("&Open"));
    saveButton = new QPushButton(tr("&Save"));

    homeTabLayout->addWidget(openButton);
    homeTabLayout->addWidget(saveButton);

    connect(openButton, SIGNAL(clicked()), parent(), SLOT(open()));
    connect(saveButton, SIGNAL(clicked()), parent(), SLOT(save()));

    homeTab->setLayout(homeTabLayout);
    homeTabId = tabs->addTab(homeTab, tr("&Home"));
}

void Ribbon::initDrawTab()
{
    // TODO: implement!
}
