#ifndef ALBUM_H
#define ALBUM_H

#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <memory>

class Album {
public:
    Album();
    Album(const QString& id, const QString& name);

    QString id() const;
    QString name() const;
    QString description() const;
    QStringList tagIds() const;
    QString parentId() const;
    QString userId() const;

    void setName(const QString& name);
    void setDescription(const QString& description);
    void setParentId(const QString& parentId);
    void setUserId(const QString& userId);

    void addTag(const QString& tagId);
    void removeTag(const QString& tagId);
    bool hasTag(const QString& tagId) const;

    void addChildAlbumId(const QString& childId);
    void removeChildAlbumId(const QString& childId);
    QStringList childAlbumIds() const;
    bool hasChildren() const;

    bool isRoot() const;

    QJsonObject toJson() const;
    static Album fromJson(const QJsonObject& json);

    bool operator==(const Album& other) const;

private:
    QString m_id;
    QString m_name;
    QString m_description;
    QStringList m_tagIds;
    QString m_parentId;
    QStringList m_childAlbumIds;
    QString m_userId;
};

#endif // ALBUM_H
