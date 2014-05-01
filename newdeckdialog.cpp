#include "newdeckdialog.h"
#include "ui_newdeckdialog.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QMap>
#include <QProgressDialog>
#include <QTimer>
#include "carddata.h"

NewDeckDialog::NewDeckDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDeckDialog),
    cardData(CardData::instance().getCards())
{
    ui->setupUi(this);
    initUi();
}

NewDeckDialog::NewDeckDialog(QJsonObject &deck, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDeckDialog),
    cardData(CardData::instance().getCards()),
    deck(deck)
{
    ui->setupUi(this);
    initUi();
    loadDeck();
}

void NewDeckDialog::initUi(void)
{
    this->arenaMode = false;
    ui->comboBoxHeroClass->blockSignals(true);
    ui->comboBoxHeroClass->insertItem(0, "", 0);
    ui->comboBoxHeroClass->insertItem(1, "Warrior", 1);
    ui->comboBoxHeroClass->insertItem(2, "Paladin", 2);
    ui->comboBoxHeroClass->insertItem(3, "Hunter", 3);
    ui->comboBoxHeroClass->insertItem(4, "Rogue", 4);
    ui->comboBoxHeroClass->insertItem(5, "Priest", 5);
    ui->comboBoxHeroClass->insertItem(7, "Shaman", 7);
    ui->comboBoxHeroClass->insertItem(8, "Mage", 8);
    ui->comboBoxHeroClass->insertItem(9, "Warlock", 9);
    ui->comboBoxHeroClass->insertItem(11, "Druid", 11);
    ui->comboBoxHeroClass->blockSignals(false);

    QString defaultStr = QString(  // Default Cards
        "QPushButton {"
            "border: 1px solid;"
            "height: 20px;"
            "border-radius: 4px;"
            "background-color: #919191;}"
        "QPushButton:hover {"
            "background-color: #fff;"
            "color: #000;}"
        "QPushButton:pressed {background-color: #919191;}");

    QString warriorStr = QString(  // Warrior
        "QPushButton {"
            "border: 1px solid;"
            "height: 20px;"
            "border-radius: 4px;"
            "background-color: #cc9966;"
            "color: #000;}"
        "QPushButton:hover {"
            "border: 2px solid #cc9966;"
            "background-color: #fff;"
            "color: #000;}"
        "QPushButton:pressed {background-color: #cc9966;}");

    QString paladinStr = QString(  // Paladin
        "QPushButton {"
            "border: 1px solid;"
            "height: 20px;"
            "border-radius: 4px;"
            "background-color: #ff9999;"
            "color: #000;}"
        "QPushButton:hover {"
            "border: 2px solid #ff9999;"
            "background-color: #fff;"
            "color: #000;}"
        "QPushButton:pressed {background-color: #ff9999;}");

    QString hunterStr = QString(  // Hunter
        "QPushButton {"
            "border: 1px solid;"
            "height: 20px;"
            "border-radius: 4px;"
            "background-color: #aad35a;"
            "color: #000;}"
        "QPushButton:hover {"
            "border: 2px solid #aad35a;"
            "background-color: #fff;"
            "color: #000;}"
        "QPushButton:pressed {background-color: #aad35a;}");

    QString rogueStr = QString(  // Rogue
        "QPushButton {"
            "border: 1px solid;"
            "height: 20px;"
            "border-radius: 4px;"
            "background-color: #ffff33;"
            "color: #000;}"
        "QPushButton:hover {"
            "border: 2px solid #ffff33;"
            "background-color: #fff;"
            "color: #000;}"
        "QPushButton:pressed {background-color: #ffff33;}");

    QString priestStr = QString(  // Priest
        "QPushButton {"
            "border: 1px solid;"
            "height: 20px;"
            "border-radius: 4px;"
            "background-color: #fff;"
            "color: #000;}"
        "QPushButton:hover {"
            "border: 2px solid #fff;"
            "background-color: #000;"
            "color: #fff;}"
        "QPushButton:pressed {background-color: #fff;}");

    QString shamanStr = QString(  // Shaman
        "QPushButton {"
            "border: 1px solid;"
            "height: 20px;"
            "border-radius: 4px;"
            "background-color: #2459f1;"
            "color: #000;}"
        "QPushButton:hover {"
            "border: 2px solid #2459f1;"
            "background-color: #fff;"
            "color: #000;}"
        "QPushButton:pressed {background-color: #2459f1;}");

    QString mageStr = QString(  // Mage
        "QPushButton {"
            "border: 1px solid;"
            "height: 20px;"
            "border-radius: 4px;"
            "background-color: #5eccef;"
            "color: #000;}"
        "QPushButton:hover {"
            "border: 2px solid #5eccef;"
            "background-color: #fff;"
            "color: #000;}"
        "QPushButton:pressed {background-color: #5eccef;}");

    QString warlockStr = QString(  // Warlock
        "QPushButton {"
            "border: 1px solid;"
            "height: 20px;"
            "border-radius: 4px;"
            "background-color: #9382c9;"
            "color: #000;}"
        "QPushButton:hover {"
            "border: 2px solid #9382c9;"
            "background-color: #fff;"
            "color: #000;}"
        "QPushButton:pressed {background-color: #9382c9;}");

    QString druidStr = QString(  // Druid
        "QPushButton {"
            "border: 1px solid;"
            "height: 20px;"
            "border-radius: 4px;"
            "background-color: #ff6600;"
            "color: #000;}"
        "QPushButton:hover {"
            "border: 2px solid #ff6600;"
            "background-color: #fff;"
            "color: #000;}"
        "QPushButton:pressed {background-color: #ff6600;}");

    styleSheets.insert(0, defaultStr);
    styleSheets.insert(1, warriorStr);
    styleSheets.insert(2, paladinStr);
    styleSheets.insert(3, hunterStr);
    styleSheets.insert(4, rogueStr);
    styleSheets.insert(5, priestStr);
    styleSheets.insert(6, "");
    styleSheets.insert(7, shamanStr);
    styleSheets.insert(8, mageStr);
    styleSheets.insert(9, warlockStr);
    styleSheets.insert(10, "");
    styleSheets.insert(11, druidStr);

    ui->listWidgetCards->setUniformItemSizes(true);
    ui->listWidgetCards->setSortingEnabled(false);
    ui->listWidgetDeck->setUniformItemSizes(true);
    ui->listWidgetDeck->setSortingEnabled(false);

    sortButtons.addButton(ui->pushButtonSortAz);
    sortButtons.addButton(ui->pushButtonSortCost);
    ui->pushButtonSortAz->click();

    connect(&sortButtons, SIGNAL(buttonClicked(int)), SLOT(sortCards()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

}

NewDeckDialog::~NewDeckDialog()
{
    delete ui;
}

void NewDeckDialog::loadDeck()
{
    // Init UI
    QString name = deck.value("name").toString();
    this->heroClass = deck.value("heroClass").toInt();
    bool arenaDeck = deck.value("arenaDeck").toBool();
    if(arenaDeck)
    {
        ui->comboBoxMode->setCurrentIndex(ui->comboBoxMode->findText("Arena"));
    }
    else
    {
        ui->comboBoxMode->setCurrentIndex(ui->comboBoxMode->findText("Constructed"));
    }
    this->setWindowTitle(QString("Edit Deck: %1").arg(name));
    int index = ui->comboBoxHeroClass->findData(heroClass);
    ui->comboBoxHeroClass->setCurrentIndex(index);
    ui->lineEditName->setText(name);

    foreach(QJsonValue cardValue, deck.value("cards").toArray())
    {
        QJsonObject cardJson = cardValue.toObject();
        //qDebug() << cardJson.value("name").toString();

        QString name = cardJson["name"].toString();
        int count = cardJson["count"].toInt();
        int cardClass = cardJson["classs"].isUndefined()? 0 : cardJson.value("classs").toInt();
        int cost = cardJson["cost"].toInt();
        int id = cardJson["id"].toInt();

        deckMap.insert(name, cardJson);
        currentCount += count;

        QListWidgetItem* item = new QListWidgetItem(name);
        QString label = QString("[%1]  %2  (%3)").arg(cost).arg(name).arg(count);
        QPushButton* button = new QPushButton(label);
        button->setProperty("name", name);
        button->setProperty("class", cardClass);
        button->setProperty("cost", cost);
        button->setStyleSheet(styleSheets.at(cardClass));

        item->setSizeHint(button->minimumSizeHint());

        ui->listWidgetDeck->addItem(item);
        ui->listWidgetDeck->setItemWidget(item, button);
        deckCardButtons->addButton(button, id);
    }

    sortCards();
    calculateStats();
}

void NewDeckDialog::on_comboBoxHeroClass_currentIndexChanged(const QString &value)
{
    if(value.length() != 0)
    {
        ui->listWidgetCards->clear();
        ui->listWidgetDeck->clear();
        addCardButtons = new QButtonGroup(this);
        deckCardButtons = new QButtonGroup(this);
        cardMap.clear();
        deckMap.clear();
        currentCount = 0;
        connect(addCardButtons, SIGNAL(buttonClicked(QAbstractButton*)), SLOT(addCard(QAbstractButton *)));
        connect(deckCardButtons, SIGNAL(buttonClicked(QAbstractButton*)), SLOT(removeCard(QAbstractButton *)));
        ui->comboBoxHeroClass->setEnabled(false);

        // Load Cards based on class
        this->heroClass = ui->comboBoxHeroClass->currentData().toInt();

        QProgressDialog progress("Loading Cards...", "Cancel", 0, cardData.count(), this);
        progress.setWindowModality(Qt::WindowModal);
        progress.setCancelButton(0);
        int count = 0;

        this->setUpdatesEnabled(false);
        foreach(QJsonValue value, cardData)
        {
            progress.setValue(++count);
            QJsonObject card = value.toObject();
            int cardClass = card.value("classs").isUndefined()? 0 : card.value("classs").toInt();

            if(cardClass == heroClass || cardClass == 0)
            {
                QString name = card.value("name").toString();
                int cost = card.value("cost").toInt();
                QString label = QString("[%1] %2").arg(cost).arg(name);

                QListWidgetItem* item = new QListWidgetItem(name);
                QPushButton* button = new QPushButton(label);
                button->setProperty("name", name);
                button->setProperty("class", cardClass);
                button->setProperty("cost", cost);
                button->setProperty("json", card);

                button->setStyleSheet(styleSheets.at(cardClass));
                item->setSizeHint(button->minimumSizeHint());

                ui->listWidgetCards->addItem(item);
                ui->listWidgetCards->setItemWidget(item, button);
                addCardButtons->addButton(button);

                // Insert to master map
                cardMap.insert(name, card);
            }
        }

        this->setUpdatesEnabled(true);
        sortCards();
    }
}

void NewDeckDialog::addCard(QAbstractButton *button)
{
    if(currentCount >= 30)
    {
        // Card Limit
        return;
    }

    QString name = button->property("name").toString();
    int cost = button->property("cost").toInt();
    int cardClass = button->property("class").toInt();

    if(!cardMap.contains(name))
    {
        // Card not found???
        return;
    }

    const QJsonObject &cardData = button->property("json").toJsonObject();
    if(deckMap.contains(name))
    {
        QJsonObject deckCard = deckMap.take(name);
        if(arenaMode)
        {
            int count = deckCard["count"].toInt();
            deckCard.remove("count");
            deckCard.insert("count", ++count);
            QString label = QString("[%1]  %2  (%3)").arg(cost).arg(name).arg(count);
            deckCardButtons->button(deckCard["id"].toInt())->setText(label);
            currentCount++;
        }
        else if(deckCard.value("count").toInt() < 2 && // Max count 2 in constructed
            deckCard.value("quality").toInt() != 5) // Check for Legendaries
        {
            int count = deckCard["count"].toInt();
            deckCard.remove("count");
            deckCard.insert("count", ++count);
            QString label = QString("[%1]  %2  (%3)").arg(cost).arg(name).arg(count);
            deckCardButtons->button(deckCard["id"].toInt())->setText(label);
            currentCount++;
        }
        deckMap.insert(name, deckCard);
    }
    else
    {
        QJsonObject deckCard(cardData);
        deckCard.insert("count", 1);
        deckMap.insert(name, deckCard);
        currentCount++;

        QListWidgetItem* item = new QListWidgetItem(name);
        QString label = QString("[%1]  %2  (1)").arg(cost).arg(name);
        QPushButton* button = new QPushButton(label);
        button->setProperty("name", name);
        button->setProperty("class", cardClass);
        button->setProperty("cost", cost);
        button->setStyleSheet(styleSheets.at(cardClass));

        item->setSizeHint(button->minimumSizeHint());

        ui->listWidgetDeck->addItem(item);
        ui->listWidgetDeck->setItemWidget(item, button);
        deckCardButtons->addButton(button, cardData["id"].toInt());
    }
    calculateStats();
    sortCards();
}

void NewDeckDialog::removeCard(QAbstractButton *button)
{
    QString name = button->property("name").toString();
    int cost = button->property("cost").toInt();

    if(!deckMap.contains(name))
    {
        // Card not found???
        return;
    }

    const QJsonObject &cardData = deckMap.value(name);
    if(cardData.value("count").toInt() > 1)
    {
        // Decrement Count
        QJsonObject deckCard = deckMap.take(name);
        int count = deckCard.value("count").toInt();
        deckCard.remove("count");
        deckCard.insert("count", --count);
        QString label = QString("[%1]  %2  (%3)").arg(cost).arg(name).arg(count);
        deckCardButtons->button(deckCard["id"].toInt())->setText(label);
        deckMap.insert(name, deckCard);
        currentCount--;
    }
    else
    {
        // Remove Card
        QJsonObject deckCard = deckMap.take(name);
        qDeleteAll(ui->listWidgetDeck->findItems(name, Qt::MatchExactly));
        delete deckCardButtons->button(deckCard["id"].toInt());
        currentCount--;
    }
    calculateStats();
    sortCards();
}

void NewDeckDialog::calculateStats()
{
    int manaCost[8] = {0};
    int percent[8] = {0};
    int spellCount = 0;
    int minionCount = 0;
    int weaponCount = 0;

    ui->labelCount->setText(QString("%1").arg(currentCount));
    foreach(QString key, deckMap.keys())
    {
        const QJsonObject &card = deckMap.value(key);
        int count = card.value("count").toInt();
        int type = card.value("type").toInt();
        int cost = card.value("cost").toInt();

//        qDebug() << card;
//        qDebug() << "Count: " << count;
//        qDebug() << "Type: " << type;
//        qDebug() << "Cost: " << cost;

        // Sort By Type
        if(type == 4) // Minion
        {
            minionCount += count;
        }
        else if(type == 5) // Spell
        {
            spellCount += count;
        }
        else if(type == 7) // Weapon
        {
            weaponCount += count;
        }

        // Mana Cost
        if(cost < 7)
        {
            manaCost[cost] += card.value("count").toInt();
        }
        else
        {
            manaCost[7] += card.value("count").toInt();
        }
    }

    ui->labelMinions->setText(QString("%1").arg(minionCount));
    ui->labelSpells->setText(QString("%1").arg(spellCount));
    ui->labelWeapons->setText(QString("%1").arg(weaponCount));

    if(currentCount == 0)
    {
        ui->progressBar0->setValue(0);
        ui->progressBar1->setValue(0);
        ui->progressBar2->setValue(0);
        ui->progressBar3->setValue(0);
        ui->progressBar4->setValue(0);
        ui->progressBar5->setValue(0);
        ui->progressBar6->setValue(0);
        ui->progressBar7->setValue(0);
        return;
    }

    // Calculate & set mana curves
    for(int i = 0; i < 8; i++)
    {
        percent[i] = static_cast<double>(manaCost[i]) / static_cast<double>(currentCount) * 100.0;
    }

    ui->progressBar0->setValue(percent[0] > 50? 50 : percent[0]);
    ui->progressBar1->setValue(percent[1] > 50? 50 : percent[1]);
    ui->progressBar2->setValue(percent[2] > 50? 50 : percent[2]);
    ui->progressBar3->setValue(percent[3] > 50? 50 : percent[3]);
    ui->progressBar4->setValue(percent[4] > 50? 50 : percent[4]);
    ui->progressBar5->setValue(percent[5] > 50? 50 : percent[5]);
    ui->progressBar6->setValue(percent[6] > 50? 50 : percent[6]);
    ui->progressBar7->setValue(percent[7] > 50? 50 : percent[7]);
}

QJsonObject NewDeckDialog::getDeck()
{
    QJsonObject deck;
    QJsonArray cards;
    foreach(QString key, deckMap.keys())
    {
        cards.append(deckMap.value(key));
    }
    deck.insert("heroClass", this->heroClass);
    deck.insert("arenaDeck", this->arenaMode);
    deck.insert("name", ui->lineEditName->text());
    deck.insert("cards", cards);
    return deck;
}

void NewDeckDialog::on_comboBoxMode_currentIndexChanged(const QString &arg1)
{
    if(arg1.contains("Arena", Qt::CaseInsensitive))
    {
        arenaMode = true;
    }
    else
    {
        arenaMode = false;
    }
}

void NewDeckDialog::sortCards()
{
    // Sort ListWidget
    ui->listWidgetCards->blockSignals(true);
    ui->listWidgetDeck->blockSignals(true);

    // Set the "display role" value
    foreach(QListWidgetItem *item, ui->listWidgetCards->findItems("*", Qt::MatchWildcard))
    {
        QAbstractButton *button = reinterpret_cast<QAbstractButton *>(ui->listWidgetCards->itemWidget(item));
        QString name = button->property("name").toString();
        int cost = button->property("cost").toInt();
        if(ui->pushButtonSortAz->isChecked())
        {
            item->setData(Qt::DisplayRole, name);
        }
        else if(ui->pushButtonSortCost->isChecked())
        {
            QString label = QString("%1 %2").arg(cost).arg(name);
            item->setData(Qt::DisplayRole, label);
        }
    }

    foreach(QListWidgetItem *item, ui->listWidgetDeck->findItems("*", Qt::MatchWildcard))
    {
        QAbstractButton *button = reinterpret_cast<QAbstractButton *>(ui->listWidgetDeck->itemWidget(item));
        QString name = button->property("name").toString();
        int cost = button->property("cost").toInt();
        if(ui->pushButtonSortAz->isChecked())
        {
            item->setData(Qt::DisplayRole, name);
        }
        else if(ui->pushButtonSortCost->isChecked())
        {
            QString label = QString("%1 %2").arg(cost).arg(name);
            item->setData(Qt::DisplayRole, label);
        }
    }

    // Sort
    ui->listWidgetCards->sortItems(Qt::AscendingOrder);
    ui->listWidgetDeck->sortItems(Qt::AscendingOrder);

    // Remove text
    foreach(QListWidgetItem *item, ui->listWidgetCards->findItems("*", Qt::MatchWildcard))
    {
        QAbstractButton *button = reinterpret_cast<QAbstractButton *>(ui->listWidgetCards->itemWidget(item));
        QString name = button->property("name").toString();
        item->setData(Qt::DisplayRole, name);
    }

    foreach(QListWidgetItem *item, ui->listWidgetDeck->findItems("*", Qt::MatchWildcard))
    {
        QAbstractButton *button = reinterpret_cast<QAbstractButton *>(ui->listWidgetDeck->itemWidget(item));
        QString name = button->property("name").toString();
        item->setData(Qt::DisplayRole, name);
    }

    ui->listWidgetCards->blockSignals(false);
    ui->listWidgetDeck->blockSignals(false);

    ui->listWidgetCards->update();
    ui->listWidgetDeck->update();
}
