#include "photoviewer.h"
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QResizeEvent>
#include <QTimer>

PhotoViewer::PhotoViewer(const Photo& photo, QWidget* parent)
    : QWidget(parent)
    , m_photo(photo)
    , m_scaleFactor(1.0)
{
    setupUI();
}

void PhotoViewer::setupUI() {
    setWindowTitle("Просмотр фото");
    setWindowFlags(Qt::Window);
    resize(800, 600);
    setMinimumSize(400, 300);
    setStyleSheet("background-color: #2d2d2d;");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setAlignment(Qt::AlignCenter);
    m_scrollArea->setStyleSheet("border: none;");

    m_imageLabel = new QLabel();
    m_imageLabel->setAlignment(Qt::AlignCenter);

    m_originalPixmap = QPixmap(m_photo.filePath());
    if (!m_originalPixmap.isNull()) {
        m_imageLabel->setPixmap(m_originalPixmap);
    } else {
        m_imageLabel->setText("Не удалось загрузить изображение");
        m_imageLabel->setStyleSheet("color: white; font-size: 18px;");
    }

    m_scrollArea->setWidget(m_imageLabel);
    layout->addWidget(m_scrollArea);

    m_infoLabel = new QLabel(this);
    m_infoLabel->setStyleSheet("color: white; background-color: rgba(0,0,0,0.7); padding: 10px;");

    QString info = QString("Дата: %1\nОписание: %2\n[Колесо мыши - масштаб, Esc - закрыть]")
        .arg(m_photo.dateTime().toString("dd.MM.yyyy HH:mm"))
        .arg(m_photo.description().isEmpty() ? "Нет описания" : m_photo.description());
    m_infoLabel->setText(info);

    layout->addWidget(m_infoLabel);

    QTimer::singleShot(0, this, &PhotoViewer::fitToWindow);
}

void PhotoViewer::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        close();
    } else if (event->key() == Qt::Key_Plus || event->key() == Qt::Key_Equal) {
        m_scaleFactor *= 1.25;
        updateZoom();
    } else if (event->key() == Qt::Key_Minus) {
        m_scaleFactor /= 1.25;
        updateZoom();
    } else if (event->key() == Qt::Key_0) {
        m_scaleFactor = 1.0;
        updateZoom();
    }
    QWidget::keyPressEvent(event);
}

void PhotoViewer::wheelEvent(QWheelEvent* event) {
    if (event->angleDelta().y() > 0) {
        m_scaleFactor *= 1.1;
    } else {
        m_scaleFactor /= 1.1;
    }
    m_scaleFactor = qBound(0.1, m_scaleFactor, 5.0);
    updateZoom();
    event->accept();
}

void PhotoViewer::updateZoom() {
    if (m_originalPixmap.isNull()) return;

    QPixmap scaled = m_originalPixmap.scaled(
        m_originalPixmap.size() * m_scaleFactor,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
    );
    m_imageLabel->setPixmap(scaled);
    m_imageLabel->adjustSize();
}

void PhotoViewer::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    fitToWindow();
}

void PhotoViewer::fitToWindow() {
    if (m_originalPixmap.isNull()) return;

    QSize availableSize = m_scrollArea->viewport()->size();
    QPixmap scaled = m_originalPixmap.scaled(
        availableSize * m_scaleFactor,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
    );
    m_imageLabel->setPixmap(scaled);
}
