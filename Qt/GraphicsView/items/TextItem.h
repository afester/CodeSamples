#ifndef TEXTITEM_H
#define TEXTITEM_H

#include "RectItem.h"
#include <QGraphicsItem>
#include <QFont>

class InternalTextItem;


static const int EditableTextItemType = QGraphicsItem::UserType + 2;
static const int InternalTextItemType = QGraphicsItem::UserType + 10000;

class TextItem : public RectItem {
    Q_OBJECT;

    Q_PROPERTY(QString Text      READ text         WRITE setText);
    Q_PROPERTY(QFont   Font      READ font         WRITE setFont);
    Q_PROPERTY(QColor  Textcolor READ getTextColor WRITE setTextColor);

public:
    enum EditHandle {NoHandle, MoveHandle,              // TODO: Move in common "super" enum
                     TopLeftHandle, TopHandle, TopRightHandle,
                     LeftHandle, RotationHandle, RightHandle,
                     BottomLeftHandle, BottomHandle, BottomRightHandle,
                     CursorHandle};

    enum EditHandles {MoveHandleMask        = 0x001,
                      TopLeftHandleMask     = 0x002,
                      TopHandleMask         = 0x004,
                      TopRightHandleMask    = 0x008,
                      LeftHandleMask        = 0x010,
                      RotationHandleMask    = 0x020,
                      RightHandleMask       = 0x040,
                      BottomLeftHandleMask  = 0x080,
                      BottomHandleMask      = 0x100,
                      BottomRightHandleMask = 0x200,
                      CursorHandleMask      = 0x400,

                      AllHandlesMask = TopLeftHandleMask | TopHandleMask | TopRightHandleMask |
                                       LeftHandleMask | RotationHandleMask | RightHandleMask |
                                       BottomLeftHandleMask | BottomHandleMask | BottomRightHandleMask|
                                       MoveHandleMask | CursorHandleMask,

                      BasicHandlesMask = TopHandleMask | LeftHandleMask |
                                         RightHandleMask | BottomHandleMask |
                                         MoveHandleMask | CursorHandleMask
                     };

    TextItem();

    TextItem(const QPointF& pos, QGraphicsItem * parent = 0);

    /**
     * Factory function to create a TextItem instance.
     */
    static QGraphicsItem* create();

#if 0

    virtual void accept(const ItemVisitor& visitor);

    static EditableTextItem* readExternal(QXmlStreamReader& reader);

#endif

    // @Override
    virtual void writeExternal(QXmlStreamWriter& writer);

    // @Override
    virtual void readExternal(QXmlStreamReader& reader);

    virtual void setText(const QString& text);

    virtual QString text();

    virtual QFont font() const;

    virtual void setFont(const QFont& font);

    virtual void setInternalFont(const QFont& font);


///// Alignment
//    virtual void setAlignment(Qt::Alignment align);

    virtual void setInternalAlignment(Qt::Alignment align);

    virtual Qt::Alignment getAlignment();

///// Text color
    virtual void setTextColor(const QColor& col);

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

    // @Override
    virtual AbstractEditHandle getEditHandle(GraphicsSheet* view, const QPointF& pos, AbstractEditHandle enabledHandles = AllHandlesMask);

    // @Override
    virtual void moveHandle(AbstractEditHandle editHandle, const QPointF& scenePos);

    // @Override
    virtual void setCursor(GraphicsSheet* theView, AbstractEditHandle handle);

private:

    Qt::Alignment alignment;

    InternalTextItem* textItem;

};

#endif
