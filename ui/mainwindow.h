#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <memory>
#include "../managers/photomanager.h"
#include "../managers/usermanager.h"
#include "../managers/datamanager.h"

namespace Ui {
class MainWindow;
}

class TimelineWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:

    void onAddPhoto();
    void onImportAlbum();
    void onExportAlbum();
    void onSwitchUser();
    void onExit();

    void onCreateAlbum();
    void onCreateSubAlbum();
    void onEditAlbum();
    void onDeleteAlbum();

    void onManageTags();

    void onSearch();
    void onSearchTypeChanged(int index);

    void onAlbumTreeItemClicked(QTreeWidgetItem* item, int column);
    void onAlbumContextMenu(const QPoint& pos);

    void onPhotoDoubleClicked(QListWidgetItem* item);
    void onPhotoContextMenu(const QPoint& pos);

    void onShowTimeline();

    void refreshAlbumTree();
    void refreshPhotoList();
    void saveData();

private:
    Ui::MainWindow* ui;

    PhotoManager* m_photoManager;
    UserManager* m_userManager;
    DataManager* m_dataManager;

    QString m_currentAlbumId;

    void setupUI();
    void setupConnections();
    void loadData();

    void buildAlbumTree(QTreeWidgetItem* parentItem, const QString& parentAlbumId);

    void displayPhotos(const QList<Photo>& photos);

    void editPhoto(const QString& photoId);
    void deletePhoto(const QString& photoId);
};

#endif // MAINWINDOW_H
