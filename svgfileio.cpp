#include "svgfileio.h"

// Open a file at fileName into the editor
void svgOpen(const QString &fileName, NoteEditor *editor)
{
    QDomDocument inkSvg("ink");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) return;
    if (!inkSvg.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    // Convert XML data in inkSvg to editor->drawing
    QDomElement root = inkSvg.documentElement();
    QDomElement child = root.firstChildElement("g");
    while (!child.isNull()) {
        svgParseGroup(child, editor);
        child = child.nextSiblingElement("g");
    }
    editor->update();
}

// Save the file in the editor to a file at fileName
void svgSave(const QString &fileName, NoteEditor *editor)
{
    QSvgGenerator generator;
    generator.setFileName(fileName);
    generator.setSize(editor->size());
    generator.setViewBox(QRect(0, 0, editor->width(), editor->height()));
    generator.setTitle("Ink");
    generator.setDescription("ThreeRing ink");

    QPainter painter;
    painter.begin(&generator);
    editor->paint(painter);
    painter.end();
}

// Parse a <g> tag while opening a file
void svgParseGroup(const QDomElement &element, NoteEditor *editor, QPen pen)
{
    if (element.tagName() != "g") return;

    // Convert the <g> tag into a QPen
    if (element.hasAttribute("stroke-width"))
        pen.setWidthF(element.attribute("stroke-width").toDouble());
    if (element.hasAttribute("stroke")) {
        // Colors by hex value with '#' prefix
        if (element.attribute("stroke").at(0) == '#') {
            pen.setColor(QColor(element.attribute("stroke")
                                .right(6).toInt(0, 16)));
        }
        // TODO: handle colors by name
    }

    if (element.hasAttribute("stroke-linecap")) {
        QString linecap = element.attribute("stroke-linecap");
        if (linecap == "square") {
            pen.setCapStyle(Qt::SquareCap);
        } else if (linecap == "round") {
            pen.setCapStyle(Qt::RoundCap);
        } else if (linecap == "butt") {
            pen.setCapStyle(Qt::FlatCap);
        }
    }

    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == "g") {
            svgParseGroup(child, editor, pen);
        } else if (child.tagName() == "polyline") {
            // Get curve from points, add to editor
            drawing_type::iterator curve = editor->getNewCurve();
            curve->setPen(pen);
            QStringList pointStrs = child.attribute("points")
                    .split(' ', QString::SkipEmptyParts);
            QStringList::iterator itr;
            for (itr = pointStrs.begin(); itr != pointStrs.end(); ++itr) {
                QStringList curPointStr =
                        itr->split(',', QString::SkipEmptyParts);
                QPointF point(curPointStr[0].toDouble(),
                              curPointStr[1].toDouble());
                editor->addPointToCurve(point, curve);
            }
        }
        child = child.nextSiblingElement();
    }
}
