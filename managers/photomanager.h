#ifndef PHOTOMANAGER_H
#define PHOTOMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>
#include <memory>
#include "../models/photo.h"
#include "../models/album.h"
#include "../models/tag.h"
#include "../strategies/searchstrategy.h"

class PhotoManager : public QObject {
    Q_OBJECT

public:
    explicit PhotoManager(QObject* parent = nullptr);

    void addPhoto(const Photo& photo);
    void updatePhoto(const Photo& photo);
    void removePhoto(const QString& photoId);
    Photo getPhoto(const QString& photoId) const;
    QList<Photo> getAllPhotos() const;
    QList<Photo> getPhotosByAlbum(const QString& albumId) const;

    void addAlbum(const Album& album);
    void updateAlbum(const Album& album);
    void removeAlbum(const QString& albumId);
    Album getAlbum(const QString& albumId) const;
    QList<Album> getAllAlbums() const;
    QList<Album> getRootAlbums(const QString& userId) const;
    QList<Album> getChildAlbums(const QString& parentId) const;

    QList<Photo> getPhotosRecursive(const QString& albumId) const;

    void addTag(const Tag& tag);
    void updateTag(const Tag& tag);
    void removeTag(const QString& tagId);
    Tag getTag(const QString& tagId) const;
    QList<Tag> getAllTags() const;
    QMap<QString, Tag> getTagsMap() const;

    void setSearchStrategy(std::shared_ptr<SearchStrategy> strategy);
    QList<Photo> search(const QString& query) const;

    void setPhotos(const QList<Photo>& photos);
    void setAlbums(const QList<Album>& albums);
    void setTags(const QList<Tag>& tags);

signals:
    void photoAdded(const Photo& photo);
    void photoUpdated(const Photo& photo);
    void photoRemoved(const QString& photoId);
    void albumAdded(const Album& album);
    void albumUpdated(const Album& album);
    void albumRemoved(const QString& albumId);
    void tagAdded(const Tag& tag);
    void tagUpdated(const Tag& tag);
    void tagRemoved(const QString& tagId);
    void dataChanged();

private:
    QMap<QString, Photo> m_photos;
    QMap<QString, Album> m_albums;
    QMap<QString, Tag> m_tags;
    std::shared_ptr<SearchStrategy> m_searchStrategy;
};

#endif // PHOTOMANAGER_H
