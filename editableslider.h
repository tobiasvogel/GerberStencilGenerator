#ifndef EDITABLESLIDER_H
#define EDITABLESLIDER_H
#include <QSlider>
#include <QList>
#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>

class editableSlider : public QSlider {
public:
    editableSlider(QWidget *parent = nullptr);
    ~editableSlider();

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    friend class GerberStencilGenerator;
    friend class sliderSizeLabel;

protected Q_SLOTS:
    void enableControl(bool restoreValue);
    void disableControl();

private:
    int singleStepSize = 1;
    int pageStepSize = 1;

    int _minimum = -1;
    int _maximum = -1;
    int _initvalue = -1;

    int storedValue = -1;
    bool _isEnabled = true;


};


#endif // EDITABLESLIDER_H
