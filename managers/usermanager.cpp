#include "usermanager.h"

UserManager::UserManager(QObject* parent) : QObject(parent) {}

void UserManager::addUser(const User& user) {
    m_users[user.id()] = user;
    emit userAdded(user);
    emit dataChanged();
}

void UserManager::updateUser(const User& user) {
    if (m_users.contains(user.id())) {
        m_users[user.id()] = user;
        emit userUpdated(user);

        if (m_currentUserId == user.id()) {
            emit currentUserChanged(user);
        }
        emit dataChanged();
    }
}

void UserManager::removeUser(const QString& userId) {
    if (m_users.contains(userId)) {
        m_users.remove(userId);
        emit userRemoved(userId);

        if (m_currentUserId == userId) {
            m_currentUserId.clear();
            if (!m_users.isEmpty()) {
                setCurrentUser(m_users.first().id());
            }
        }
        emit dataChanged();
    }
}

User UserManager::getUser(const QString& userId) const {
    return m_users.value(userId);
}

QList<User> UserManager::getAllUsers() const {
    return m_users.values();
}

bool UserManager::hasUsers() const {
    return !m_users.isEmpty();
}

void UserManager::setCurrentUser(const QString& userId) {
    if (m_users.contains(userId)) {
        m_currentUserId = userId;
        emit currentUserChanged(m_users[userId]);
    }
}

User UserManager::currentUser() const {
    return m_users.value(m_currentUserId);
}

QString UserManager::currentUserId() const {
    return m_currentUserId;
}

bool UserManager::hasCurrentUser() const {
    return !m_currentUserId.isEmpty() && m_users.contains(m_currentUserId);
}

void UserManager::setUsers(const QList<User>& users) {
    m_users.clear();
    for (const User& user : users) {
        m_users[user.id()] = user;
    }
    emit dataChanged();
}
