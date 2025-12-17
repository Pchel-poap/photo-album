#include "user.h"
#include <QUuid>

User::User()
    : m_id(QUuid::createUuid().toString(QUuid::WithoutBraces)) {}

User::User(const QString& id, const QString& username)
    : m_id(id), m_username(username), m_displayName(username) {}

QString User::id() const { return m_id; }
QString User::username() const { return m_username; }
QString User::displayName() const { return m_displayName; }
QString User::avatarPath() const { return m_avatarPath; }

void User::setUsername(const QString& username) { m_username = username; }
void User::setDisplayName(const QString& displayName) { m_displayName = displayName; }
void User::setAvatarPath(const QString& path) { m_avatarPath = path; }

QJsonObject User::toJson() const {
    QJsonObject json;
    json["id"] = m_id;
    json["username"] = m_username;
    json["displayName"] = m_displayName;
    json["avatarPath"] = m_avatarPath;
    return json;
}

User User::fromJson(const QJsonObject& json) {
    User user(json["id"].toString(), json["username"].toString());
    user.setDisplayName(json["displayName"].toString());
    user.setAvatarPath(json["avatarPath"].toString());
    return user;
}

bool User::operator==(const User& other) const {
    return m_id == other.m_id;
}
