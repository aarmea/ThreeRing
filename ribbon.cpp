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
    blackPenSelect = new QPushButton(tr("Black"));
    penSectionLayout->addWidget(blackPenSelect);
    redPenSelect = new QPushButton(tr("Red"));
    penSectionLayout->addWidget(redPenSelect);
    bluePenSelect = new QPushButton(tr("Blue"));
    penSectionLayout->addWidget(bluePenSelect);
    greenPenSelect = new QPushButton(tr("Green"));
    penSectionLayout->addWidget(greenPenSelect);
    penSection->setLayout(penSectionLayout);
    drawTabLayout->addWidget(penSection);

    // TODO: fix these connect()s - "No such slot NoteEditor::setPen()"
    /*
    // Method 1: use a QSignalMapper to send multiple signals to the same slot
    penSelectMapper = new QSignalMapper;
    penSelectMapper->setMapping(blackPenSelect,
                                QPen(QBrush(Qt::black), 1.0, Qt::SolidLine,
                                     Qt::RoundCap, Qt::RoundJoin));
    penSelectMapper->setMapping(redPenSelect,
                                QPen(QBrush(Qt::red), 1.0, Qt::SolidLine,
                                     Qt::RoundCap, Qt::RoundJoin));
    penSelectMapper->setMapping(bluePenSelect,
                                QPen(QBrush(Qt::blue), 1.0, Qt::SolidLine,
                                     Qt::RoundCap, Qt::RoundJoin));
    penSelectMapper->setMapping(greenPenSelect,
                                QPen(QBrush(Qt::green), 1.0, Qt::SolidLine,
                                     Qt::RoundCap, Qt::RoundJoin));
    connect(blackPenSelect, SIGNAL(clicked()), penSelectMapper, SLOT(map()));
    connect(redPenSelect, SIGNAL(clicked()), penSelectMapper, SLOT(map()));
    connect(bluePenSelect, SIGNAL(clicked()), penSelectMapper, SLOT(map()));
    connect(greenPenSelect, SIGNAL(clicked()), penSelectMapper, SLOT(map()));
    connect(penSelectMapper, SIGNAL(mapped(QObject*))),
            noteEdit, SLOT(setPen(QObject*));
    */

    /*
    // Method 2: subclass the buttons to send the QPen in the signal
    connect(blackPenSelect, SIGNAL(clicked()), noteEdit, SLOT(setPen(
                QPen(QBrush(Qt::black), 1.0, Qt::SolidLine, Qt::RoundCap,
                     Qt::RoundJoin))));
    connect(redPenSelect, SIGNAL(clicked()), noteEdit, SLOT(setPen(
                QPen(QBrush(Qt::red), 1.0, Qt::SolidLine, Qt::RoundCap,
                     Qt::RoundJoin))));
    connect(bluePenSelect, SIGNAL(clicked()), noteEdit, SLOT(setPen(
                QPen(QBrush(Qt::blue), 1.0, Qt::SolidLine, Qt::RoundCap,
                     Qt::RoundJoin))));
    connect(greenPenSelect, SIGNAL(clicked()), noteEdit, SLOT(setPen(
                QPen(QBrush(Qt::green), 1.0, Qt::SolidLine, Qt::RoundCap,
                     Qt::RoundJoin))));
    */

    drawTab->setLayout(drawTabLayout);
    drawTabId = tabs->addTab(drawTab, tr("&Draw"));
}
