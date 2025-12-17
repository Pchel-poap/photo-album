#ifndef USER_H
#define USER_H

#include <QString>
#include <QJsonObject>

class User {
public:
    User();
    User(const QString& id, const QString& username);

    QString id() const;
    QString username() const;
    QString displayName() const;
    QString avatarPath() const;

    void setUsername(const QString& username);
    void setDisplayName(const QString& displayName);
    void setAvatarPath(const QString& path);

    QJsonObject toJson() const;
    static User fromJson(const QJsonObject& json);

    bool operator==(const User& other) const;

private:
    QString m_id;
    QString m_username;
    QString m_displayName;
    QString m_avatarPath;
};

#endif // USER_H
