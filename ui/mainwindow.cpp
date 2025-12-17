#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logindialog.h"
#include "photodialog.h"
#include "albumdialog.h"
#include "timelinewidget.h"
#include "photoviewer.h"
#include "tagsdialog.h"
#include "../strategies/tagsearchstrategy.h"
#include "../strategies/datesearchstrategy.h"
#include "../strategies/descriptionsearchstrategy.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>
#include <QInputDialog>
#include <QTimer>
#include <QSet>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_photoManager(new PhotoManager(this))
    , m_userManager(new UserManager(this))
    , m_dataManager(new DataManager())
{
    ui->setupUi(this);
    setupUI();
    setupConnections();
    loadData();

    LoginDialog dialog(m_userManager, this);
    if (dialog.exec() != QDialog::Accepted || !m_userManager->hasCurrentUser()) {
        QTimer::singleShot(0, this, &QWidget::close);
        return;
    }

    refreshAlbumTree();
    refreshPhotoList();
}

MainWindow::~MainWindow() {
    saveData();
    delete ui;
    delete m_dataManager;
}

void MainWindow::setupUI() {
    setWindowTitle("Фотоальбом - " + m_userManager->currentUser().displayName());

    ui->photoList->setSpacing(10);
}

void MainWindow::setupConnections() {

    connect(ui->actionAddPhoto, &QAction::triggered, this, &MainWindow::onAddPhoto);
    connect(ui->actionImportAlbum, &QAction::triggered, this, &MainWindow::onImportAlbum);
    connect(ui->actionExportAlbum, &QAction::triggered, this, &MainWindow::onExportAlbum);
    connect(ui->actionSwitchUser, &QAction::triggered, this, &MainWindow::onSwitchUser);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onExit);

    connect(ui->actionCreateAlbum, &QAction::triggered, this, &MainWindow::onCreateAlbum);
    connect(ui->actionCreateSubAlbum, &QAction::triggered, this, &MainWindow::onCreateSubAlbum);
    connect(ui->actionEditAlbum, &QAction::triggered, this, &MainWindow::onEditAlbum);
    connect(ui->actionDeleteAlbum, &QAction::triggered, this, &MainWindow::onDeleteAlbum);

    connect(ui->actionManageTags, &QAction::triggered, this, &MainWindow::onManageTags);

    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::onSearch);
    connect(ui->searchEdit, &QLineEdit::returnPressed, this, &MainWindow::onSearch);
    connect(ui->searchTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onSearchTypeChanged);

    connect(ui->albumTree, &QTreeWidget::itemClicked, this, &MainWindow::onAlbumTreeItemClicked);
    connect(ui->albumTree, &QWidget::customContextMenuRequested, this, &MainWindow::onAlbumContextMenu);

    connect(ui->photoList, &QListWidget::itemDoubleClicked, this, &MainWindow::onPhotoDoubleClicked);
    connect(ui->photoList, &QWidget::customContextMenuRequested, this, &MainWindow::onPhotoContextMenu);

    connect(ui->timelineButton, &QPushButton::clicked, this, &MainWindow::onShowTimeline);

    connect(m_photoManager, &PhotoManager::dataChanged, this, &MainWindow::saveData);
    connect(m_userManager, &UserManager::currentUserChanged, this, [this](const User& user) {
        setWindowTitle("Фотоальбом - " + user.displayName());
        refreshAlbumTree();
        refreshPhotoList();
    });
}

void MainWindow::loadData() {
    m_userManager->setUsers(m_dataManager->loadUsers());
    m_photoManager->setPhotos(m_dataManager->loadPhotos());
    m_photoManager->setAlbums(m_dataManager->loadAlbums());
    m_photoManager->setTags(m_dataManager->loadTags());
}

void MainWindow::saveData() {
    m_dataManager->saveUsers(m_userManager->getAllUsers());
    m_dataManager->savePhotos(m_photoManager->getAllPhotos());
    m_dataManager->saveAlbums(m_photoManager->getAllAlbums());
    m_dataManager->saveTags(m_photoManager->getAllTags());
}

