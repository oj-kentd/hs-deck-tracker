#include "activedeck.h"
#include "ui_activedeck.h"
#include <QPushButton>
#include <QListWidgetItem>

ActiveDeck::ActiveDeck(const QJsonObject &deck, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ActiveDeck),
    deck(deck),
    sortingButtons(this)
{
    ui->setupUi(this);
    sortingButtons.setExclusive(true);
    sortingButtons.addButton(ui->pushButtonSortAz);
    sortingButtons.addButton(ui->pushButtonSortPercent);
    sortingButtons.addButton(ui->pushButtonCost);

    this->setWindowTitle("Active Deck: " + deck.value("name").toString());
    loadCards();
    connect(ui->buttonBox->button(QDialogButtonBox::Reset), SIGNAL(clicked()), SLOT(resetDeck()));
    connect(ui->buttonBox, SIGNAL(rejected()), SLOT(reject()));
}

ActiveDeck::~ActiveDeck()
{
    delete ui;
}

void ActiveDeck::loadCards()
{
    ui->listWidgetCards->clear();
    this->cards.clear();
    QJsonArray cards = deck.value("cards").toArray();
    foreach(QJsonValue value, cards)
    {
        QJsonObject card = value.toObject();

        QString name = card.value("name").toString();
        QListWidgetItem *item = new QListWidgetItem();
        item->setData(Qt::UserRole, name);

        CardWidget *widget = new CardWidget(card);
        connect(widget, SIGNAL(updateCardArt(QImage&)), this, SLOT(updateCardLabel(QImage&)));
        this->cards.insert(name, widget);

        item->setSizeHint(widget->minimumSizeHint());

        ui->listWidgetCards->addItem(item);
        ui->listWidgetCards->setItemWidget(item, widget);

        connect(widget, SIGNAL(cardClicked()), this, SLOT(calculateStats()));
        connect(&sortingButtons, SIGNAL(buttonClicked(int)), this, SLOT(sortCards()));
    }

    calculateStats();
}

void ActiveDeck::calculateStats()
{
    int manaCost[8] = {0};
    int percent[8] = {0};
    int manaTotal = 0;
    int spellCount = 0;
    int minionCount = 0;
    int weaponCount = 0;

    // Count how many total cards remain
    int totalCount = 0;
    foreach(CardWidget *widget, this->cards.values())
    {
        totalCount += widget->getActiveCardCount();
    }

    // Calculate card statistics
    foreach(CardWidget *widget, this->cards.values())
    {
        const QJsonObject &cardJson = widget->getCardJson();
        int count = widget->getActiveCardCount();
        int type = cardJson.value("type").toInt();
        int cost = cardJson.value("cost").toInt();

        double percent = (double) count / (double) totalCount * 100.0;
        widget->setPercentage(percent);

        //qDebug() << "Count: " << count;
        //qDebug() << "Type: " << type;
        //qDebug() << "Cost: " << cost;

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
            manaCost[cost] += count;
        }
        else
        {
            manaCost[7] += count;
        }

        manaTotal += cost * count;
    }

    // Average Card Cost in deck
    double avgCost = (double) manaTotal / (double) totalCount;

    ui->labelMinions->setText(QString("%1").arg(minionCount));
    ui->labelSpells->setText(QString("%1").arg(spellCount));
    ui->labelWeapons->setText(QString("%1").arg(weaponCount));
    ui->labelCost->setText(QString("%1").arg(avgCost, 0, 'f', 1));
    ui->labelCount->setText(QString("%1").arg(totalCount));
    sortCards();

    if(totalCount == 0)
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
        percent[i] = static_cast<double>(manaCost[i]) / static_cast<double>(totalCount) * 100.0;
    }

    ui->progressBar0->setValue(percent[0]);
    ui->progressBar1->setValue(percent[1]);
    ui->progressBar2->setValue(percent[2]);
    ui->progressBar3->setValue(percent[3]);
    ui->progressBar4->setValue(percent[4]);
    ui->progressBar5->setValue(percent[5]);
    ui->progressBar6->setValue(percent[6]);
    ui->progressBar7->setValue(percent[7]);

    ui->labelCount0->setNum(manaCost[0]);
    ui->labelCount1->setNum(manaCost[1]);
    ui->labelCount2->setNum(manaCost[2]);
    ui->labelCount3->setNum(manaCost[3]);
    ui->labelCount4->setNum(manaCost[4]);
    ui->labelCount5->setNum(manaCost[5]);
    ui->labelCount6->setNum(manaCost[6]);
    ui->labelCount7->setNum(manaCost[7]);
}

void ActiveDeck::resetDeck()
{
    foreach(CardWidget *widget, this->cards.values())
    {
        widget->reset();
    }
    calculateStats();
}

void ActiveDeck::sortCards()
{
    // Sort ListWidget
    ui->listWidgetCards->blockSignals(true);

    // Set the "display role" value
    foreach(QListWidgetItem *item, ui->listWidgetCards->findItems("*", Qt::MatchWildcard))
    {
        CardWidget *widget = reinterpret_cast<CardWidget *>(ui->listWidgetCards->itemWidget(item));
        if(ui->pushButtonSortAz->isChecked())
        {
            item->setData(Qt::DisplayRole, widget->getName());
        }
        else if(ui->pushButtonSortPercent->isChecked())
        {
            // Invert percent for sorting
            double percent = widget->getPercent();
            QString label = QString("%1 %2").arg(percent, 3, 'f', 4).arg(widget->getName());
            item->setData(Qt::DisplayRole, label);
        }
        else if(ui->pushButtonCost->isChecked())
        {
            int cost = widget->getCardJson().value("cost").toInt();
            QString label = QString("%1 %2").arg(cost).arg(widget->getName());
            item->setData(Qt::DisplayRole, label);
        }
    }

    // Sort
    if(ui->pushButtonSortAz->isChecked() || ui->pushButtonCost->isChecked())
    {
        ui->listWidgetCards->sortItems(Qt::AscendingOrder);
    }
    else if(ui->pushButtonSortPercent->isChecked())
    {
        ui->listWidgetCards->sortItems(Qt::DescendingOrder);
    }



    // turn text off
    foreach(QListWidgetItem *item, ui->listWidgetCards->findItems("*", Qt::MatchWildcard))
    {
        item->setData(Qt::DisplayRole, "");
    }

    ui->listWidgetCards->blockSignals(false);
    ui->listWidgetCards->update();
}

void ActiveDeck::updateCardLabel(QImage &image)
{
    if(image.isNull())
    {
        ui->labelCardImage->setPixmap(QPixmap());
        return;
    }

    ui->labelCardImage->setPixmap(QPixmap::fromImage(image));
}
