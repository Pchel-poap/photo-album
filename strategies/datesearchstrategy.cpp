#include "datesearchstrategy.h"
#include <QDate>

QList<Photo> DateSearchStrategy::search(const QList<Photo>& photos, const QString& query) const {
    QList<Photo> results;

    QDate searchDate = QDate::fromString(query, "yyyy-MM-dd");
    bool searchByDay = searchDate.isValid();

    QDate searchMonth = QDate::fromString(query + "-01", "yyyy-MM-dd");
    bool searchByMonth = !searchByDay && searchMonth.isValid();

    bool searchByYear = false;
    int year = 0;
    if (!searchByDay && !searchByMonth) {
        bool ok;
        year = query.toInt(&ok);
        searchByYear = ok && year >= 1900 && year <= 2100;
    }

    for (const Photo& photo : photos) {
        QDate photoDate = photo.dateTime().date();

        if (searchByDay && photoDate == searchDate) {
            results.append(photo);
        } else if (searchByMonth &&
                   photoDate.year() == searchMonth.year() &&
                   photoDate.month() == searchMonth.month()) {
            results.append(photo);
        } else if (searchByYear && photoDate.year() == year) {
            results.append(photo);
        }
    }

    return results;
}

QString DateSearchStrategy::name() const {
    return "Поиск по дате";
}
