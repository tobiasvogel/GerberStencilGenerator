#include "gerberplotterdialog.h"
#include "ui_gerberplotterdialog.h"


GerberPlotterDialog::GerberPlotterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GerberPlotterDialog)
{
    ui->setupUi(this);
    lastDir = QDir::home();
    QObject::connect(ui->cancelButton, &QPushButton::clicked, [this]() { close(); });
    //QObject::connect(ui->generateButton, &QPushButton::clicked, [this]() { std::ignore = this; qDebug() << "Not implemented yet."; });
    QObject::connect(ui->boardOutlineBrowseButton, &QPushButton::clicked, [this]() {
       browseOpenFile(ui->boardOutlineLineEdit, tr("Board Outline Gerber File"), nullptr, tr("Board Outline (*.gml);;Generic Gerber File (*.grb);;Any file (*.*)"));
    });
    QObject::connect(ui->drillsBrowseButton, &QPushButton::clicked, [this]() {
       browseOpenFile(ui->drillsLineEdit, tr("Drills Gerber File"), nullptr, tr("Excellon Drills File (*.xln *.txt *.drl *.nc *.exc *.drd);;Generic Gerber File (*.grb);;Any file (*.*)"));
    });
    QObject::connect(ui->topCopperBrowseButton, &QPushButton::clicked, [this]() {
        browseOpenFile(ui->topCopperLineEdit, tr("Top Copper Gerber File"), nullptr, tr("Copper Gerber File (*.gtl *.top *.cmp);;Generic Gerber File (*.grb);;Any file (*.*)"));
    });
    QObject::connect(ui->topSolderMaskBrowseButton, &QPushButton::clicked, [this]() {
        browseOpenFile(ui->topSolderMaskLineEdit, tr("Top Solder Mask Gerber File"), nullptr, tr("Solder Mask Gerber File (*.gts *.tsm *.smt);;Generic Gerber File (*.grb);;Any file (*.*)"));
    });
    QObject::connect(ui->topSilkscreenBrowseButton, &QPushButton::clicked, [this]() {
        browseOpenFile(ui->topSilkscreenLineEdit, tr("Top Silkscreen Gerber File"), nullptr, tr("Silkscreen Gerber File (*.gto *.tsk *.sst);;Generic Gerber File (*.grb);;Any file (*.*)"));
    });
    QObject::connect(ui->bottomCopperBrowseButton, &QPushButton::clicked, [this]() {
        browseOpenFile(ui->bottomCopperLineEdit, tr("Bottom Copper Gerber File"), nullptr, tr("Copper Gerber File (*.gbl *.bot *.sol);;Generic Gerber File (*.grb);;Any file (*.*)"));
    });
    QObject::connect(ui->bottomSolderMaskBrowseButton, &QPushButton::clicked, [this]() {
        browseOpenFile(ui->bottomSolderMaskLineEdit, tr("Bottom Solder Mask Gerber File"), nullptr, tr("Solder Mask Gerber File (*.gbs *.bsm *.smb);;Generic Gerber File (*.grb);;Any file (*.*)"));
    });
    QObject::connect(ui->bottomSilkscreenBrowseButton, &QPushButton::clicked, [this]() {
        browseOpenFile(ui->bottomSilkscreenLineEdit, tr("Bottom Silkscreen Gerber File"), nullptr, tr("Silkscreen Gerber File (*.gbo *.bsk *.ssb);;Generic Gerber File (*.grb);;Any file (*.*)"));
    });
    QObject::connect(ui->topSideBrowseButton, &QPushButton::clicked, [this]() {
        browseSaveFile(ui->topSideLineEdit, tr("Top Side Output Image File"), nullptr, tr("PNG-Graphic (*.png);;JPEG-Image (*.jpg *.jpeg)"));
    });
    QObject::connect(ui->bottomSideBrowseButton, &QPushButton::clicked, [this]() {
        browseSaveFile(ui->bottomSideLineEdit, tr("Bottom Side Output Image File"), nullptr, tr("PNG-Graphic (*.png);;JPEG-Image (*.jpg *.jpeg)"));
    });
    QObject::connect(ui->radioButton_jpeg, &QRadioButton::clicked, [this]() { imageFormatChanged(); });
    QObject::connect(ui->radioButton_png, &QRadioButton::clicked, [this]() { imageFormatChanged(); });
    QObject::connect(ui->radioButton_green, &QRadioButton::clicked, [this]() { colorSchemeChanged(); });
    QObject::connect(ui->radioButton_blue, &QRadioButton::clicked, [this]() { colorSchemeChanged(); });
    QObject::connect(ui->radioButton_red, &QRadioButton::clicked, [this]() { colorSchemeChanged(); });
    QObject::connect(ui->radioButton_yellow, &QRadioButton::clicked, [this]() { colorSchemeChanged(); });
    QObject::connect(ui->radioButton_white, &QRadioButton::clicked, [this]() { colorSchemeChanged(); });
    QObject::connect(ui->radioButton_black, &QRadioButton::clicked, [this]() { colorSchemeChanged(); });
    QObject::connect(ui->radioButton_custom, &QRadioButton::clicked, [this]() { colorSchemeChanged(); });
    QObject::connect(ui->padsColorComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                     [=](int index) { colorSchemeChanged(); });
}

