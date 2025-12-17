#include "tagsdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QWhatsThis>

TagsDialog::TagsDialog(PhotoManager* photoManager, QWidget* parent)
    : QDialog(parent)
    , m_photoManager(photoManager)
{
    setupUI();
    refreshTagsList();
}

void TagsDialog::setupUI() {
    setWindowTitle("Управление тегами");
    resize(400, 350);
    setWhatsThis("Управление тегами позволяет создавать и удалять теги.\n\n"
                 "Теги используются для категоризации фотографий и альбомов, "
                 "а также для поиска по ним.");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("Список тегов:");
    mainLayout->addWidget(titleLabel);

    m_tagsList = new QListWidget();
    m_tagsList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tagsList->setWhatsThis("Список всех созданных тегов.\n"
                             "Выберите тег для удаления.");
    connect(m_tagsList, &QListWidget::itemSelectionChanged, this, &TagsDialog::onTagSelectionChanged);
    mainLayout->addWidget(m_tagsList);

    QHBoxLayout* addLayout = new QHBoxLayout();
    m_tagNameEdit = new QLineEdit();
    m_tagNameEdit->setPlaceholderText("Название нового тега...");
    m_tagNameEdit->setWhatsThis("Введите название нового тега и нажмите Enter или кнопку \"Добавить\".");
    connect(m_tagNameEdit, &QLineEdit::returnPressed, this, &TagsDialog::onAddTag);
    addLayout->addWidget(m_tagNameEdit);

    m_addButton = new QPushButton("Добавить");
    m_addButton->setAutoDefault(false);
    m_addButton->setWhatsThis("Создать новый тег с введённым названием.");
    connect(m_addButton, &QPushButton::clicked, this, &TagsDialog::onAddTag);
    addLayout->addWidget(m_addButton);

    mainLayout->addLayout(addLayout);

    QHBoxLayout* buttonsLayout = new QHBoxLayout();

    m_deleteButton = new QPushButton("Удалить выбранный");
    m_deleteButton->setEnabled(false);
    m_deleteButton->setAutoDefault(false);
    m_deleteButton->setWhatsThis("Удалить выбранный тег.\nТег будет удалён со всех фотографий и альбомов.");
    connect(m_deleteButton, &QPushButton::clicked, this, &TagsDialog::onDeleteTag);
    buttonsLayout->addWidget(m_deleteButton);

    buttonsLayout->addStretch();

    QPushButton* closeButton = new QPushButton("Закрыть");
    closeButton->setAutoDefault(false);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    buttonsLayout->addWidget(closeButton);

    mainLayout->addLayout(buttonsLayout);
}

void TagsDialog::refreshTagsList() {
    m_tagsList->clear();

    QList<Tag> tags = m_photoManager->getAllTags();
    for (const Tag& tag : tags) {
        QListWidgetItem* item = new QListWidgetItem(tag.name());
        item->setData(Qt::UserRole, tag.id());
        m_tagsList->addItem(item);
    }

    m_deleteButton->setEnabled(false);
}

void TagsDialog::onAddTag() {
    QString tagName = m_tagNameEdit->text().trimmed();
    if (tagName.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Введите название тега.");
        return;
    }

    QList<Tag> tags = m_photoManager->getAllTags();
    for (const Tag& tag : tags) {
        if (tag.name().toLower() == tagName.toLower()) {
            QMessageBox::warning(this, "Внимание", "Тег с таким названием уже существует.");
            return;
        }
    }

    Tag tag;
    tag.setName(tagName);
    m_photoManager->addTag(tag);

    m_tagNameEdit->clear();
    refreshTagsList();
}

void TagsDialog::onDeleteTag() {
    QListWidgetItem* item = m_tagsList->currentItem();
    if (!item) return;

    QString tagId = item->data(Qt::UserRole).toString();
    QString tagName = item->text();

    int result = QMessageBox::question(
        this, "Подтверждение",
        QString("Удалить тег '%1'?\n\nТег будет удалён со всех фотографий.").arg(tagName),
        QMessageBox::Yes | QMessageBox::No
    );

    if (result == QMessageBox::Yes) {
        m_photoManager->removeTag(tagId);
        refreshTagsList();
    }
}

void TagsDialog::onTagSelectionChanged() {
    m_deleteButton->setEnabled(m_tagsList->currentItem() != nullptr);
}
