#include "tagsearchstrategy.h"

TagSearchStrategy::TagSearchStrategy(const QMap<QString, Tag>& tagsMap)
    : m_tagsMap(tagsMap) {}

QList<Photo> TagSearchStrategy::search(const QList<Photo>& photos, const QString& query) const {
    QList<Photo> results;
    QString lowerQuery = query.toLower();

    for (const Photo& photo : photos) {
        for (const QString& tagId : photo.tagIds()) {
            if (m_tagsMap.contains(tagId)) {
                Tag tag = m_tagsMap[tagId];
                if (tag.name().toLower().contains(lowerQuery)) {
                    results.append(photo);
                    break;
                }
            }
        }
    }

    return results;
}

QString TagSearchStrategy::name() const {
    return "Поиск по тегам";
}
