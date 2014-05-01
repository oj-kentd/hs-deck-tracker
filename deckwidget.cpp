#include "deckwidget.h"
#include "ui_deckwidget.h"
#include "newdeckdialog.h"
#include "activedeck.h"
#include "HearthstoneApi.h"

DeckWidget::DeckWidget(const QJsonObject &deck, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeckWidget),
    deck(deck)
{
    ui->setupUi(this);
    int heroClass = deck.value("heroClass").toInt();
    QString style = QString(
                "QLabel {"
                    "min-width: 80px;"
                    "background-color: %1;"
                    "font-weight: bold;"
                "}").arg(HeroColors[heroClass]);
    ui->label->setStyleSheet(style);
    ui->label->setText(deck.value("name").toString());
    ui->label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

DeckWidget::~DeckWidget()
{
    delete ui;
}

void DeckWidget::updateDeck(QJsonObject &deck)
{
    this->deck = deck;
    ui->label->setText(deck.value("name").toString());
}

void DeckWidget::on_pushButtonNewGame_clicked()
{
    ActiveDeck *dialog = new ActiveDeck(deck, this);
    dialog->setModal(false);
    dialog->show();
}

void DeckWidget::on_pushButtonEdit_clicked()
{
    emit editDeck(this, this->deck);
}

void DeckWidget::on_pushButtonDelete_clicked()
{
    emit deleteDeck(this, this->deck);
}
