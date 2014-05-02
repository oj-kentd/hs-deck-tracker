#ifndef ACTIVEDECK_H
#define ACTIVEDECK_H

#include <QDialog>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include "cardwidget.h"
#include <QButtonGroup>

namespace Ui {
class ActiveDeck;
}

class ActiveDeck : public QDialog
{
    Q_OBJECT

public:
    explicit ActiveDeck(const QJsonObject &deck, QWidget *parent = 0);
    ~ActiveDeck();

private:
    Ui::ActiveDeck *ui;
    QJsonObject deck;
    void loadCards();
    QMap<QString, CardWidget *> cards;
    QButtonGroup sortingButtons;

private slots:
    void calculateStats();
    void resetDeck();
    void sortCards();
    void updateCardLabel(const QImage&);
};

#endif // ACTIVEDECK_H
