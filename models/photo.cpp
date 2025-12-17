#include "photo.h"
#include <QUuid>
#include <QFileInfo>

Photo::Photo()
    : m_id(QUuid::createUuid().toString(QUuid::WithoutBraces)),
      m_dateTime(QDateTime::currentDateTime()) {}

Photo::Photo(const QString& id, const QString& filePath)
    : m_id(id), m_filePath(filePath), m_dateTime(QDateTime::currentDateTime()) {}

QString Photo::id() const { return m_id; }
QString Photo::filePath() const { return m_filePath; }
QString Photo::description() const { return m_description; }
QDateTime Photo::dateTime() const { return m_dateTime; }
QStringList Photo::tagIds() const { return m_tagIds; }
QString Photo::albumId() const { return m_albumId; }

void Photo::setFilePath(const QString& path) { m_filePath = path; }
void Photo::setDescription(const QString& description) { m_description = description; }
void Photo::setDateTime(const QDateTime& dateTime) { m_dateTime = dateTime; }
void Photo::setAlbumId(const QString& albumId) { m_albumId = albumId; }

void Photo::addTag(const QString& tagId) {
    if (!m_tagIds.contains(tagId)) {
        m_tagIds.append(tagId);
    }
}

void Photo::removeTag(const QString& tagId) {
    m_tagIds.removeAll(tagId);
}

void Photo::clearTags() {
    m_tagIds.clear();
}

bool Photo::hasTag(const QString& tagId) const {
    return m_tagIds.contains(tagId);
}

QJsonObject Photo::toJson() const {
    QJsonObject json;
    json["id"] = m_id;
    json["filePath"] = m_filePath;
    json["description"] = m_description;
    json["dateTime"] = m_dateTime.toString(Qt::ISODate);
    json["albumId"] = m_albumId;

    QJsonArray tagsArray;
    for (const QString& tagId : m_tagIds) {
        tagsArray.append(tagId);
    }
    json["tagIds"] = tagsArray;

    return json;
}

Photo Photo::fromJson(const QJsonObject& json) {
    Photo photo(json["id"].toString(), json["filePath"].toString());
    photo.setDescription(json["description"].toString());
    photo.setDateTime(QDateTime::fromString(json["dateTime"].toString(), Qt::ISODate));
    photo.setAlbumId(json["albumId"].toString());

    QJsonArray tagsArray = json["tagIds"].toArray();
    for (const QJsonValue& val : tagsArray) {
        photo.addTag(val.toString());
    }

    return photo;
}

bool Photo::operator==(const Photo& other) const {
    return m_id == other.m_id;
}
