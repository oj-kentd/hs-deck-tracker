#ifndef DECKWIDGET_H
#define DECKWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QJsonObject>

namespace Ui {
class DeckWidget;
}

class DeckWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeckWidget(const QJsonObject &deck, QWidget *parent = 0);
    ~DeckWidget();
    void updateDeck(QJsonObject &deck);

signals:
    void deleteDeck(DeckWidget *widget, QJsonObject &deck);
    void editDeck(DeckWidget *widget, QJsonObject &deck);


private slots:
    void on_pushButtonNewGame_clicked();
    void on_pushButtonEdit_clicked();
    void on_pushButtonDelete_clicked();

private:
    Ui::DeckWidget *ui;
    QJsonObject deck;
};

#endif // DECKWIDGET_H
