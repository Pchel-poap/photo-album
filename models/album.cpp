#include "album.h"
#include <QUuid>

Album::Album()
    : m_id(QUuid::createUuid().toString(QUuid::WithoutBraces)) {}

Album::Album(const QString& id, const QString& name)
    : m_id(id), m_name(name) {}

QString Album::id() const { return m_id; }
QString Album::name() const { return m_name; }
QString Album::description() const { return m_description; }
QStringList Album::tagIds() const { return m_tagIds; }
QString Album::parentId() const { return m_parentId; }
QString Album::userId() const { return m_userId; }

void Album::setName(const QString& name) { m_name = name; }
void Album::setDescription(const QString& description) { m_description = description; }
void Album::setParentId(const QString& parentId) { m_parentId = parentId; }
void Album::setUserId(const QString& userId) { m_userId = userId; }

void Album::addTag(const QString& tagId) {
    if (!m_tagIds.contains(tagId)) {
        m_tagIds.append(tagId);
    }
}

void Album::removeTag(const QString& tagId) {
    m_tagIds.removeAll(tagId);
}

bool Album::hasTag(const QString& tagId) const {
    return m_tagIds.contains(tagId);
}

void Album::addChildAlbumId(const QString& childId) {
    if (!m_childAlbumIds.contains(childId)) {
        m_childAlbumIds.append(childId);
    }
}

void Album::removeChildAlbumId(const QString& childId) {
    m_childAlbumIds.removeAll(childId);
}

QStringList Album::childAlbumIds() const {
    return m_childAlbumIds;
}

bool Album::hasChildren() const {
    return !m_childAlbumIds.isEmpty();
}

bool Album::isRoot() const {
    return m_parentId.isEmpty();
}

QJsonObject Album::toJson() const {
    QJsonObject json;
    json["id"] = m_id;
    json["name"] = m_name;
    json["description"] = m_description;
    json["parentId"] = m_parentId;
    json["userId"] = m_userId;

    QJsonArray tagsArray;
    for (const QString& tagId : m_tagIds) {
        tagsArray.append(tagId);
    }
    json["tagIds"] = tagsArray;

    QJsonArray childrenArray;
    for (const QString& childId : m_childAlbumIds) {
        childrenArray.append(childId);
    }
    json["childAlbumIds"] = childrenArray;

    return json;
}

Album Album::fromJson(const QJsonObject& json) {
    Album album(json["id"].toString(), json["name"].toString());
    album.setDescription(json["description"].toString());
    album.setParentId(json["parentId"].toString());
    album.setUserId(json["userId"].toString());

    QJsonArray tagsArray = json["tagIds"].toArray();
    for (const QJsonValue& val : tagsArray) {
        album.addTag(val.toString());
    }

    QJsonArray childrenArray = json["childAlbumIds"].toArray();
    for (const QJsonValue& val : childrenArray) {
        album.addChildAlbumId(val.toString());
    }

    return album;
}

bool Album::operator==(const Album& other) const {
    return m_id == other.m_id;
}
