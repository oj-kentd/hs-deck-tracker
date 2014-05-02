#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>
#include <QJsonObject>
#include <QNetworkReply>
#include <QButtonGroup>
#include <QMouseEvent>

namespace Ui {
class CardWidget;
}

class CardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CardWidget(QJsonObject &card, QWidget *parent = 0);
    ~CardWidget();
    void reset();
    QString getName() const;
    void setPercentage(double percent);
    int getActiveCardCount() const;
    const QJsonObject &getCardJson() const;
    double getPercent() const;
    const QImage &getCardArt();

signals:
    void cardClicked();
    void updateCardArt(const QImage &art);

protected:
    void enterEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *);

private:
    Ui::CardWidget *ui;
    QJsonObject card;
    QButtonGroup cardButtons;
    QString name;
    QString imageFilename;
    QString imagePath;
    int count;
    double percentage;
    QImage cardImage;

    void setName(QString name);
    void setCost(int cost);
    void setCount(int count);

private slots:
    void onResult(QNetworkReply *reply);
    void loadCardImage();
    void buttonClickedSlot();
};

#endif // CARDWIDGET_H

