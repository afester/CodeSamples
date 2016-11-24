/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */


#include <QLayout>

#ifdef EXP_SYMBOLS
class Q_DECL_EXPORT ScrollAreaLayout : public QLayout {
#else
class Q_DECL_EXPORT ScrollAreaLayout : public QLayout {
#endif

    QLayoutItem* theItem;

public:
    ScrollAreaLayout();

protected:
    virtual QSize sizeHint() const;

    virtual void addItem(QLayoutItem *item);

    virtual QLayoutItem *itemAt(int index) const;

    virtual QLayoutItem *takeAt(int index);

    virtual int count() const;

    virtual void setGeometry ( const QRect & r );
};
