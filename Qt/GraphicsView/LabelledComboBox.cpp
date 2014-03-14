#include "LabelledComboBox.h"


#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>

LabelledComboBox::LabelledComboBox(QWidget* parent, const QString& labelText) : QWidget(parent) {
    QHBoxLayout* borderSettingLayout = new QHBoxLayout(this);
    QLabel* label = new QLabel(labelText, this);
    comboBox = new QComboBox(this);
    borderSettingLayout->addWidget(label);
    borderSettingLayout->addWidget(comboBox);

    // emit valuechanged signal of spin box as signal of this component
//    connect(spinbox, SIGNAL(valueChanged(double)),
//            this,    SIGNAL(valueChanged(double)));
}
/*
void LabelledDoubleSpinbox::setValue(double value) {
    spinbox->blockSignals(true);
    spinbox->setValue(value);
    spinbox->blockSignals(false);
}

*/
