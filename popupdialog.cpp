#include "popupdialog.h"
#include "ui_popup.h"
#include "warningdialog.h"

PopupDialog::PopupDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PopupDialog)
    , iterations(0)
{
    ui->setupUi(this);
    setWindowTitle("Iteraciones");

    connect(ui->pushButton, &QPushButton::clicked, this, &PopupDialog::onAcceptClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &PopupDialog::onCancelClicked);
}

PopupDialog::~PopupDialog()
{
    delete ui;
}

bool PopupDialog::validateInput(const QString &text)
{
    // Verifica si el texto es un número y está en el rango permitido
    QRegularExpression re("^[0-9]+$");
    if (!re.match(text).hasMatch()) {
        return false;
    }

    bool ok;
    int value = text.toInt(&ok);
    return ok && value >= 1 && value <= 1000;
}

void PopupDialog::onAcceptClicked()
{
    QString input = ui->lineEdit->text();

    if (validateInput(input)) {
        iterations = input.toInt();
        accept();
    } else {
        WarningDialog warning(this);
        warning.exec();
    }
}

void PopupDialog::onCancelClicked()
{
    reject();
}

int PopupDialog::getIterations() const
{
    return iterations;
}
