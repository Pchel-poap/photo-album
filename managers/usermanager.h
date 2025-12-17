#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>
#include "../models/user.h"

class UserManager : public QObject {
    Q_OBJECT

public:
    explicit UserManager(QObject* parent = nullptr);

    void addUser(const User& user);
    void updateUser(const User& user);
    void removeUser(const QString& userId);
    User getUser(const QString& userId) const;
    QList<User> getAllUsers() const;
    bool hasUsers() const;

    void setCurrentUser(const QString& userId);
    User currentUser() const;
    QString currentUserId() const;
    bool hasCurrentUser() const;

    void setUsers(const QList<User>& users);

signals:
    void userAdded(const User& user);
    void userUpdated(const User& user);
    void userRemoved(const QString& userId);
    void currentUserChanged(const User& user);
    void dataChanged();

private:
    QMap<QString, User> m_users;
    QString m_currentUserId;
};

#endif // USERMANAGER_H
