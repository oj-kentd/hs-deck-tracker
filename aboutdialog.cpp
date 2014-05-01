#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <QMessageBox>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    QString text = ui->labelAbout->text();
    text.replace("%VERSION%", APP_VERSION);
    ui->labelAbout->setText(text);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_pushButtonAboutQt_clicked()
{
    QMessageBox::aboutQt(this);
}
