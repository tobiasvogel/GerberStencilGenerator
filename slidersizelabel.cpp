#include <QDebug>
#include <QInputDialog>
#include "slidersizelabel.h"
#include "gerberstencilgenerator.h"

sliderSizeLabel::sliderSizeLabel(QWidget *parent) {
    std::ignore = parent;
    setStyleSheet("* { padding-right: 4px; padding-left: 4px; } :hover { background-color: #ddd; }");
}

sliderSizeLabel::~sliderSizeLabel() {}

void sliderSizeLabel::mouseDoubleClickEvent(QMouseEvent *event) {
    if (!_isEnabled) {
        event->ignore();
        return;
    }
    bool ok;
    QString childName = this->objectName();
    childName.replace("Label", "Slider");
    snapSlider *slider = this->parent()->findChild<snapSlider *>(childName);
    int minimumValue = slider->minimum();
    int currentValue = slider->value();
    int maximumValue = slider->maximum();
    int stepSize = slider->singleStep();
    int inputPercentage = QInputDialog::getInt(this, tr("Percentage"), tr("Percentage"), currentValue, minimumValue, maximumValue, stepSize,&ok);

    if (ok) {
        slider->setValue(inputPercentage);
        slider->valueChanged(inputPercentage);
    }

}

void sliderSizeLabel::disableControl() {
    _isEnabled = false;
    this->setEnabled(false);
    this->setText(notAvailableAbbreviation);
}

void sliderSizeLabel::enableControl() {
    this->setEnabled(true);
    _isEnabled = true;
    QString childName = this->objectName();
    childName.replace("Label", "Slider");
    snapSlider *slider = this->parent()->findChild<snapSlider *>(childName);
    this->setText(QString("%1%").arg(slider->value()));
}

void sliderSizeLabel::enable(bool toggle)
{
    if (toggle) {
        this->enableControl();
    } else {
        this->disableControl();
    }
}

bool sliderSizeLabel::getEnabled()
{
    return _isEnabled;
}