GerberPlotterDialog::~GerberPlotterDialog()
{
    delete ui;
}

void GerberPlotterDialog::browseOpenFile(QLineEdit *lineEdit, QString promptText, QString browseDir, QString fileFilters)
{
    QString dir;
    if (browseDir.isNull() || browseDir.isEmpty()) {
        dir = getLastDir();
    } else {
        dir = browseDir;
    }
    QString fileName = QFileDialog::getOpenFileName(this, promptText, dir, fileFilters);
    if (fileName.isEmpty() || fileName.isNull()) {
        return;
    } else {
        lineEdit->setText(fileName);
        QString filePath = QFileInfo(fileName).absolutePath();
        lastDir.setPath(filePath);
    }
}

void GerberPlotterDialog::browseSaveFile(QLineEdit *lineEdit, QString promptText, QString browseDir, QString fileFilter)
{
    QString dir;
    if (browseDir.isNull() || browseDir.isEmpty()) {
        dir = getLastDir();
    } else {
        dir = browseDir;
    }
    QString fileName = QFileDialog::getOpenFileName(this, promptText, dir, fileFilter);
    if (fileName.isEmpty() || fileName.isNull()) {
        return;
    } else {
        lineEdit->setText(fileName);
        QString filePath = QFileInfo(fileName).absolutePath();
        lastDir.setPath(filePath);
    }
}

void GerberPlotterDialog::colorSchemeChanged()
{
    if (ui->radioButton_green->isChecked()) { pcbColorScheme = PCB_GREEN; }
    else if (ui->radioButton_blue->isChecked()) { pcbColorScheme = PCB_BLUE; }
    else if (ui->radioButton_red->isChecked()) { pcbColorScheme = PCB_RED; }
    else if (ui->radioButton_yellow->isChecked()) { pcbColorScheme = PCB_YELLOW; }
    else if (ui->radioButton_white->isChecked()) { pcbColorScheme = PCB_WHITE; }
    else if (ui->radioButton_black->isChecked()) { pcbColorScheme = PCB_BLACK; }
    else if (ui->radioButton_custom->isChecked()) { pcbColorScheme = PCB_CUSTOM; }
    else { pcbColorScheme = PCB_GREEN; }

    QStringList colors;


    switch (pcbColorScheme) {
    case PCB_BLUE:
        if (ui->padsColorComboBox->currentText().toLower().contains("gold")) {
            colors =  QString::fromStdString(bluePcbGoldPad).split(QLatin1Char(';'));
        } else {
           colors =  QString::fromStdString(bluePcbSilverPad).split(QLatin1Char(';'));
        }
        break;
    case PCB_RED:
        if (ui->padsColorComboBox->currentText().toLower().contains("gold")) {
            colors =  QString::fromStdString(redPcbGoldPad).split(QLatin1Char(';'));
        } else {
           colors =  QString::fromStdString(redPcbSilverPad).split(QLatin1Char(';'));
        }
        break;
    case PCB_YELLOW:
        if (ui->padsColorComboBox->currentText().toLower().contains("gold")) {
            colors =  QString::fromStdString(yellowPcbGoldPad).split(QLatin1Char(';'));
        } else {
           colors =  QString::fromStdString(yellowPcbSilverPad).split(QLatin1Char(';'));
        }
        break;
    case PCB_WHITE:
        if (ui->padsColorComboBox->currentText().toLower().contains("gold")) {
            colors =  QString::fromStdString(whitePcbGoldPad).split(QLatin1Char(';'));
        } else {
           colors =  QString::fromStdString(whitePcbSilverPad).split(QLatin1Char(';'));
        }
        break;
    case PCB_BLACK:
        if (ui->padsColorComboBox->currentText().toLower().contains("gold")) {
            colors =  QString::fromStdString(blackPcbGoldPad).split(QLatin1Char(';'));
        } else {
           colors =  QString::fromStdString(blackPcbSilverPad).split(QLatin1Char(';'));
        }
        break;
    case PCB_CUSTOM:
        if (ui->padsColorComboBox->currentText().toLower().contains("gold")) {
            colors =  QString(customPcbGoldPad).split(QLatin1Char(';'));
        } else {
           colors =  QString(customPcbSilverPad).split(QLatin1Char(';'));
        }
        break;
    case PCB_GREEN:
    default:
        if (ui->padsColorComboBox->currentText().toLower().contains("gold")) {
            colors =  QString::fromStdString(greenPcbGoldPad).split(QLatin1Char(';'));
        } else {
           colors =  QString::fromStdString(greenPcbSilverPad).split(QLatin1Char(';'));
        }
    }

    ui->baseColorLineEdit->setText(QString("#").append(colors.at(0)));
    ui->copperColorLineEdit->setText(QString("#").append(colors.at(1)));
    ui->silkscreenColorLineEdit->setText(QString("#").append(colors.at(2)));
    ui->bumpMapSpinBox->setValue(colors.at(4).toInt());

}

void GerberPlotterDialog::imageFormatChanged()
{
    qDebug() << "Image Format Changed";
}

QString GerberPlotterDialog::getLastDir()
{
    QString lastDirString = lastDir.absolutePath();
    return lastDirString;
}
