#ifndef TAG_H
#define TAG_H

#include <QString>
#include <QJsonObject>

class Tag {
public:
    Tag();
    Tag(const QString& id, const QString& name);

    QString id() const;
    QString name() const;

    void setName(const QString& name);

    QJsonObject toJson() const;
    static Tag fromJson(const QJsonObject& json);

    bool operator==(const Tag& other) const;
    bool operator!=(const Tag& other) const;

private:
    QString m_id;
    QString m_name;
};

#endif // TAG_H
