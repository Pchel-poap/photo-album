#ifndef DESCRIPTIONSEARCHSTRATEGY_H
#define DESCRIPTIONSEARCHSTRATEGY_H

#include "searchstrategy.h"

class DescriptionSearchStrategy : public SearchStrategy {
public:
    DescriptionSearchStrategy() = default;

    QList<Photo> search(const QList<Photo>& photos, const QString& query) const override;
    QString name() const override;
};

#endif // DESCRIPTIONSEARCHSTRATEGY_H
