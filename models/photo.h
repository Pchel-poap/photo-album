#ifndef PHOTO_H
#define PHOTO_H

#include <QString>
#include <QDateTime>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>

class Photo {
public:
    Photo();
    Photo(const QString& id, const QString& filePath);

    QString id() const;
    QString filePath() const;
    QString description() const;
    QDateTime dateTime() const;
    QStringList tagIds() const;
    QString albumId() const;

    void setFilePath(const QString& path);
    void setDescription(const QString& description);
    void setDateTime(const QDateTime& dateTime);
    void setAlbumId(const QString& albumId);

    void addTag(const QString& tagId);
    void removeTag(const QString& tagId);
    void clearTags();
    bool hasTag(const QString& tagId) const;

    QJsonObject toJson() const;
    static Photo fromJson(const QJsonObject& json);

    bool operator==(const Photo& other) const;

private:
    QString m_id;
    QString m_filePath;
    QString m_description;
    QDateTime m_dateTime;
    QStringList m_tagIds;
    QString m_albumId;
};

#endif // PHOTO_H
