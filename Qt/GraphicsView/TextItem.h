#ifndef TEXTITEM_H
#define TEXTITEM_H

#include "RectItem.h"
#include <QGraphicsItem>

class InternalTextItem;


static const int EditableTextItemType = QGraphicsItem::UserType + 2;
static const int InternalTextItemType = QGraphicsItem::UserType + 10000;

class TextItem : public RectItem {
public:
    TextItem(const QPoint& pos, QGraphicsItem * parent = 0);

#if 0

    virtual void accept(const ItemVisitor& visitor);

    static EditableTextItem* readExternal(QXmlStreamReader& reader);

    virtual void writeExternal(QXmlStreamWriter& writer);
#endif

    virtual void setText(const QString& text);

#if 0
///// Font
    virtual QFont font() const;

    virtual void setFont(const QFont& font);
#endif

    virtual void setInternalFont(const QFont& font);


///// Alignment
//    virtual void setAlignment(Qt::Alignment align);

    virtual void setInternalAlignment(Qt::Alignment align);

    virtual Qt::Alignment getAlignment();

///// Text color
//    virtual void setTextColor(const QColor& col);

    virtual void setInternalDefaultTextColor(const QColor& col);

    virtual QColor getTextColor();
#if 0

    // @Override
    virtual int type () const;
#endif

    void centerTextItem();
#if 0
protected:
    // @Override
    virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
#endif

    virtual void moveHandle(AbstractEditHandle editHandle, const QPointF& scenePos);

private:

    Qt::Alignment alignment;

    InternalTextItem* textItem;

};

#endif
