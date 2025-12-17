#include "albumdialog.h"
#include "ui_albumdialog.h"
#include <QMessageBox>
#include <QWhatsThis>

AlbumDialog::AlbumDialog(const Album& album, const QList<Tag>& tags, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::AlbumDialog)
    , m_album(album)
    , m_tags(tags)
{
    ui->setupUi(this);

    setWhatsThis("Создание или редактирование альбома.\n\n"
                 "Альбомы позволяют организовывать фотографии в коллекции. "
                 "Можно создавать вложенные альбомы (подальбомы).");
    ui->nameEdit->setWhatsThis("Название альбома. Обязательное поле.");
    ui->descriptionEdit->setWhatsThis("Описание альбома. Необязательное поле для заметок.");
    ui->tagsListWidget->setWhatsThis("Выберите теги для альбома.\n"
                                     "Теги помогают в поиске и категоризации.");

    connect(ui->saveButton, &QPushButton::clicked, this, &AlbumDialog::onSave);
    connect(ui->cancelButton, &QPushButton::clicked, this, &AlbumDialog::onCancel);

    loadAlbumData();
    loadTags();
}

AlbumDialog::~AlbumDialog() {
    delete ui;
}

void AlbumDialog::loadAlbumData() {
    ui->nameEdit->setText(m_album.name());
    ui->descriptionEdit->setPlainText(m_album.description());
}

void AlbumDialog::loadTags() {
    ui->tagsListWidget->clear();

    for (const Tag& tag : m_tags) {
        QListWidgetItem* item = new QListWidgetItem(tag.name(), ui->tagsListWidget);
        item->setData(Qt::UserRole, tag.id());
        item->setCheckState(m_album.hasTag(tag.id()) ? Qt::Checked : Qt::Unchecked);
    }
}

Album AlbumDialog::album() const {
    return m_album;
}

void AlbumDialog::onSave() {
    QString name = ui->nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Введите название альбома.");
        return;
    }

    m_album.setName(name);
    m_album.setDescription(ui->descriptionEdit->toPlainText());

    for (int i = 0; i < ui->tagsListWidget->count(); ++i) {
        QListWidgetItem* item = ui->tagsListWidget->item(i);
        QString tagId = item->data(Qt::UserRole).toString();
        if (item->checkState() == Qt::Checked) {
            m_album.addTag(tagId);
        } else {
            m_album.removeTag(tagId);
        }
    }

    accept();
}

void AlbumDialog::onCancel() {
    reject();
}
