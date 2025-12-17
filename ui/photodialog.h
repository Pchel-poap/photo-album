#ifndef PHOTODIALOG_H
#define PHOTODIALOG_H

#include <QDialog>
#include "../models/photo.h"
#include "../models/tag.h"

namespace Ui {
class PhotoDialog;
}

class PhotoDialog : public QDialog {
    Q_OBJECT

public:
    explicit PhotoDialog(const Photo& photo, const QList<Tag>& tags, QWidget* parent = nullptr);
    ~PhotoDialog();

    Photo photo() const;

private slots:
    void onSave();
    void onCancel();

private:
    Ui::PhotoDialog* ui;
    Photo m_photo;
    QList<Tag> m_tags;

    void loadPhotoData();
    void loadTags();
};

#endif // PHOTODIALOG_H
