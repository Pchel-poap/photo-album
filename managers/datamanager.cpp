#include "datamanager.h"
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QStandardPaths>

DataManager::DataManager() {
    m_dataDirectory = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

void DataManager::setDataDirectory(const QString& path) {
    m_dataDirectory = path;
}

QString DataManager::dataDirectory() const {
    return m_dataDirectory;
}

QString DataManager::usersFilePath() const {
    return m_dataDirectory + "/users.json";
}

QString DataManager::photosFilePath() const {
    return m_dataDirectory + "/photos.json";
}

QString DataManager::albumsFilePath() const {
    return m_dataDirectory + "/albums.json";
}

QString DataManager::tagsFilePath() const {
    return m_dataDirectory + "/tags.json";
}

bool DataManager::ensureDataDirectoryExists() {
    QDir dir(m_dataDirectory);
    if (!dir.exists()) {
        return dir.mkpath(".");
    }
    return true;
}

bool DataManager::saveUsers(const QList<User>& users) {
    if (!ensureDataDirectoryExists()) return false;

    QJsonArray array;
    for (const User& user : users) {
        array.append(user.toJson());
    }

    QJsonDocument doc(array);
    QFile file(usersFilePath());
    if (!file.open(QIODevice::WriteOnly)) return false;

    file.write(doc.toJson());
    file.close();
    return true;
}

bool DataManager::savePhotos(const QList<Photo>& photos) {
    if (!ensureDataDirectoryExists()) return false;

    QJsonArray array;
    for (const Photo& photo : photos) {
        array.append(photo.toJson());
    }

    QJsonDocument doc(array);
    QFile file(photosFilePath());
    if (!file.open(QIODevice::WriteOnly)) return false;

    file.write(doc.toJson());
    file.close();
    return true;
}

bool DataManager::saveAlbums(const QList<Album>& albums) {
    if (!ensureDataDirectoryExists()) return false;

    QJsonArray array;
    for (const Album& album : albums) {
        array.append(album.toJson());
    }

    QJsonDocument doc(array);
    QFile file(albumsFilePath());
    if (!file.open(QIODevice::WriteOnly)) return false;

    file.write(doc.toJson());
    file.close();
    return true;
}

bool DataManager::saveTags(const QList<Tag>& tags) {
    if (!ensureDataDirectoryExists()) return false;

    QJsonArray array;
    for (const Tag& tag : tags) {
        array.append(tag.toJson());
    }

    QJsonDocument doc(array);
    QFile file(tagsFilePath());
    if (!file.open(QIODevice::WriteOnly)) return false;

    file.write(doc.toJson());
    file.close();
    return true;
}

QList<User> DataManager::loadUsers() {
    QList<User> users;
    QFile file(usersFilePath());
    if (!file.open(QIODevice::ReadOnly)) return users;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonArray array = doc.array();
    for (const QJsonValue& val : array) {
        users.append(User::fromJson(val.toObject()));
    }

    return users;
}

QList<Photo> DataManager::loadPhotos() {
    QList<Photo> photos;
    QFile file(photosFilePath());
    if (!file.open(QIODevice::ReadOnly)) return photos;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonArray array = doc.array();
    for (const QJsonValue& val : array) {
        photos.append(Photo::fromJson(val.toObject()));
    }

    return photos;
}

QList<Album> DataManager::loadAlbums() {
    QList<Album> albums;
    QFile file(albumsFilePath());
    if (!file.open(QIODevice::ReadOnly)) return albums;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonArray array = doc.array();
    for (const QJsonValue& val : array) {
        albums.append(Album::fromJson(val.toObject()));
    }

    return albums;
}

QList<Tag> DataManager::loadTags() {
    QList<Tag> tags;
    QFile file(tagsFilePath());
    if (!file.open(QIODevice::ReadOnly)) return tags;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonArray array = doc.array();
    for (const QJsonValue& val : array) {
        tags.append(Tag::fromJson(val.toObject()));
    }

    return tags;
}

bool DataManager::exportAlbum(const Album& album, const QList<Photo>& photos,
                              const QList<Album>& subAlbums, const QString& exportPath) {
    QDir exportDir(exportPath);
    if (!exportDir.exists()) {
        exportDir.mkpath(".");
    }

    QJsonObject exportData;
    exportData["album"] = album.toJson();

    QJsonArray photosArray;
    for (const Photo& photo : photos) {
        photosArray.append(photo.toJson());
    }
    exportData["photos"] = photosArray;

    QJsonArray subAlbumsArray;
    for (const Album& subAlbum : subAlbums) {
        subAlbumsArray.append(subAlbum.toJson());
    }
    exportData["subAlbums"] = subAlbumsArray;

    QJsonDocument doc(exportData);
    QFile file(exportPath + "/album_data.json");
    if (!file.open(QIODevice::WriteOnly)) return false;

    file.write(doc.toJson());
    file.close();

    QString photosDir = exportPath + "/photos";
    QDir().mkpath(photosDir);

    for (const Photo& photo : photos) {
        QFileInfo fileInfo(photo.filePath());
        QString destPath = photosDir + "/" + fileInfo.fileName();
        QFile::copy(photo.filePath(), destPath);
    }

    return true;
}

bool DataManager::importAlbum(const QString& importPath, Album& album,
                              QList<Photo>& photos, QList<Album>& subAlbums) {
    QFile file(importPath + "/album_data.json");
    if (!file.open(QIODevice::ReadOnly)) return false;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject importData = doc.object();

    Album importedAlbum = Album::fromJson(importData["album"].toObject());
    album = Album();
    album.setName(importedAlbum.name());
    album.setDescription(importedAlbum.description());
    QString newAlbumId = album.id();

    QString photosSourceDir = importPath + "/photos";
    QString photosDestDir = m_dataDirectory + "/imported_photos";
    QDir().mkpath(photosDestDir);

    photos.clear();
    QJsonArray photosArray = importData["photos"].toArray();
    for (const QJsonValue& val : photosArray) {
        Photo importedPhoto = Photo::fromJson(val.toObject());

        Photo photo;
        photo.setDescription(importedPhoto.description());
        photo.setDateTime(importedPhoto.dateTime());
        photo.setAlbumId(newAlbumId);

        QFileInfo fileInfo(importedPhoto.filePath());
        QString sourceFile = photosSourceDir + "/" + fileInfo.fileName();
        QString destFile = photosDestDir + "/" + photo.id() + "_" + fileInfo.fileName();

        if (QFile::exists(sourceFile)) {
            QFile::copy(sourceFile, destFile);
            photo.setFilePath(destFile);
        } else {
            photo.setFilePath(importedPhoto.filePath());
        }

        for (const QString& tagId : importedPhoto.tagIds()) {
            photo.addTag(tagId);
        }

        photos.append(photo);
    }

    subAlbums.clear();
    QJsonArray subAlbumsArray = importData["subAlbums"].toArray();
    for (const QJsonValue& val : subAlbumsArray) {
        Album importedSubAlbum = Album::fromJson(val.toObject());
        Album subAlbum;
        subAlbum.setName(importedSubAlbum.name());
        subAlbum.setDescription(importedSubAlbum.description());
        subAlbum.setParentId(newAlbumId);
        subAlbums.append(subAlbum);
    }

    return true;
}
