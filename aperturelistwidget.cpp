#include "aperturelistwidget.h"
#include "gerberstencilgenerator.h"
#include <QApplication>
#include <QDebug>


apertureListWidget::apertureListWidget(QWidget *parent) : QListView(parent)
{
    QWidgetList wList = qApp->allWidgets();
    for (int i = 0; i < wList.length(); i++) {
        if (wList.at(i)->objectName() == "GerberStencilGenerator") {
        gsgInstance = dynamic_cast<GerberStencilGenerator*>(wList.at(i));
        }
    }
}

apertureListWidget::~apertureListWidget() {
    gsgInstance = nullptr;
}

void apertureListWidget::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    qDebug() << "Current: " << current.row();
    qDebug() << "Previous: " << previous.row();
    _currentIndex = current.row();
    _currentModelIndex = current;
    _previousIndex = previous.row();
    _previousModelIndex = previous;
    if (current.row() > -1) {
    changedText(current.data().toString());
    }
    if (gsgInstance->autoPreview) {
        gsgInstance->generatePreview();
    }
}

int apertureListWidget::getCurrentIndex() const {
    return _currentIndex;
}

int apertureListWidget::getPreviousIndex() const {
    return _previousIndex;
}

QModelIndex apertureListWidget::getCurrentModelIndex() const {
    return _currentModelIndex;
}

QModelIndex apertureListWidget::getPreviousModelIndex() const {
    return _previousModelIndex;
}

QStandardItemModel *apertureListWidget::getItemModel()
{
    QStandardItemModel *model = gsgInstance->getWidgetListModel();
    return model;
}

QString apertureListWidget::getCurrentItemText() {
    if (getCurrentIndex() < 0) {
        return QString();
    }
    QStandardItem *currentItem = this->getItemModel()->item(getCurrentIndex());
    return currentItem->text();
}

QString apertureListWidget::getItemTextAt(int row) {
    if (row > this->getItemModel()->rowCount()) {
        return QString();
    }
    QStandardItem *itemAt = this->getItemModel()->item(row);
    return itemAt->text();
}

void apertureListWidget::changedText(QString text) {
    Q_EMIT currentTextChanged(text);
}

