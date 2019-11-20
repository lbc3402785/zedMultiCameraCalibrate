#include "dialog.h"
#include "ui_dialog.h"
#include <QString>
#include "settings.h"
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    assign();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::assign()
{
    Settings& sets=Settings::instance();
    ui->boardW->setValue(sets.chessBoardConfig.boardW);
    ui->boardH->setValue(sets.chessBoardConfig.boardH);
    ui->dx->setValue(sets.chessBoardConfig.dx);
    ui->dy->setValue(sets.chessBoardConfig.dy);
    ui->imageDir->setText(sets.getImageDir());
    ui->outputDir->setText(sets.getOutputDir());
    ui->surffix->setText(sets.getImageSurffix());
}

void Dialog::on_save_clicked()
{
    Settings& set = Settings::instance();
    set.chessBoardConfig.boardW=ui->boardW->value();
    set.chessBoardConfig.boardH=ui->boardH->value();
    set.chessBoardConfig.dx=ui->dx->value();
    set.chessBoardConfig.dy=ui->dy->value();
    set.setImageDir(ui->imageDir->text());
    set.setOutputDir(ui->outputDir->text());
    set.setImageSurffix(ui->surffix->text());
    set.saveSettings();
    accept();
}

void Dialog::on_cancel_clicked()
{
    reject();
}

void Dialog::on_reset_clicked()
{
    Settings::instance().reset();
}
