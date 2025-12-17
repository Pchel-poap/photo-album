#include "photomanager.h"
#include "../strategies/descriptionsearchstrategy.h"

PhotoManager::PhotoManager(QObject* parent) : QObject(parent) {
    m_searchStrategy = std::make_shared<DescriptionSearchStrategy>();
}

void PhotoManager::addPhoto(const Photo& photo) {
    m_photos[photo.id()] = photo;
    emit photoAdded(photo);
    emit dataChanged();
}

void PhotoManager::updatePhoto(const Photo& photo) {
    if (m_photos.contains(photo.id())) {
        m_photos[photo.id()] = photo;
        emit photoUpdated(photo);
        emit dataChanged();
    }
}

void PhotoManager::removePhoto(const QString& photoId) {
    if (m_photos.contains(photoId)) {
        m_photos.remove(photoId);
        emit photoRemoved(photoId);
        emit dataChanged();
    }
}

Photo PhotoManager::getPhoto(const QString& photoId) const {
    return m_photos.value(photoId);
}

QList<Photo> PhotoManager::getAllPhotos() const {
    return m_photos.values();
}

QList<Photo> PhotoManager::getPhotosByAlbum(const QString& albumId) const {
    QList<Photo> result;
    for (const Photo& photo : m_photos) {
        if (photo.albumId() == albumId) {
            result.append(photo);
        }
    }
    return result;
}

void PhotoManager::addAlbum(const Album& album) {
    m_albums[album.id()] = album;

    if (!album.parentId().isEmpty() && m_albums.contains(album.parentId())) {
        Album parent = m_albums[album.parentId()];
        parent.addChildAlbumId(album.id());
        m_albums[parent.id()] = parent;
    }

    emit albumAdded(album);
    emit dataChanged();
}

void PhotoManager::updateAlbum(const Album& album) {
    if (m_albums.contains(album.id())) {
        m_albums[album.id()] = album;
        emit albumUpdated(album);
        emit dataChanged();
    }
}

void PhotoManager::removeAlbum(const QString& albumId) {
    if (!m_albums.contains(albumId)) return;

    Album album = m_albums[albumId];

    if (!album.parentId().isEmpty() && m_albums.contains(album.parentId())) {
        Album parent = m_albums[album.parentId()];
        parent.removeChildAlbumId(albumId);
        m_albums[parent.id()] = parent;
    }

    for (const QString& childId : album.childAlbumIds()) {
        removeAlbum(childId);
    }

    QList<QString> photosToRemove;
    for (const Photo& photo : m_photos) {
        if (photo.albumId() == albumId) {
            photosToRemove.append(photo.id());
        }
    }
    for (const QString& photoId : photosToRemove) {
        m_photos.remove(photoId);
    }

    m_albums.remove(albumId);
    emit albumRemoved(albumId);
    emit dataChanged();
}

Album PhotoManager::getAlbum(const QString& albumId) const {
    return m_albums.value(albumId);
}

QList<Album> PhotoManager::getAllAlbums() const {
    return m_albums.values();
}

QList<Album> PhotoManager::getRootAlbums(const QString& userId) const {
    QList<Album> result;
    for (const Album& album : m_albums) {
        if (album.isRoot() && album.userId() == userId) {
            result.append(album);
        }
    }
    return result;
}

QList<Album> PhotoManager::getChildAlbums(const QString& parentId) const {
    QList<Album> result;
    for (const Album& album : m_albums) {
        if (album.parentId() == parentId) {
            result.append(album);
        }
    }
    return result;
}

QList<Photo> PhotoManager::getPhotosRecursive(const QString& albumId) const {
    QList<Photo> result = getPhotosByAlbum(albumId);

    if (m_albums.contains(albumId)) {
        Album album = m_albums[albumId];
        for (const QString& childId : album.childAlbumIds()) {
            result.append(getPhotosRecursive(childId));
        }
    }

    return result;
}

void PhotoManager::addTag(const Tag& tag) {
    m_tags[tag.id()] = tag;
    emit tagAdded(tag);
    emit dataChanged();
}

void PhotoManager::updateTag(const Tag& tag) {
    if (m_tags.contains(tag.id())) {
        m_tags[tag.id()] = tag;
        emit tagUpdated(tag);
        emit dataChanged();
    }
}

void PhotoManager::removeTag(const QString& tagId) {
    if (m_tags.contains(tagId)) {
        for (auto it = m_photos.begin(); it != m_photos.end(); ++it) {
            it.value().removeTag(tagId);
        }

        m_tags.remove(tagId);
        emit tagRemoved(tagId);
        emit dataChanged();
    }
}

Tag PhotoManager::getTag(const QString& tagId) const {
    return m_tags.value(tagId);
}

QList<Tag> PhotoManager::getAllTags() const {
    return m_tags.values();
}

QMap<QString, Tag> PhotoManager::getTagsMap() const {
    return m_tags;
}

void PhotoManager::setSearchStrategy(std::shared_ptr<SearchStrategy> strategy) {
    m_searchStrategy = strategy;
}

QList<Photo> PhotoManager::search(const QString& query) const {
    if (m_searchStrategy) {
        return m_searchStrategy->search(m_photos.values(), query);
    }
    return QList<Photo>();
}

void PhotoManager::setPhotos(const QList<Photo>& photos) {
    m_photos.clear();
    for (const Photo& photo : photos) {
        m_photos[photo.id()] = photo;
    }
    emit dataChanged();
}

void PhotoManager::setAlbums(const QList<Album>& albums) {
    m_albums.clear();
    for (const Album& album : albums) {
        m_albums[album.id()] = album;
    }
    emit dataChanged();
}

void PhotoManager::setTags(const QList<Tag>& tags) {
    m_tags.clear();
    for (const Tag& tag : tags) {
        m_tags[tag.id()] = tag;
    }
    emit dataChanged();
}