void MainWindow::onAddPhoto() {
    if (m_currentAlbumId.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Сначала выберите альбом для добавления фото.");
        return;
    }

    QStringList filePaths = QFileDialog::getOpenFileNames(
        this, "Выберите фотографии", QString(),
        "Изображения (*.png *.jpg *.jpeg *.bmp *.gif);;Все файлы (*.*)"
    );

    for (const QString& filePath : filePaths) {
        Photo photo;
        photo.setFilePath(filePath);
        photo.setAlbumId(m_currentAlbumId);

        PhotoDialog dialog(photo, m_photoManager->getAllTags(), this);
        if (dialog.exec() == QDialog::Accepted) {
            m_photoManager->addPhoto(dialog.photo());
        }
    }

    refreshPhotoList();
}

void MainWindow::onImportAlbum() {
    QString importPath = QFileDialog::getExistingDirectory(this, "Выберите папку для импорта");
    if (importPath.isEmpty()) return;

    Album album;
    QList<Photo> photos;
    QList<Album> subAlbums;

    if (m_dataManager->importAlbum(importPath, album, photos, subAlbums)) {
        album.setUserId(m_userManager->currentUserId());
        m_photoManager->addAlbum(album);

        for (const Photo& photo : photos) {
            m_photoManager->addPhoto(photo);
        }

        for (Album& subAlbum : subAlbums) {
            subAlbum.setUserId(m_userManager->currentUserId());
            m_photoManager->addAlbum(subAlbum);
        }

        refreshAlbumTree();
        QMessageBox::information(this, "Успех", "Альбом успешно импортирован!");
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось импортировать альбом.");
    }
}

void MainWindow::onExportAlbum() {
    if (m_currentAlbumId.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Сначала выберите альбом для экспорта.");
        return;
    }

    QString exportPath = QFileDialog::getExistingDirectory(this, "Выберите папку для экспорта");
    if (exportPath.isEmpty()) return;

    Album album = m_photoManager->getAlbum(m_currentAlbumId);
    QList<Photo> photos = m_photoManager->getPhotosRecursive(m_currentAlbumId);
    QList<Album> subAlbums = m_photoManager->getChildAlbums(m_currentAlbumId);

    QString albumExportPath = exportPath + "/" + album.name();

    if (m_dataManager->exportAlbum(album, photos, subAlbums, albumExportPath)) {
        QMessageBox::information(this, "Успех", "Альбом успешно экспортирован!");
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось экспортировать альбом.");
    }
}

void MainWindow::onSwitchUser() {
    saveData();
    LoginDialog dialog(m_userManager, this);
    dialog.exec();
    m_currentAlbumId.clear();
    refreshAlbumTree();
    refreshPhotoList();
}

void MainWindow::onExit() {
    saveData();
    close();
}

void MainWindow::onCreateAlbum() {
    Album album;
    album.setUserId(m_userManager->currentUserId());

    AlbumDialog dialog(album, m_photoManager->getAllTags(), this);
    dialog.setWindowTitle("Создать альбом");

    if (dialog.exec() == QDialog::Accepted) {
        m_photoManager->addAlbum(dialog.album());
        refreshAlbumTree();
    }
}

void MainWindow::onCreateSubAlbum() {
    if (m_currentAlbumId.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Сначала выберите родительский альбом.");
        return;
    }

    Album album;
    album.setUserId(m_userManager->currentUserId());
    album.setParentId(m_currentAlbumId);

    AlbumDialog dialog(album, m_photoManager->getAllTags(), this);
    dialog.setWindowTitle("Создать подальбом");

    if (dialog.exec() == QDialog::Accepted) {
        m_photoManager->addAlbum(dialog.album());
        refreshAlbumTree();
    }
}

