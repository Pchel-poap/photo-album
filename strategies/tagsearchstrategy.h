#ifndef TAGSEARCHSTRATEGY_H
#define TAGSEARCHSTRATEGY_H

#include "searchstrategy.h"
#include "../models/tag.h"
#include <QMap>

class TagSearchStrategy : public SearchStrategy {
public:
    TagSearchStrategy(const QMap<QString, Tag>& tagsMap);

    QList<Photo> search(const QList<Photo>& photos, const QString& query) const override;
    QString name() const override;

private:
    QMap<QString, Tag> m_tagsMap;
};

#endif // TAGSEARCHSTRATEGY_H
