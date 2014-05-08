#include "cardwidget.h"
#include "ui_cardwidget.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QTimer>
#include <QHBoxLayout>
#include <QPushButton>

CardWidget::CardWidget(QJsonObject &card, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CardWidget),
    card(card),
    cardImage()
{
    ui->setupUi(this);
    this->setName(card.value("name").toString());
    this->setCount(card.value("count").toInt());
    this->setCost(card.value("cost").toInt());

    cardButtons.setExclusive(false);
    connect(&this->cardButtons, SIGNAL(buttonClicked(int)), SLOT(buttonClickedSlot()));
}

CardWidget::~CardWidget()
{
    delete ui;
}

void CardWidget::enterEvent(QEvent *)
{
    emit updateCardArt(this->getCardArt());
}

void CardWidget::setCost(int cost)
{
    ui->labelCost->setText(QString("[%1]").arg(cost));
}

void CardWidget::setPercentage(double percent)
{
    this->percentage = percent;
    ui->labelPercent->setText(QString("%1%").arg(percent, 0, 'f', 1));
}

double CardWidget::getPercent() const
{
    return this->percentage;
}

void CardWidget::setName(QString name)
{
    ui->label->setText(name);
    this->name = name;
    QTimer::singleShot(100, this, SLOT(loadCardImage()));
}

void CardWidget::setCount(int count)
{
    this->count = count;
    QHBoxLayout *layout = new QHBoxLayout(ui->widgetButtonPlaceholder);
    layout->setMargin(0);
    for(int i = 0; i < count; i++)
    {
        QPushButton *button = new QPushButton(QString("%1").arg(i+1), this);
        button->setMaximumWidth(30);
        button->setMaximumHeight(20);
        button->setCheckable(true);
        this->cardButtons.addButton(button);
        layout->addWidget(button);
    }
}

void CardWidget::loadCardImage()
{
    QString filename = name.replace(' ', '-').replace('\'', '-').toLower();
    this->imageFilename = filename + ".png";
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    this->imagePath = dir.absolutePath() + QDir::separator();

    QFile file(imagePath + imageFilename);
    if(file.exists())
    {
        return;
    }

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(onResult(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl("http://s3-us-west-2.amazonaws.com/hearthstats/cards/" + imageFilename)));
}

void CardWidget::onResult(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError)
    {
        qWarning() << "Error reading card image";
        return;
    }

    QImage* image = new QImage();
    image->loadFromData(reply->readAll());

    if(image->isNull())
    {
        // Error
        return;
    }

    //qDebug() << "Save Image: " << imagePath + imageFilename;
    if(!image->save(imagePath + imageFilename))
    {
        qWarning() << "Could not save card image to: " << QString(imagePath + imageFilename);
    }

    //this->setToolTip("<html><img src=" + imagePath + imageFilename +"/></html>");
}

void CardWidget::buttonClickedSlot()
{
    emit cardClicked();
    ui->label->setEnabled(getActiveCardCount());
}

void CardWidget::mousePressEvent(QMouseEvent *event)
{
    QAbstractButton *lastButton = NULL;

    if(event->button() == Qt::LeftButton)
    {
        foreach(QAbstractButton *button, cardButtons.buttons())
        {
            if(!button->isChecked())
            {
                lastButton = button;
            }
        }
    }
    else
    {
        foreach(QAbstractButton *button, cardButtons.buttons())
        {
            if(button->isChecked())
            {
                lastButton = button;
                break;
            }
        }
    }

    if(lastButton)
    {
        lastButton->click();
    }
}

QString CardWidget::getName() const
{
    return this->name;
}

int CardWidget::getActiveCardCount() const
{
    int output = count;
    foreach(QAbstractButton *button, cardButtons.buttons())
    {
        if(button->isChecked())
        {
            output--;
        }
    }
    return output;
}

void CardWidget::reset()
{
    foreach(QAbstractButton *button, cardButtons.buttons())
    {
        button->setChecked(false);
    }
    ui->label->setEnabled(getActiveCardCount());
}

const QJsonObject &CardWidget::getCardJson() const
{
    return card;
}

const QImage &CardWidget::getCardArt()
{
    if(this->cardImage.isNull())
    {
        QFile file(imagePath + imageFilename);
        //qDebug() << file.fileName();
        if(file.exists())
        {
            // Local Cache exists, return image
            this->cardImage = QImage(imagePath + imageFilename);
        }
    }

    return cardImage;
}

