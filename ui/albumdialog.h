#ifndef ALBUMDIALOG_H
#define ALBUMDIALOG_H

#include <QDialog>
#include "../models/album.h"
#include "../models/tag.h"

namespace Ui {
class AlbumDialog;
}

class AlbumDialog : public QDialog {
    Q_OBJECT

public:
    explicit AlbumDialog(const Album& album, const QList<Tag>& tags, QWidget* parent = nullptr);
    ~AlbumDialog();

    Album album() const;

private slots:
    void onSave();
    void onCancel();

private:
    Ui::AlbumDialog* ui;
    Album m_album;
    QList<Tag> m_tags;

    void loadAlbumData();
    void loadTags();
};

#endif // ALBUMDIALOG_H
