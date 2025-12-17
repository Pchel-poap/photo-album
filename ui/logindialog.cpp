#include "logindialog.h"
#include "ui_logindialog.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QWhatsThis>

LoginDialog::LoginDialog(UserManager* userManager, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
    , m_userManager(userManager)
{
    ui->setupUi(this);

    setWhatsThis("Выбор пользователя.\n\n"
                 "Каждый пользователь имеет свои альбомы и фотографии. "
                 "Создайте нового пользователя или выберите существующего.");
    ui->userComboBox->setWhatsThis("Выберите пользователя из списка для входа.");
    ui->createUserButton->setWhatsThis("Создать нового пользователя.");
    ui->deleteUserButton->setWhatsThis("Удалить выбранного пользователя и все его данные.");
    ui->loginButton->setWhatsThis("Войти под выбранным пользователем.");

    connect(ui->userComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LoginDialog::onUserSelected);
    connect(ui->createUserButton, &QPushButton::clicked, this, &LoginDialog::onCreateUser);
    connect(ui->deleteUserButton, &QPushButton::clicked, this, &LoginDialog::onDeleteUser);
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginDialog::onLogin);

    refreshUserList();

    if (m_userManager->hasCurrentUser()) {
        int index = ui->userComboBox->findData(m_userManager->currentUserId());
        if (index >= 0) {
            ui->userComboBox->setCurrentIndex(index);
        }
    }
}

LoginDialog::~LoginDialog() {
    delete ui;
}

void LoginDialog::refreshUserList() {
    ui->userComboBox->clear();

    QList<User> users = m_userManager->getAllUsers();
    for (const User& user : users) {
        ui->userComboBox->addItem(user.displayName(), user.id());
    }

    ui->loginButton->setEnabled(!users.isEmpty());
    ui->deleteUserButton->setEnabled(!users.isEmpty());
}

void LoginDialog::onUserSelected(int index) {
    Q_UNUSED(index);
}

void LoginDialog::onCreateUser() {
    bool ok;
    QString username = QInputDialog::getText(this, "Создать пользователя",
                                             "Имя пользователя:", QLineEdit::Normal, "", &ok);

    if (ok && !username.isEmpty()) {
        User user;
        user.setUsername(username);
        user.setDisplayName(username);
        m_userManager->addUser(user);
        refreshUserList();

        int index = ui->userComboBox->findData(user.id());
        if (index >= 0) {
            ui->userComboBox->setCurrentIndex(index);
        }
    }
}

void LoginDialog::onDeleteUser() {
    if (ui->userComboBox->currentIndex() < 0) return;

    QString userId = ui->userComboBox->currentData().toString();
    User user = m_userManager->getUser(userId);

    int result = QMessageBox::question(
        this, "Подтверждение",
        QString("Удалить пользователя '%1'?").arg(user.displayName()),
        QMessageBox::Yes | QMessageBox::No
    );

    if (result == QMessageBox::Yes) {
        m_userManager->removeUser(userId);
        refreshUserList();
    }
}

void LoginDialog::onLogin() {
    if (ui->userComboBox->currentIndex() < 0) {
        QMessageBox::warning(this, "Внимание", "Выберите пользователя или создайте нового.");
        return;
    }

    QString userId = ui->userComboBox->currentData().toString();
    m_userManager->setCurrentUser(userId);
    accept();
}
