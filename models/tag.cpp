#include "tag.h"
#include <QUuid>

Tag::Tag() : m_id(QUuid::createUuid().toString(QUuid::WithoutBraces)) {}

Tag::Tag(const QString& id, const QString& name)
    : m_id(id), m_name(name) {}

QString Tag::id() const { return m_id; }
QString Tag::name() const { return m_name; }

void Tag::setName(const QString& name) { m_name = name; }

QJsonObject Tag::toJson() const {
    QJsonObject json;
    json["id"] = m_id;
    json["name"] = m_name;
    return json;
}

Tag Tag::fromJson(const QJsonObject& json) {
    return Tag(json["id"].toString(), json["name"].toString());
}

bool Tag::operator==(const Tag& other) const {
    return m_id == other.m_id;
}

bool Tag::operator!=(const Tag& other) const {
    return !(*this == other);
}
