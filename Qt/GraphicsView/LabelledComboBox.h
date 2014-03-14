#ifndef __LABELLED_COMBO_BOX_H__
#define __LABELLED_COMBO_BOX_H__

#include <QWidget>

class QComboBox;

class LabelledComboBox : public QWidget {
	Q_OBJECT

	QComboBox* comboBox;

public:
	LabelledComboBox(QWidget* parent, const QString& labelText);

	inline QComboBox* getComboBox() { return comboBox; }
};

#endif
