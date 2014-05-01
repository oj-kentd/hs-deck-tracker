#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QButtonGroup>
#include <QScrollArea>
#include <QLabel>
#include <QVBoxLayout>
#include "deckwidget.h"
#include "carddata.h"
#include "newdeckdialog.h"
#include "activedeck.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QMap<QString, QJsonObject> decks;
    QWidget *deckWidgetList;
    QVBoxLayout *deckListLayout;

    void reloadCardData();
    void serializeDeckList();
    void updateDeckList();
    void addDeck(const QJsonObject &deck);

private slots:
    void loadDeckList();
    void loadCardList();
    void on_pushButtonNewDeck_clicked();
    void on_actionAbout_triggered();
    void on_actionReloadCardData_triggered();
    void onResult(QNetworkReply *reply);
    void deleteDeck(DeckWidget *widget, QJsonObject &deck);
    void editDeck(DeckWidget *widget, QJsonObject &deck);
    void on_actionExit_triggered();
};

#endif // MAINWINDOW_H