void MainWindow::onEditAlbum() {
    if (m_currentAlbumId.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Сначала выберите альбом для редактирования.");
        return;
    }

    Album album = m_photoManager->getAlbum(m_currentAlbumId);
    AlbumDialog dialog(album, m_photoManager->getAllTags(), this);
    dialog.setWindowTitle("Редактировать альбом");

    if (dialog.exec() == QDialog::Accepted) {
        m_photoManager->updateAlbum(dialog.album());
        refreshAlbumTree();
    }
}

void MainWindow::onDeleteAlbum() {
    if (m_currentAlbumId.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Сначала выберите альбом для удаления.");
        return;
    }

    Album album = m_photoManager->getAlbum(m_currentAlbumId);

    int result = QMessageBox::question(
        this, "Подтверждение",
        QString("Удалить альбом '%1' и все его содержимое?").arg(album.name()),
        QMessageBox::Yes | QMessageBox::No
    );

    if (result == QMessageBox::Yes) {
        m_photoManager->removeAlbum(m_currentAlbumId);
        m_currentAlbumId.clear();
        refreshAlbumTree();
        refreshPhotoList();
    }
}

void MainWindow::onManageTags() {
    TagsDialog dialog(m_photoManager, this);
    dialog.exec();
}

void MainWindow::onSearch() {
    QString query = ui->searchEdit->text();
    if (query.isEmpty()) {
        refreshPhotoList();
        return;
    }

    QList<Photo> results = m_photoManager->search(query);

    QList<Photo> filteredResults;
    QSet<QString> userAlbumIds;

    for (const Album& album : m_photoManager->getAllAlbums()) {
        if (album.userId() == m_userManager->currentUserId()) {
            userAlbumIds.insert(album.id());
        }
    }

    for (const Photo& photo : results) {
        if (userAlbumIds.contains(photo.albumId())) {
            filteredResults.append(photo);
        }
    }

    displayPhotos(filteredResults);
    ui->labelPhotos->setText(QString("Результаты поиска: %1").arg(filteredResults.size()));
}

void MainWindow::onSearchTypeChanged(int index) {
    switch (index) {
        case 0:
            m_photoManager->setSearchStrategy(std::make_shared<DescriptionSearchStrategy>());
            break;
        case 1:
            m_photoManager->setSearchStrategy(
                std::make_shared<TagSearchStrategy>(m_photoManager->getTagsMap()));
            break;
        case 2:
            m_photoManager->setSearchStrategy(std::make_shared<DateSearchStrategy>());
            break;
    }
}

void MainWindow::onAlbumTreeItemClicked(QTreeWidgetItem* item, int column) {
    Q_UNUSED(column);
    m_currentAlbumId = item->data(0, Qt::UserRole).toString();
    Album album = m_photoManager->getAlbum(m_currentAlbumId);
    ui->labelPhotos->setText(QString("Фотографии: %1").arg(album.name()));
    refreshPhotoList();
}

void MainWindow::onAlbumContextMenu(const QPoint& pos) {
    QTreeWidgetItem* item = ui->albumTree->itemAt(pos);

    QMenu menu(this);

    if (item) {
        m_currentAlbumId = item->data(0, Qt::UserRole).toString();

        QAction* addPhotoAction = menu.addAction("Добавить фото");
        menu.addSeparator();
        QAction* editAction = menu.addAction("Редактировать");
        QAction* deleteAction = menu.addAction("Удалить");
        menu.addSeparator();
        QAction* subAlbumAction = menu.addAction("Создать подальбом");

        QAction* selectedAction = menu.exec(ui->albumTree->mapToGlobal(pos));

        if (selectedAction == addPhotoAction) {
            onAddPhoto();
        } else if (selectedAction == editAction) {
            onEditAlbum();
        } else if (selectedAction == deleteAction) {
            onDeleteAlbum();
        } else if (selectedAction == subAlbumAction) {
            onCreateSubAlbum();
        }
    } else {
        QAction* createAction = menu.addAction("Создать альбом");

        QAction* selectedAction = menu.exec(ui->albumTree->mapToGlobal(pos));

        if (selectedAction == createAction) {
            onCreateAlbum();
        }
    }
}

