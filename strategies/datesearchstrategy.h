#ifndef DATESEARCHSTRATEGY_H
#define DATESEARCHSTRATEGY_H

#include "searchstrategy.h"

class DateSearchStrategy : public SearchStrategy {
public:
    DateSearchStrategy() = default;

    QList<Photo> search(const QList<Photo>& photos, const QString& query) const override;
    QString name() const override;
};

#endif // DATESEARCHSTRATEGY_H
