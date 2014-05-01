#ifndef NEWDECKDIALOG_H
#define NEWDECKDIALOG_H

#include <QDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QAbstractButton>
#include <QButtonGroup>
#include <QMap>

namespace Ui {
class NewDeckDialog;
}

class NewDeckDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewDeckDialog(QWidget *parent = 0);
    explicit NewDeckDialog(QJsonObject &deck, QWidget *parent = 0);
    ~NewDeckDialog();
    QJsonObject getDeck();

private slots:
    void on_comboBoxHeroClass_currentIndexChanged(const QString &arg1);
    void on_comboBoxMode_currentIndexChanged(const QString &arg1);
    void addCard(QAbstractButton *button);
    void removeCard(QAbstractButton *button);
    void calculateStats();
    void sortCards();
    void loadDeck();

private:
    Ui::NewDeckDialog *ui;
    QJsonArray cardData;
    QJsonObject deck;
    QButtonGroup *addCardButtons;
    QButtonGroup *deckCardButtons;
    QMap<QString, QJsonObject> cardMap;
    QMap<QString, QJsonObject> deckMap;
    int currentCount;
    bool arenaMode;
    int heroClass;
    QStringList styleSheets;
    QButtonGroup sortButtons;

    void initUi(void);

};

#endif // NEWDECKDIALOG_H
