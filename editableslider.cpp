#include "editableslider.h"
#include "gerberstencilgenerator.h"
#include <QDebug>

editableSlider::editableSlider(QWidget *parent) : QSlider(parent) {
    singleStepSize = this->singleStep();
    pageStepSize = this->pageStep();
    _minimum = this->minimum();
    _maximum = this->maximum();
    _initvalue = this->value();
}

editableSlider::~editableSlider() {}

void editableSlider::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Shift) {
        this->setSingleStep(pageStepSize);
        this->update();
        qDebug()<<this->singleStep();
    }
    event->accept();
}

void editableSlider::keyReleaseEvent(QKeyEvent *event) {
    event->accept();
    if (event->key() == Qt::Key_Shift) {
        this->setSingleStep(singleStepSize);
    }
}

void editableSlider::enableControl(bool restoreValue = false) {
    this->setEnabled(true);
    _isEnabled = true;
    if (restoreValue && storedValue > -1) {
           this->setValue(storedValue);
    } else if (restoreValue) {
        this->setValue(_initvalue);
    }
}

void editableSlider::disableControl() {
    storedValue = this->value();
    this->setEnabled(false);
    _isEnabled = false;
}

