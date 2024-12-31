#ifndef WARNINGDIALOG_H
#define WARNINGDIALOG_H

#include <QDialog>

namespace Ui {
class WarningDialog; // Cambiado para que apunte al diseño correcto
}

class WarningDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WarningDialog(QWidget *parent = nullptr);
    ~WarningDialog();

private slots:
    void onAcceptClicked();
    void onCancelClicked();

private:
    Ui::WarningDialog *ui; // Cambiado para que coincida con el nuevo diseño
};

#endif // WARNINGDIALOG_H
