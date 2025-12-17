#ifndef TAGSDIALOG_H
#define TAGSDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include "../managers/photomanager.h"

class TagsDialog : public QDialog {
    Q_OBJECT

public:
    explicit TagsDialog(PhotoManager* photoManager, QWidget* parent = nullptr);

private slots:
    void onAddTag();
    void onDeleteTag();
    void onTagSelectionChanged();

private:
    void setupUI();
    void refreshTagsList();

    PhotoManager* m_photoManager;
    QListWidget* m_tagsList;
    QLineEdit* m_tagNameEdit;
    QPushButton* m_addButton;
    QPushButton* m_deleteButton;
};

#endif // TAGSDIALOG_H
