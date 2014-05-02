#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QTimer>
#include "aboutdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButtonNewDeck->setEnabled(false);

    // Setup Deck List Widget
    deckWidgetList = new QWidget(ui->scrollAreaDecks);
    deckListLayout = new QVBoxLayout(deckWidgetList);
    deckWidgetList->setLayout(deckListLayout);
    deckWidgetList->setContentsMargins(2, 2, 2, 2);
    deckListLayout->setContentsMargins(2, 2, 2, 2);
    deckListLayout->setSpacing(2);
    deckListLayout->insertStretch(-1);
    ui->scrollAreaDecks->setWidget(deckWidgetList);

    QTimer::singleShot(100, this, SLOT(loadCardList()));
    QTimer::singleShot(200, this, SLOT(loadDeckList()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadCardList()
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    if(!dir.exists())
    {
        dir.mkpath(".");
    }
    QString filename = "cards.json";
    QFile cardsFile(dir.absolutePath() + QDir::separator() + filename);

    if(!cardsFile.exists() || !cardsFile.open(QIODevice::ReadOnly))
    {
        int value = QMessageBox::question(this, tr("Load Card Data."), tr("Hearthstone card data not found. Load card data from internet?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(value == QMessageBox::Yes)
        {
            reloadCardData();
            ui->statusBar->showMessage(tr("Card Data not found. Loading from Internet."), 2000);
            return;
        }
    }
    else
    {
        QJsonDocument cardsJson = QJsonDocument::fromJson(cardsFile.readAll());
        CardData::instance().setCards(cardsJson.array());
        ui->pushButtonNewDeck->setEnabled(true);
    }
}

void MainWindow::reloadCardData()
{
    ui->pushButtonNewDeck->setEnabled(false);

    // Load Card Data from hearthstoneapi.com
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(onResult(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl("http://hearthstoneapi.com/cards/findAll")));
}

void MainWindow::onResult(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qWarning() << tr("Error reading cards");
        return;
    }

    QJsonParseError parseError;
    QJsonDocument cardsJson = QJsonDocument::fromJson(reply->readAll(), &parseError);
    CardData::instance().setCards(cardsJson.array());

    QDir dir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    if(!dir.exists())
    {
        dir.mkpath(".");
    }
    QString filename = "cards.json";

    QFile outFile(dir.absolutePath() + QDir::separator() + filename);
    if(!outFile.open(QIODevice::WriteOnly))
    {
        qWarning() << QString(tr("Couldn't open %1 for writing.")).arg(filename);
        return;
    }
    outFile.write(cardsJson.toJson());
    ui->pushButtonNewDeck->setEnabled(true);
}

void MainWindow::on_pushButtonNewDeck_clicked()
{
    NewDeckDialog dialog(this);
    int retVal = dialog.exec();
    if(retVal == QDialog::Accepted)
    {
        QJsonObject deck = dialog.getDeck();
        QString name = deck["name"].toString();
        if(name.isEmpty())
        {
            // Get name for deck
            name = QInputDialog::getText(this,
                                         tr("Enter name for deck:"),
                                         tr("Name:"));
            deck.take("name");
            deck.insert("name", name);
        }
        decks.insert(name, deck);
        addDeck(deck);
        serializeDeckList();
    }
}

void MainWindow::serializeDeckList()
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    if(!dir.exists())
    {
        dir.mkpath(".");
    }

    QString filename = "decks.json";
    QFile saveFile(dir.absolutePath() + QDir::separator() + filename);

    if (!saveFile.open(QIODevice::WriteOnly))
    {
        qWarning() << tr("Couldn't open decks.json file for output.");
        return;
    }

    QJsonArray array;
    foreach(QString key, decks.keys())
    {
        array.append(decks.value(key));
    }

    QJsonDocument output;
    output.setArray(array);
    saveFile.write(output.toJson());
}

void MainWindow::loadDeckList()
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    QString filename = "decks.json";
    QFile loadFile(dir.absolutePath() + QDir::separator() + filename);

    if(!loadFile.exists())
    {
        return;
    }

    if(!loadFile.open(QIODevice::ReadOnly))
    {
       qWarning() << tr("Couldn't open decks file.");
       return;
    }

    this->decks.clear();
    QByteArray saveData = loadFile.readAll();
    QJsonParseError parseError;
    QJsonDocument input(QJsonDocument::fromJson(saveData, &parseError));

    if(parseError.error != QJsonParseError::NoError)
    {
        QMessageBox::warning(this, tr("JSON Error"), tr("Error Loading \"decks.json\". Error: %1").arg(parseError.errorString()), QMessageBox::Ok);
    }

    QJsonArray array = input.array();
    for(int i = 0; i < array.size(); i++)
    {
        QJsonObject deck = array.at(i).toObject();
        this->decks.insert(deck.value("name").toString(), deck);
    }

    updateDeckList();
}

void MainWindow::updateDeckList()
{
    for(int i = 0; i < decks.keys().size(); i++)
    {
        addDeck(decks.value(decks.keys().at(i)));
    }
}

void MainWindow::addDeck(const QJsonObject &deck)
{
    DeckWidget *widget = new DeckWidget(deck, deckWidgetList);
    connect(widget, SIGNAL(deleteDeck(DeckWidget *, QJsonObject &)), this, SLOT(deleteDeck(DeckWidget *, QJsonObject &)));
    connect(widget, SIGNAL(editDeck(DeckWidget *, QJsonObject &)), this, SLOT(editDeck(DeckWidget *, QJsonObject &)));
    int row = deckListLayout->count()-1;
    widget->setProperty("row", row);
    deckListLayout->insertWidget(row, widget);
}

void MainWindow::deleteDeck(DeckWidget *widget, QJsonObject &deck)
{
    QString key = deck.value("name").toString();
    decks.take(key);
    widget->deleteLater();
    serializeDeckList();
}

void MainWindow::editDeck(DeckWidget *widget, QJsonObject &deck)
{
    QString key = deck.value("name").toString();

    NewDeckDialog dialog(deck, this);
    int retVal = dialog.exec();
    if(retVal == QDialog::Accepted)
    {
        QJsonObject newDeck = dialog.getDeck();
        widget->updateDeck(newDeck);
        QString name = deck["name"].toString();
        decks.take(key);
        decks.insert(name, deck);
        serializeDeckList();
    }
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog *dialog = new AboutDialog(this);
    dialog->show();
}

void MainWindow::on_actionReloadCardData_triggered()
{
    int value = QMessageBox::question(this, tr("Load Card Data."), tr("Reload Hearthstone card data from the Internet?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(value == QMessageBox::Yes)
    {
        reloadCardData();
        ui->statusBar->showMessage(tr("Reloading card data from Internet."), 2000);
        return;
    }
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}
