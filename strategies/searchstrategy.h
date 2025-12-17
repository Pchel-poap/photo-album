#ifndef SEARCHSTRATEGY_H
#define SEARCHSTRATEGY_H

#include <QList>
#include <QString>
#include "../models/photo.h"

class SearchStrategy {
public:
    virtual ~SearchStrategy() = default;

    virtual QList<Photo> search(const QList<Photo>& photos, const QString& query) const = 0;

    virtual QString name() const = 0;
};

#endif // SEARCHSTRATEGY_H
