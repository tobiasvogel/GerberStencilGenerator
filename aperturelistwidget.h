#ifndef APERTURELISTWIDGET_H
#define APERTURELISTWIDGET_H

#include <QListView>
#include <QObject>
#include <QAbstractItemModel>
#include <QStandardItemModel>

class GerberStencilGenerator;

class apertureListWidget : public QListView
{
    Q_OBJECT
public:
    apertureListWidget(QWidget *parent);
    ~apertureListWidget();

Q_SIGNALS:
    void currentTextChanged(const QString &text);


protected:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);
    int getCurrentIndex(void) const;
    int getPreviousIndex(void) const;
    QModelIndex getCurrentModelIndex(void) const;
    QModelIndex getPreviousModelIndex(void) const;
    QStandardItemModel *getItemModel(void);
    QString getCurrentItemText(void);
    QString getItemTextAt(int row);

private Q_SLOTS:
    void changedText(QString text);

private:
    int _currentIndex = -1;
    int _previousIndex = -1;
    QModelIndex _currentModelIndex = QModelIndex();
    QModelIndex _previousModelIndex = QModelIndex();

    GerberStencilGenerator *gsgInstance = nullptr;

    friend class GerberStencilGenerator;
};

#endif // APERTURELISTWIDGET_H
