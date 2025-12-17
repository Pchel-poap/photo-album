#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QMap>
#include <QPushButton>
#include "../models/photo.h"

class TimelineWidget : public QWidget {
    Q_OBJECT

public:
    explicit TimelineWidget(const QList<Photo>& photos, QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    void setupUI(const QList<Photo>& photos);
    void buildTimeline(const QList<Photo>& photos);

    QScrollArea* m_scrollArea;
    QWidget* m_contentWidget;
    QVBoxLayout* m_layout;
};

#endif // TIMELINEWIDGET_H
