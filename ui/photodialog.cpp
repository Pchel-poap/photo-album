#include "photodialog.h"
#include "ui_photodialog.h"
#include <QFileInfo>
#include <QWhatsThis>

PhotoDialog::PhotoDialog(const Photo& photo, const QList<Tag>& tags, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::PhotoDialog)
    , m_photo(photo)
    , m_tags(tags)
{
    ui->setupUi(this);

    setWhatsThis("Редактирование информации о фотографии.\n\n"
                 "Здесь можно изменить описание, дату съёмки и теги фотографии.");
    ui->descriptionEdit->setWhatsThis("Описание фотографии.\n"
                                      "Используется при поиске по описанию.");
    ui->dateTimeEdit->setWhatsThis("Дата и время съёмки.\n"
                                   "Используется для хронологии и поиска по дате.\n"
                                   "Форматы поиска: ГГГГ-ММ-ДД, ГГГГ-ММ или ГГГГ.");
    ui->tagsListWidget->setWhatsThis("Выберите теги для фотографии.\n"
                                     "Теги помогают в поиске и категоризации.");

    connect(ui->saveButton, &QPushButton::clicked, this, &PhotoDialog::onSave);
    connect(ui->cancelButton, &QPushButton::clicked, this, &PhotoDialog::onCancel);

    loadPhotoData();
    loadTags();
}

PhotoDialog::~PhotoDialog() {
    delete ui;
}

void PhotoDialog::loadPhotoData() {
    QPixmap pixmap(m_photo.filePath());
    if (!pixmap.isNull()) {
        ui->photoPreview->setPixmap(pixmap.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    QFileInfo fileInfo(m_photo.filePath());
    ui->fileNameLabel->setText(fileInfo.fileName());
    ui->descriptionEdit->setPlainText(m_photo.description());
    ui->dateTimeEdit->setDateTime(m_photo.dateTime());
}

void PhotoDialog::loadTags() {
    ui->tagsListWidget->clear();

    for (const Tag& tag : m_tags) {
        QListWidgetItem* item = new QListWidgetItem(tag.name(), ui->tagsListWidget);
        item->setData(Qt::UserRole, tag.id());
        item->setCheckState(m_photo.hasTag(tag.id()) ? Qt::Checked : Qt::Unchecked);
    }
}

Photo PhotoDialog::photo() const {
    return m_photo;
}

void PhotoDialog::onSave() {
    m_photo.setDescription(ui->descriptionEdit->toPlainText());
    m_photo.setDateTime(ui->dateTimeEdit->dateTime());

    m_photo.clearTags();
    for (int i = 0; i < ui->tagsListWidget->count(); ++i) {
        QListWidgetItem* item = ui->tagsListWidget->item(i);
        if (item->checkState() == Qt::Checked) {
            m_photo.addTag(item->data(Qt::UserRole).toString());
        }
    }

    accept();
}

void PhotoDialog::onCancel() {
    reject();
}
