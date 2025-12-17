#ifndef PHOTOVIEWER_H
#define PHOTOVIEWER_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include "../models/photo.h"

class PhotoViewer : public QWidget {
    Q_OBJECT

public:
    explicit PhotoViewer(const Photo& photo, QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void setupUI();
    void updateZoom();
    void fitToWindow();

    Photo m_photo;
    QLabel* m_imageLabel;
    QScrollArea* m_scrollArea;
    QLabel* m_infoLabel;
    double m_scaleFactor;
    QPixmap m_originalPixmap;
};

#endif // PHOTOVIEWER_H
