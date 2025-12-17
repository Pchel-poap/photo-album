#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "../managers/usermanager.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(UserManager* userManager, QWidget* parent = nullptr);
    ~LoginDialog();

private slots:
    void onUserSelected(int index);
    void onCreateUser();
    void onDeleteUser();
    void onLogin();

private:
    Ui::LoginDialog* ui;
    UserManager* m_userManager;

    void refreshUserList();
};

#endif // LOGINDIALOG_H