void MainWindow::onPhotoDoubleClicked(QListWidgetItem* item) {
    QString photoId = item->data(Qt::UserRole).toString();
    Photo photo = m_photoManager->getPhoto(photoId);

    PhotoViewer* viewer = new PhotoViewer(photo, this);
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->show();
}

void MainWindow::onPhotoContextMenu(const QPoint& pos) {
    QListWidgetItem* item = ui->photoList->itemAt(pos);
    if (!item) return;

    QString photoId = item->data(Qt::UserRole).toString();

    QMenu menu(this);
    QAction* editAction = menu.addAction("Редактировать");
    QAction* deleteAction = menu.addAction("Удалить");

    QAction* selectedAction = menu.exec(ui->photoList->mapToGlobal(pos));

    if (selectedAction == editAction) {
        editPhoto(photoId);
    } else if (selectedAction == deleteAction) {
        deletePhoto(photoId);
    }
}

void MainWindow::onShowTimeline() {
    QList<Photo> photos;
    if (m_currentAlbumId.isEmpty()) {
        for (const Album& album : m_photoManager->getRootAlbums(m_userManager->currentUserId())) {
            photos.append(m_photoManager->getPhotosRecursive(album.id()));
        }
    } else {
        photos = m_photoManager->getPhotosRecursive(m_currentAlbumId);
    }

    TimelineWidget* timeline = new TimelineWidget(photos, this);
    timeline->setAttribute(Qt::WA_DeleteOnClose);
    timeline->show();
}

void MainWindow::refreshAlbumTree() {
    ui->albumTree->clear();

    if (!m_userManager->hasCurrentUser()) return;

    QList<Album> rootAlbums = m_photoManager->getRootAlbums(m_userManager->currentUserId());

    for (const Album& album : rootAlbums) {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->albumTree);
        item->setText(0, album.name());
        item->setData(0, Qt::UserRole, album.id());

        buildAlbumTree(item, album.id());
    }

    ui->albumTree->expandAll();
}

void MainWindow::buildAlbumTree(QTreeWidgetItem* parentItem, const QString& parentAlbumId) {
    QList<Album> childAlbums = m_photoManager->getChildAlbums(parentAlbumId);

    for (const Album& album : childAlbums) {
        QTreeWidgetItem* item = new QTreeWidgetItem(parentItem);
        item->setText(0, album.name());
        item->setData(0, Qt::UserRole, album.id());

        buildAlbumTree(item, album.id());
    }
}

void MainWindow::refreshPhotoList() {
    if (m_currentAlbumId.isEmpty()) {
        ui->photoList->clear();
        ui->labelPhotos->setText("Фотографии");
        return;
    }

    QList<Photo> photos = m_photoManager->getPhotosByAlbum(m_currentAlbumId);
    displayPhotos(photos);
}

void MainWindow::displayPhotos(const QList<Photo>& photos) {
    ui->photoList->clear();

    for (const Photo& photo : photos) {
        QListWidgetItem* item = new QListWidgetItem(ui->photoList);

        QPixmap pixmap(photo.filePath());
        if (!pixmap.isNull()) {
            item->setIcon(QIcon(pixmap.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
        } else {
            item->setText("Нет изображения");
        }

        item->setData(Qt::UserRole, photo.id());
        item->setToolTip(photo.description());
    }
}

void MainWindow::editPhoto(const QString& photoId) {
    Photo photo = m_photoManager->getPhoto(photoId);

    PhotoDialog dialog(photo, m_photoManager->getAllTags(), this);
    dialog.setWindowTitle("Редактировать фото");

    if (dialog.exec() == QDialog::Accepted) {
        m_photoManager->updatePhoto(dialog.photo());
        refreshPhotoList();
    }
}

void MainWindow::deletePhoto(const QString& photoId) {
    int result = QMessageBox::question(
        this, "Подтверждение",
        "Удалить выбранную фотографию?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (result == QMessageBox::Yes) {
        m_photoManager->removePhoto(photoId);
        refreshPhotoList();
    }
}
