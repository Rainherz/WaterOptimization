#ifndef POPUPDIALOG_H
#define POPUPDIALOG_H

#include <QDialog>
#include <QRegularExpression>

namespace Ui {
class PopupDialog;
}

class PopupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PopupDialog(QWidget *parent = nullptr);
    ~PopupDialog();
    int getIterations() const;

private slots:
    void onAcceptClicked();
    void onCancelClicked();

private:
    Ui::PopupDialog *ui;
    int iterations;
    bool validateInput(const QString &text);
};

#endif // POPUPDIALOG_H
