#include <QApplication>
#include <QScreen>
#include <QGraphicsScene>
#include <QXmlStreamWriter>
#include <QStyleOptionGraphicsItem>
#include <QTextDocument>
#include <QTextBlock>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <QPainter>
#include <QDebug>

#include "TextItem.h"
#include "GraphicsSheet.h"


class InternalTextItem : public QGraphicsTextItem {
public:
    InternalTextItem(QGraphicsItem * parent) : QGraphicsTextItem(parent) {
        // calculate the scale for the text item - this is the same
        // calculation as in GraphicsSheet::GraphicsSheet, so this should
        // be located in some common convenience method
        QScreen *srn = QApplication::screens().at(0);
        qreal scale = 25.4 / srn->logicalDotsPerInchY();
        setScale(scale);
    }


    void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 ){
        QStyleOptionGraphicsItem option2 = *option;
        option2.state = 0;
        QGraphicsTextItem::paint(painter, &option2, widget);
    }


    // @Override
    virtual void focusOutEvent ( QFocusEvent * event ) {
        // qDebug() << "focusOutEvent";
    	// clear the current selection
    	// See also http://qt-project.org/forums/viewthread/7322
    	QTextCursor t = textCursor();
    	t.clearSelection();
    	setTextCursor(t);

    	QGraphicsTextItem::focusOutEvent(event);
    }


    int type () const {
        return InternalTextItemType;
    }


    // Set the alignment of all blocks to the given alignment
    void setAlignment(Qt::Alignment align) {
        QTextDocument* textDocument = document();
        for (QTextBlock it = textDocument->begin(); it != textDocument->end(); it = it.next()) {
            QTextCursor cur(it);
            QTextBlockFormat format = cur.blockFormat();
            format.setAlignment(align);
            cur.setBlockFormat(format);
        }
    }

};


TextItem::TextItem() : RectItem(), alignment(Qt::AlignHCenter) {
    textItem = new InternalTextItem (this);
    textItem->setTextInteractionFlags(Qt::TextEditorInteraction);

    // This item is focusable and passes the focus on to the child item
    setFlag (QGraphicsItem::ItemIsFocusable);
    setFocusProxy(textItem);

    setFiltersChildEvents(true);
    setInternalAlignment(Qt::AlignHCenter);

    centerTextItem();
}


TextItem::TextItem(const QPointF& pos, QGraphicsItem * parent) :
        RectItem(QRectF(pos.x(), pos.y(), 50, 30), parent), alignment(Qt::AlignHCenter) {

    textItem = new InternalTextItem (this);
    textItem->setTextInteractionFlags(Qt::TextEditorInteraction);

    // This item is focusable and passes the focus on to the child item
    setFlag (QGraphicsItem::ItemIsFocusable);
    setFocusProxy(textItem);

    setFiltersChildEvents(true);
    setInternalAlignment(Qt::AlignHCenter);

    centerTextItem();
}


QGraphicsItem* TextItem::create() {
    return new TextItem();
}


#if 0
void EditableTextItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {

	// paint border if not in preview mode - selection border is drawn by the interactor decorations
    KollageGraphicsScene* theScene = dynamic_cast<KollageGraphicsScene*>(scene());
	if (!isSelected() && theScene->getEditMode() != KollageGraphicsScene::MODE_PREVIEW) {
		painter->setPen(QPen(Qt::gray, 0, Qt::DashLine));
		painter->drawRect(rect());
	}

    // overlay edit markers
    EditableItem::paint(painter, option, widget);
}


void EditableTextItem::accept(const ItemVisitor& visitor) {
    visitor.visit(this);
}


int EditableTextItem::type () const {
    return EditableTextItemType;
}

#endif

QFont TextItem::font() const {
    return textItem->font();
}

void TextItem::setFont(const QFont& font) {
#if 0
    // TODO: Maybe it is better to create the SetFontCommand outside of this class -
    // same for the text color command ...
    KollageGraphicsScene* theScene = dynamic_cast<KollageGraphicsScene*>(scene());
    QUndoCommand* undo = new SetFontCommand(this, font);
    theScene->getUndoStack()->push(undo);
#endif
    setInternalFont(font);
}

void TextItem::setInternalFont(const QFont& font) {
    textItem->setFont(font);
    centerTextItem();
}


void TextItem::setText(const QString& text) {
    textItem->setPlainText(text);
    centerTextItem();
    setInternalAlignment(alignment);
}


QString TextItem::text() {
    return textItem->toPlainText();
}


void TextItem::centerTextItem() {
    // set the text width to the whole EditableTextItem width
	// !!!! http://www.cesarbs.org/blog/2011/05/30/aligning-text-in-qgraphicstextitem
    textItem->setTextWidth(rect().width() / textItem->scale());

    // vertically center the text item
    QRectF textBound = textItem->boundingRect();
    qreal textRectHeight = textBound.height() * textItem->scale();
    QPointF centerPos(0, (rect().height() - textRectHeight) / 2);
    textItem->setPos(centerPos);
}


AbstractEditHandle TextItem::getEditHandle(GraphicsSheet* view, const QPointF& pos, AbstractEditHandle enabledHandles){
    QPointF check = textItem->mapFromScene(pos);
    if (textItem->contains(check)) {
        return CursorHandle;
    }

    return RectItem::getEditHandle(view, pos, enabledHandles);
}


void TextItem::moveHandle(AbstractEditHandle editHandle, const QPointF& scenePos) {
    RectItem::moveHandle(editHandle, scenePos);
    centerTextItem();
}


