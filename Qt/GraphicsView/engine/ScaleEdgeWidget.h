/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include <QWidget>

#ifdef EXP_SYMBOLS
class Q_DECL_EXPORT ScaleEdgeWidget : public QWidget {
#else
class Q_DECL_IMPORT ScaleEdgeWidget : public QWidget {
#endif

    QString unit;

public:
    ScaleEdgeWidget(QWidget* parent);

    void setUnit(const QString& theUnit);

protected:
    void paintEvent ( QPaintEvent * event );
};
