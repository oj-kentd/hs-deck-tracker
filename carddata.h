#ifndef CARDDATA_H
#define CARDDATA_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

class CardData
{
private:
    CardData();
    QJsonArray cards;

public:
    static CardData& instance();
    void setCards(const QJsonArray &cardData);
    const QJsonArray &getCards() const;

};

#endif // CARDDATA_H
