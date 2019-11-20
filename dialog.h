#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_save_clicked();

    void on_cancel_clicked();

    void on_reset_clicked();

private:
    Ui::Dialog *ui;
    void assign();
};

#endif // DIALOG_H
