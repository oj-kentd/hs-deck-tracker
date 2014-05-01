#include "carddata.h"

CardData::CardData()
{
}

CardData& CardData::instance()
{
    static CardData thisInst;
    return thisInst;
}

void CardData::setCards(const QJsonArray &cardData)
{
    this->cards = cardData;
}

const QJsonArray &CardData::getCards() const
{
    return this->cards;
}
