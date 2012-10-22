#include "noteeditorcommand.h"

NoteEditorCommand::NoteEditorCommand(NoteEditor *newNoteEditor,
                                     NoteEditorCommand::CommandType newType,
                                     const QLinkedList<Curve> &newCurves,
                                     QPointF newDistance) :
    noteEdit(newNoteEditor), type(newType), curves(newCurves),
    distance(newDistance)
{
    if (!noteEdit) qDebug() << "WARN: Default NoteEditorCommand created";
    if (newCurves.size() == 0) qDebug() << "WARN: Empty NoteEditorCommand created";
    qDebug() << "NoteEditorCommand: Constructor called";
}

NoteEditorCommand::NoteEditorCommand(NoteEditor *newNoteEditor,
                                     CommandType newType,
                                     const selection_type &newCurves,
                                     QPointF newDistance) :
    noteEdit(newNoteEditor), type(newType), distance(newDistance)
{
    qDebug() << "NoteEditorCommand: Constructor using QMap called";
    selection_type::const_iterator itr;
    for (itr = newCurves.begin(); itr != newCurves.end(); ++itr) {
        curves.push_back(*itr.key());
    }
}

// Copy constructor.
NoteEditorCommand::NoteEditorCommand(const NoteEditorCommand &other) :
    noteEdit(other.noteEdit), type(other.type), curves(other.curves),
    distance(other.distance)
{
    qDebug() << "NoteEditorCommand: Copy constructor called";
}

// Assignment.
NoteEditorCommand &NoteEditorCommand::operator=(const NoteEditorCommand &other)
{
    qDebug() << "NoteEditorCommand: Assignment operator called";
    noteEdit = other.noteEdit;
    type = other.type;
    curves = other.curves;
    distance = other.distance;
    return *this;
}

void NoteEditorCommand::execute()
{
    switch (type) {
    case AddCurves:
        addCurves();
        break;
    case RemoveCurves:
        removeCurves();
        break;
    case MoveCurves:
        moveCurves();
        break;
    default:
        break;
    }
}

void NoteEditorCommand::unexecute()
{
    switch (type) {
    case AddCurves:
        removeCurves();
        break;
    case RemoveCurves:
        addCurves();
        break;
    case MoveCurves:
        moveCurves(NegativeDirection);
        break;
    default:
        break;
    }
}

void NoteEditorCommand::addCurves()
{
    QLinkedList<Curve>::iterator itr;
    for (itr = curves.begin(); itr != curves.end(); ++itr) {
        qDebug() << "Add Curve with hash:" << itr->getHash();
        qDebug() << "    with first point:" << itr->first();
        Curve *newCurve = new Curve(*itr);
        noteEdit->addCurve(newCurve);
        noteEdit->addBackpointers(newCurve);
        noteEdit->updateAround(newCurve->boundingRect().toAlignedRect());
    }
}

void NoteEditorCommand::removeCurves()
{
    QLinkedList<Curve>::iterator itr;
    for (itr = curves.begin(); itr != curves.end(); ++itr) {
        qDebug() << "Remove Curve with hash:" << itr->getHash();
        qDebug() << "    with first point: " << itr->first();
        noteEdit->eraseCurve(itr->getHash());
        // Redrawing is implicitly handled by eraseCurve
    }
}

void NoteEditorCommand::moveCurves(MoveDirection direction)
{
    // TODO: implement!
}
