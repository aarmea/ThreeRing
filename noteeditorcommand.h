#ifndef NOTEEDITORCOMMAND_H
#define NOTEEDITORCOMMAND_H

#include <QDebug>
#include <QLinkedList>
#include <QPointF>
#include "noteeditor.h"
#include "curve.h"

class NoteEditor;

class NoteEditorCommand
{
public:
    enum CommandType {AddCurves, RemoveCurves, MoveCurves};
    NoteEditorCommand(NoteEditor *newNoteEditor = 0,
                      CommandType newType = AddCurves,
                      QLinkedList<Curve> newCurves = QLinkedList<Curve>(),
                      QPointF newDistance = QPointF());
    NoteEditorCommand(const NoteEditorCommand &other);
    NoteEditorCommand &operator=(const NoteEditorCommand &other);

    void execute();
    void unexecute();

private:
    NoteEditor *noteEdit;
    CommandType type;
    QPointF distance;
    QLinkedList<Curve> curves;

    void addCurves();
    void removeCurves();
    enum MoveDirection {PositiveDirection, NegativeDirection};
    void moveCurves(MoveDirection direction = PositiveDirection);
};

#endif // NOTEEDITORCOMMAND_H
