#include "ribbon.h"

Ribbon::Ribbon(QWidget *parent, NoteEditor *noteEdit_) :
    QWidget(parent), noteEdit(noteEdit_)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    tabs = new QTabWidget(this);
    // TODO: somehow set tabs->sizeHint
    tabs->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    initHomeTab();
    if (noteEdit) initDrawTab();

    tabs->show();
}

QSize Ribbon::sizeHint() const
{
    return QSize(width(), tabs->height());
}

void Ribbon::initHomeTab()
{
    homeTab = new QWidget;
    homeTabLayout = new QHBoxLayout;

    fileSection = new QGroupBox(tr("&File"));
    fileSectionLayout = new QVBoxLayout;
    openButton = new QPushButton(tr("&Open"));
    fileSectionLayout->addWidget(openButton);
    saveButton = new QPushButton(tr("&Save"));
    fileSectionLayout->addWidget(saveButton);
    fileSection->setLayout(fileSectionLayout);
    homeTabLayout->addWidget(fileSection);

    connect(openButton, SIGNAL(clicked()), parent(), SLOT(open()));
    connect(saveButton, SIGNAL(clicked()), parent(), SLOT(save()));

    homeTab->setLayout(homeTabLayout);
    homeTabId = tabs->addTab(homeTab, tr("&Home"));
}

void Ribbon::initDrawTab()
{
    drawTab = new QWidget;
    drawTabLayout = new QHBoxLayout;

    penSection = new QGroupBox(tr("&Pens"));
    penSectionLayout = new QHBoxLayout;
    // TODO: generate default (and custom) pens from a settings file
    blackPenSelect = new PenSelectButton(Qt::black, 1.0);
    penSectionLayout->addWidget(blackPenSelect);
    redPenSelect = new PenSelectButton(Qt::red, 1.0);
    penSectionLayout->addWidget(redPenSelect);
    bluePenSelect = new PenSelectButton(Qt::blue, 1.0);
    penSectionLayout->addWidget(bluePenSelect);
    greenPenSelect = new PenSelectButton(Qt::green, 1.0);
    penSectionLayout->addWidget(greenPenSelect);
    penSection->setLayout(penSectionLayout);
    drawTabLayout->addWidget(penSection);

    connect(blackPenSelect, SIGNAL(clicked(QPen)), noteEdit, SLOT(setPen(QPen)));
    connect(redPenSelect, SIGNAL(clicked(QPen)), noteEdit, SLOT(setPen(QPen)));
    connect(bluePenSelect, SIGNAL(clicked(QPen)), noteEdit, SLOT(setPen(QPen)));
    connect(greenPenSelect, SIGNAL(clicked(QPen)), noteEdit, SLOT(setPen(QPen)));

    drawTab->setLayout(drawTabLayout);
    drawTabId = tabs->addTab(drawTab, tr("&Draw"));
}
