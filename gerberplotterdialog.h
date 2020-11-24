#ifndef GERBERPLOTTERDIALOG_H
#define GERBERPLOTTERDIALOG_H


#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QDir>
#include <QDebug>
#include <QFileDialog>
#include <QString>


enum pcb_color_t { PCB_GREEN, PCB_BLUE, PCB_RED, PCB_YELLOW, PCB_WHITE, PCB_BLACK, PCB_CUSTOM };
typedef enum pcb_color_t pcb_color;

namespace Ui {
class GerberPlotterDialog;
}

class GerberPlotterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GerberPlotterDialog(QWidget *parent = nullptr);
    ~GerberPlotterDialog();

    Q_ENUM(pcb_color);

    const std::string greenPcbSilverPad = "046703;4b8748;fefffa;c4c4c4;50";
    const std::string greenPcbGoldPad = "036702;5a8931;fefffa;ebc888;50";
    const std::string bluePcbSilverPad = "0b01f5;4c4be4;f2ffff;c4c4c4;50";
    const std::string bluePcbGoldPad = "0b01f5;5b4ccf;f2ffff;ebc888;50";
    const std::string redPcbSilverPad = "fc0702;e84c48;fff6f6;c4c4c4;50";
    const std::string redPcbGoldPad = "fc0702;f64d31;fff6f6;ebc888;50";
    const std::string yellowPcbSilverPad = "e0e907;d6dc4b;fffdf9;c4c4c4;50";
    const std::string yellowPcbGoldPad = "e0e907;e4dd35;fffdf9;ebc888;50";
    const std::string whitePcbSilverPad = "ffffff;ececec;050505;c4c4c4;50";
    const std::string whitePcbGoldPad = "ffffff;e9e6e1;050505;ebc888;50";
    const std::string blackPcbSilverPad = "060602;4c4c47;ffffff;c4c4c4;50";
    const std::string blackPcbGoldPad = "060602;5b4d31;ffffff;ebc888;50";
    QString customPcbSilverPad = "000000;999999;ffffff;c4c4c4;50";
    QString customPcbGoldPad = "000000;999999;ffffff;ebc888;50";

private:
    Ui::GerberPlotterDialog *ui;

protected:
    QDir lastDir;
    pcb_color pcbColorScheme;

private Q_SLOTS:
    void browseOpenFile(QLineEdit *lineEdit, QString promptText, QString browseDir, QString fileFilters);
    void browseSaveFile(QLineEdit *lineEdit, QString promptText, QString browseDir, QString fileFilter);
    void colorSchemeChanged(void);
    void imageFormatChanged(void);
    QString getLastDir(void);
};

#endif // GERBERPLOTTERDIALOG_H