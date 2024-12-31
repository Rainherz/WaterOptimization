#include "warningdialog.h"
#include "ui_warning.h"

WarningDialog::WarningDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WarningDialog) // Cambiado para que coincida
{
    ui->setupUi(this);
    setWindowTitle("Advertencia");

    connect(ui->pushButton, &QPushButton::clicked, this, &WarningDialog::onAcceptClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &WarningDialog::onCancelClicked);
}

WarningDialog::~WarningDialog()
{
    delete ui;
}

void WarningDialog::onAcceptClicked()
{
    accept();
}

void WarningDialog::onCancelClicked()
{
    reject();
}
