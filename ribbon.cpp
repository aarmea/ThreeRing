#include "ribbon.h"

Ribbon::Ribbon(QWidget *parent) :
    QWidget(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // TODO: fix size/margins
    tabs = new QTabWidget(this);
    // TODO: somehow set tabs->sizeHint
    tabs->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    initHomeTab();
    initDrawTab();

    tabs->show();
}

QSize Ribbon::sizeHint() const
{
    return QSize(width(), tabs->height());
}

void Ribbon::initHomeTab()
{
    homeTab = new QWidget;
    homeTabLayout = new QVBoxLayout;

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
    drawTab = new QWidget;

    // TODO: populate this tab!

    drawTabId = tabs->addTab(drawTab, tr("&Draw"));
}
