#include "descriptionsearchstrategy.h"

QList<Photo> DescriptionSearchStrategy::search(const QList<Photo>& photos, const QString& query) const {
    QList<Photo> results;
    QString lowerQuery = query.toLower();

    for (const Photo& photo : photos) {
        if (photo.description().toLower().contains(lowerQuery)) {
            results.append(photo);
        }
    }

    return results;
}

QString DescriptionSearchStrategy::name() const {
    return "Поиск по описанию";
}
