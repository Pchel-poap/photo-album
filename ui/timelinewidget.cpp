#include "timelinewidget.h"
#include <QHBoxLayout>
#include <QFrame>
#include <QKeyEvent>
#include <QPalette>
#include <algorithm>

TimelineWidget::TimelineWidget(const QList<Photo>& photos, QWidget* parent)
    : QWidget(parent)
{
    setupUI(photos);
    if (!photos.isEmpty()) {
        buildTimeline(photos);
    }
}

void TimelineWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        close();
    }
    QWidget::keyPressEvent(event);
}

void TimelineWidget::setupUI(const QList<Photo>& photos) {
    setWindowTitle("Хронология");
    setWindowFlags(Qt::Window);
    resize(900, 600);

    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(245, 245, 245));
    setPalette(pal);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* headerLayout = new QHBoxLayout();
    QLabel* titleLabel = new QLabel("Хронология фотографий");
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    QPushButton* closeButton = new QPushButton("Закрыть");
    connect(closeButton, &QPushButton::clicked, this, &QWidget::close);
    headerLayout->addWidget(closeButton);

    mainLayout->addLayout(headerLayout);

    if (photos.isEmpty()) {
        QLabel* emptyLabel = new QLabel("Нет фотографий для отображения.\n\nВыберите альбом с фотографиями\nили добавьте фотографии в альбом.");
        emptyLabel->setAlignment(Qt::AlignCenter);
        emptyLabel->setStyleSheet("font-size: 14px; color: #666; padding: 50px;");
        mainLayout->addWidget(emptyLabel);
        mainLayout->addStretch();
        return;
    }

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_contentWidget = new QWidget();
    m_layout = new QVBoxLayout(m_contentWidget);
    m_layout->setAlignment(Qt::AlignTop);

    m_scrollArea->setWidget(m_contentWidget);
    mainLayout->addWidget(m_scrollArea);
}

void TimelineWidget::buildTimeline(const QList<Photo>& photos) {

    QList<Photo> sortedPhotos = photos;
    std::sort(sortedPhotos.begin(), sortedPhotos.end(), [](const Photo& a, const Photo& b) {
        return a.dateTime() > b.dateTime();
    });

    QMap<QString, QList<Photo>> groupedPhotos;
    for (const Photo& photo : sortedPhotos) {
        QString monthKey = photo.dateTime().toString("MMMM yyyy");
        groupedPhotos[monthKey].append(photo);
    }

    for (auto it = groupedPhotos.begin(); it != groupedPhotos.end(); ++it) {

        QLabel* monthLabel = new QLabel(it.key());
        monthLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #333; padding: 10px 0;");
        m_layout->addWidget(monthLabel);

        QWidget* photosContainer = new QWidget();
        QHBoxLayout* photosLayout = new QHBoxLayout(photosContainer);
        photosLayout->setAlignment(Qt::AlignLeft);

        for (const Photo& photo : it.value()) {

            QWidget* photoCard = new QWidget();
            photoCard->setFixedSize(160, 200);
            photoCard->setStyleSheet("background-color: white; border: 1px solid #ddd; border-radius: 5px;");

            QVBoxLayout* cardLayout = new QVBoxLayout(photoCard);
            cardLayout->setContentsMargins(5, 5, 5, 5);

            QLabel* thumbnail = new QLabel();
            QPixmap pixmap(photo.filePath());
            if (!pixmap.isNull()) {
                thumbnail->setPixmap(pixmap.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            } else {
                thumbnail->setText("Нет фото");
                thumbnail->setAlignment(Qt::AlignCenter);
            }
            thumbnail->setFixedSize(150, 150);
            cardLayout->addWidget(thumbnail);

            QLabel* dateLabel = new QLabel(photo.dateTime().toString("dd.MM.yyyy"));
            dateLabel->setStyleSheet("font-size: 10px; color: #666;");
            dateLabel->setAlignment(Qt::AlignCenter);
            cardLayout->addWidget(dateLabel);

            photosLayout->addWidget(photoCard);
        }

        m_layout->addWidget(photosContainer);

        QFrame* line = new QFrame();
        line->setFrameShape(QFrame::HLine);
        line->setStyleSheet("color: #eee;");
        m_layout->addWidget(line);
    }

    m_layout->addStretch();
}
