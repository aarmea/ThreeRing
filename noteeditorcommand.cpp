#include "noteeditorcommand.h"

NoteEditorCommand::NoteEditorCommand(NoteEditor *newNoteEditor,
                                     NoteEditorCommand::CommandType newType,
                                     QLinkedList<Curve> newCurves,
                                     QPointF newDistance) :
    noteEdit(newNoteEditor), type(newType), curves(newCurves),
    distance(newDistance)
{
}

// Copy constructor.
NoteEditorCommand::NoteEditorCommand(const NoteEditorCommand &other) :
    noteEdit(other.noteEdit), type(other.type), curves(other.curves),
    distance(other.distance)
{
}

// Assignment.
NoteEditorCommand &NoteEditorCommand::operator=(const NoteEditorCommand &other)
{
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
    if (curves.isEmpty())
        qDebug() << "WARN: Attempted to unexecute an empty Command";
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
    // TODO: implement!
}

void NoteEditorCommand::removeCurves()
{
    QLinkedList<Curve>::iterator itr;
    for (itr = curves.begin(); itr != curves.end(); ++itr) {
        qDebug() << "Remove Curve with hash: " << itr->getHash(Curve::Update);
        qDebug() << "    with first point:" << itr->first();
        noteEdit->eraseCurve(itr->getHash());
    }
}

void NoteEditorCommand::moveCurves(MoveDirection direction)
{
    // TODO: implement!
}
