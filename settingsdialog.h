#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QObject>
#include <QDialog>
#include <QSettings>
#include <QDir>
#include <QTemporaryFile>
#include <QColorDialog>

namespace Ui {
    class settingsDialog;
}

class GerberStencilGenerator;

class settingsDialog : public QDialog {
    Q_OBJECT
public:
    settingsDialog(GerberStencilGenerator *parent);
    ~settingsDialog();

protected Q_SLOTS:
    void selectNewColor(void);
    void refreshColors(void);

private:
    Ui::settingsDialog *ui;
    GerberStencilGenerator *gsgInstance = nullptr;

friend class GerberStencilGenerator;
};

#endif // SETTINGSDIALOG_H
