#ifndef SVGFILEIO_H
#define SVGFILEIO_H

#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QDomDocument>
#include <QSvgGenerator>

#include "noteeditor.h"

void svgOpen(const QString &fileName, NoteEditor *editor);
void svgSave(const QString &fileName, NoteEditor *editor);
void svgParseGroup(const QDomElement &element, NoteEditor *editor,
                   QPen pen = QPen(QBrush(Qt::black), 1.0, Qt::SolidLine,
                                   Qt::RoundCap, Qt::RoundJoin));

#endif // SVGFILEIO_H
