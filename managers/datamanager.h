#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QString>
#include <QList>
#include <QMap>
#include "../models/user.h"
#include "../models/photo.h"
#include "../models/album.h"
#include "../models/tag.h"

class DataManager {
public:
    DataManager();

    void setDataDirectory(const QString& path);
    QString dataDirectory() const;

    bool saveUsers(const QList<User>& users);
    bool savePhotos(const QList<Photo>& photos);
    bool saveAlbums(const QList<Album>& albums);
    bool saveTags(const QList<Tag>& tags);

    QList<User> loadUsers();
    QList<Photo> loadPhotos();
    QList<Album> loadAlbums();
    QList<Tag> loadTags();

    bool exportAlbum(const Album& album, const QList<Photo>& photos,
                     const QList<Album>& subAlbums, const QString& exportPath);
    bool importAlbum(const QString& importPath, Album& album,
                     QList<Photo>& photos, QList<Album>& subAlbums);

private:
    QString m_dataDirectory;

    QString usersFilePath() const;
    QString photosFilePath() const;
    QString albumsFilePath() const;
    QString tagsFilePath() const;

    bool ensureDataDirectoryExists();
};

#endif // DATAMANAGER_H