void TextItem::setCursor(GraphicsSheet* theView, AbstractEditHandle handle) {
    if (handle == CursorHandle) {
        theView->setCursor(Qt::IBeamCursor);
    } else {
        RectItem::setCursor(theView, handle);
    }
}


void TextItem::setInternalAlignment(Qt::Alignment align) {
	// Store alignment
	alignment = align;

    // Set the alignment of all blocks to the given alignment
    QTextDocument* textDocument = textItem->document();
    for (QTextBlock it = textDocument->begin(); it != textDocument->end(); it = it.next()) {
		QTextCursor cur(it);
		QTextBlockFormat format = cur.blockFormat();
		format.setAlignment(align);
		cur.setBlockFormat(format);
    }
}

#if 0
void TextItem::setAlignment(Qt::Alignment align) {
    KollageGraphicsScene* theScene = dynamic_cast<KollageGraphicsScene*>(scene());
    QUndoCommand* undo = new SetAlignmentCommand(this, align);
    theScene->getUndoStack()->push(undo);
}
#endif

Qt::Alignment TextItem::getAlignment() {
    return alignment;
}


void TextItem::setTextColor(const QColor& col) {
    textItem->setDefaultTextColor(col);
/*
    KollageGraphicsScene* theScene = dynamic_cast<KollageGraphicsScene*>(scene());
    QUndoCommand* undo = new TextColorCommand(this, col);
    theScene->getUndoStack()->push(undo);
    */
}


void TextItem::setInternalDefaultTextColor(const QColor& col) {
    textItem->setDefaultTextColor(col);
}


QColor TextItem::getTextColor() {
    return textItem->defaultTextColor();
}

#if 0
// TODO: The generic part (x/y/w/h/rot) should be done in the super class
EditableTextItem* EditableTextItem::readExternal(QXmlStreamReader& reader) {
    EditableTextItem* item = 0;

    QString xpos = reader.attributes().value("xpos").toString();
    QString ypos = reader.attributes().value("ypos").toString();
    QString width = reader.attributes().value("width").toString();
    QString height = reader.attributes().value("height").toString();
    QString rotationAttr = reader.attributes().value("rotation").toString();

    item = new EditableTextItem(QPoint(xpos.toInt(), ypos.toInt()));
    item->setRect(QRect(0, 0, width.toInt(), height.toInt()));

    QPointF center = QPointF(item->rect().width() / 2, item->rect().height() / 2);
    item->setTransformOriginPoint(center);
    item->setRotation(rotationAttr.toInt());

    reader.readNext();  // discard characters for textFrame

    reader.readNext();
    if (reader.name() == "text") {
    	// read font parameters and set the font
        QString fontDesc = reader.attributes().value("font").toString();
        QFont font;
        font.fromString(fontDesc);
        item->setInternalFont(font);

        // read alignment, and check valid values
        int alignDesc = reader.attributes().value("align").toString().toInt();
        switch(alignDesc) {
			case Qt::AlignLeft : break;
			case Qt::AlignHCenter : break;
			case Qt::AlignRight : break;
			default : alignDesc = Qt::AlignLeft; break;
        }

        // read text color and set it
        QString bgColor = reader.attributes().value("textColor").toString();
        item->setInternalDefaultTextColor(QColor(bgColor));

        // read text and set it
        reader.readNext();
        item->setText(reader.text().toString());

        // now set alignment - needs text to be effective!
        item->setInternalAlignment(static_cast<Qt::Alignment>(alignDesc));
    }

    return item;
}
#endif


void TextItem::writeExternal(QXmlStreamWriter& writer) {
    writer.writeStartElement("TextItem");

    writer.writeAttribute("xpos", QString::number(x()));
    writer.writeAttribute("ypos", QString::number(y()));
    writer.writeAttribute("width", QString::number(rect().width()));
    writer.writeAttribute("height", QString::number(rect().height()));
    writer.writeAttribute("rotation", QString::number(rotation()));
    writer.writeAttribute("background-color", brush().color().name());
    writer.writeAttribute("border-color", pen().color().name());
    writer.writeAttribute("border-style", QString::number(pen().style()));
    writer.writeAttribute("border-width", QString::number(pen().widthF()));

    writer.writeStartElement("Text");
    writer.writeAttribute("font", textItem->font().toString());
    writer.writeAttribute("align", QString::number(alignment));
    writer.writeAttribute("textColor", textItem->defaultTextColor().name());
    writer.writeCharacters(textItem->toPlainText());
    writer.writeEndElement();

    writer.writeEndElement();
}


void TextItem::readExternal(QXmlStreamReader& reader) {
    RectItem::readExternal(reader);
    reader.readNext();  // skip characters

    // read the text
    QXmlStreamReader::TokenType type = reader.readNext();
    if (type == QXmlStreamReader::StartElement && reader.name() == "Text") {
        // get the font
        QString fontDesc = reader.attributes().value("font").toString();
        QFont font;
        font.fromString(fontDesc);

        // get the alignment
        alignment = static_cast<Qt::Alignment>(reader.attributes().value("align").toInt());
        switch(alignment) {
            case Qt::AlignLeft : break;
            case Qt::AlignHCenter : break;
            case Qt::AlignRight : break;
            default : alignment = Qt::AlignLeft; break;
        }

        // get the color
        QString color = reader.attributes().value("textColor").toString();

        // read text and set it (before activating the properties, especially the alignment!)
        reader.readNext();
        textItem->setPlainText(reader.text().toString());

        // activate properties
        textItem->setFont(font);
        textItem->setAlignment(alignment);
        textItem->setDefaultTextColor(QColor(color));

        // layout the text
        centerTextItem();
    }
}
