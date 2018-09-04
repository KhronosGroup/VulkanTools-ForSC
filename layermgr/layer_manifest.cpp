
#include "layer_manifest.h"

#include <QJsonArray>
#include <QJsonDocument>

#if defined(_WIN32)
#include <windows.h>

#define WIN_BUFFER_SIZE 1024
#endif

QString LayerManifest::PrettyName() const
{
    QList<QString> segments = name.split("_");
    if (segments.count() <= 3 || segments[0] != "VK" || segments[1] != "LAYER") {
        return name;
    }

    for (int i = 0; i < 2; ++i) {
        segments.removeFirst();
    }

    static const QHash<QString, QString> LAYER_AUTHORS = LoadLayerAuthors(":/layermgr/layer_info.json");
    if (LAYER_AUTHORS.contains(segments[0])) {
        segments[0] = LAYER_AUTHORS[segments[0]];
    }
    segments[0].append(":");

    for (int i = 0; i < segments.count(); ++i) {
        segments[i].replace(0, 1, segments[i][0].toUpper());
    }
    return segments.join(" ");
}

QList<LayerManifest> LayerManifest::LoadDirectory(const QDir &directory, LayerType type, bool recursive)
{
    QList<LayerManifest> manifests;
    if (!directory.exists()) {
        return manifests;
    }

    QList<QString> filters = { "*.json" };
    for (auto file_info : directory.entryInfoList(filters, QDir::Files | QDir::Readable)) {
        LoadLayerFile(file_info.absoluteFilePath(), type, &manifests);
    }
    if (recursive) {
        for (auto dir_info : directory.entryInfoList(filters, QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Readable)) {
            manifests.append(LoadDirectory(QDir(dir_info.absoluteFilePath()), type, true));
        }
    }

    return manifests;
}

#if defined(_WIN32)
QList<LayerManifest> LayerManifest::LoadRegistry(const QString& path, LayerType type)
{
    QList<LayerManifest> manifests;

    QString root_string = path.section('\\', 0, 0);
    static QHash<QString, HKEY> root_keys = {
        { "HKEY_CLASSES_ROOT", HKEY_CLASSES_ROOT },
        { "HKEY_CURRENT_CONFIG", HKEY_CURRENT_CONFIG },
        { "HKEY_CURRENT_USER", HKEY_CURRENT_USER },
        { "HKEY_LOCAL_MACHINE", HKEY_LOCAL_MACHINE },
        { "HKEY_USERS", HKEY_USERS },
    };
    HKEY root = HKEY_CURRENT_USER;
    for (auto label : root_keys.keys()) {
        if (label == root_string) {
            root = root_keys[label];
            break;
        }
    }

    HKEY key;
    QByteArray key_bytes = path.section('\\', 1).toLocal8Bit();
    LSTATUS err = RegCreateKeyEx(root, key_bytes.data(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &key, NULL);
    if (err != ERROR_SUCCESS) {
        return manifests;
    }

    DWORD value_count;
    RegQueryInfoKey(key, NULL, NULL, NULL, NULL, NULL, NULL, &value_count, NULL, NULL, NULL, NULL);
    for (DWORD i = 0; i < value_count; ++i) {
        TCHAR file_path[WIN_BUFFER_SIZE];
        DWORD buff_size = WIN_BUFFER_SIZE;
        RegEnumValue(key, i, file_path, &buff_size, NULL, NULL, NULL, NULL);

        LoadLayerFile(file_path, type, &manifests);
    }

    RegCloseKey(key);
    return manifests;
}
#endif

QHash<QString, QString> LayerManifest::LoadLayerAuthors(const QString &file_path)
{
    QFile file(file_path);
    file.open(QFile::ReadOnly);
    QString data = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(data.toLocal8Bit());
    if (!document.isObject()) {
        return QHash<QString, QString>();
    }
    QJsonObject root = document.object();
    if (!root.value("layer_authors").isObject()) {
        return QHash<QString, QString>();
    }
    QJsonObject author_object = root.value("layer_authors").toObject();

    QHash<QString, QString> authors;
    for(QString &key : author_object.keys()) {
        if (author_object[key].isObject() && author_object[key].toObject()["name"].isString()) {
            authors[key] = author_object[key].toObject()["name"].toString();
        }
    }
    return authors;
}

void LayerManifest::LoadLayerFile(const QString& file_path, LayerType type, QList<LayerManifest> *manifest_list)
{
    QFile file(file_path);
    QFileInfo file_info(file);
    file.open(QFile::ReadOnly);
    QString data = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(data.toLocal8Bit());
    if (!document.isObject()) {
        return;
    }
    QJsonObject root = document.object();

    if (root.value("layer").isObject()) {
        LoadLayerObject(root.value("layer").toObject(), type, file_info, manifest_list);
    }

    if (root.value("layers").isArray()) {
        QJsonArray layer_array = root.value("layers").toArray();
        for (auto layer : layer_array) {
            if (layer.isObject()) {
                LoadLayerObject(layer.toObject(), type, file_info, manifest_list);
            }
        }
    }
}

void LayerManifest::LoadLayerObject(const QJsonObject &layer_object, LayerType type, const QFileInfo &file, QList<LayerManifest> *manifest_list)
{
    QJsonValue name = layer_object.value("name");
    QJsonValue description = layer_object.value("description");
    QJsonValue library_path = layer_object.value("library_path");
    QJsonValue component_layers = layer_object.value("component_layers");
    if (name.isString() && description.isString() && (library_path.isString() || component_layers.isArray())) {

        QFileInfo library(file.dir(), library_path.toString());
        if (library.exists() || component_layers.isArray()) {

            LayerManifest layer_manifest;
            layer_manifest.file_path = file.absoluteFilePath();
            layer_manifest.name = layer_object.value("name").toString();
            layer_manifest.description = layer_object.value("description").toString();
            layer_manifest.type = type;
            manifest_list->append(layer_manifest);
        }
    }
}

QList<LayerOption> LayerOption::LoadOptions(const LayerManifest &manifest)
{
    static const QJsonObject OPTIONS_JSON = LoadOptionJson(":/layermgr/layer_info.json");
    if (OPTIONS_JSON.empty()) {
        return QList<LayerOption>();
    }

    if (!OPTIONS_JSON.value(manifest.name).isObject()) {
        return QList<LayerOption>();
    }

    QList<LayerOption> options;
    const QJsonObject layer_info = OPTIONS_JSON.value(manifest.name).toObject();
    for (const QString &key : layer_info.keys()) {
        if (!layer_info.value(key).isObject()) {
            continue;
        }
        QJsonObject option_info = layer_info.value(key).toObject();

        LayerOption option;
        option.layer_name = manifest.name;
        option.name = key;

        if (option_info.value("name").isString()) {
            option.pretty_name = option_info.value("name").toString();
        } else {
            option.pretty_name = key;
        }

        if (option_info.value("description").isString()) {
            option.description = option_info.value("description").toString();
        }

        if (!option_info.value("type").isString()) {
            continue;
        }
        QString option_type = option_info.value("type").toString();
        QJsonValue default_value = option_info.value("default");

        if(option_type == "bool") {
            option.type = LayerOptionType::Bool;
            if (!default_value.isBool()) {
                continue;
            }
            option.default_values.insert(default_value.toBool() ? "True" : "False");

        } else if (option_type == "enum" || option_type == "multi_enum") {
            QJsonValue enum_options = option_info.value("options");
            if (enum_options.isArray()) {
                for (const QJsonValue &item : enum_options.toArray()) {
                    if (!item.isString()) {
                        continue;
                    }
                    option.enum_options.insert(item.toString(), item.toString());
                }
            } else if (enum_options.isObject()) {
                for (QString &key : enum_options.toObject().keys()) {
                    if (!enum_options.toObject().value(key).isString()) {
                        continue;
                    }
                    option.enum_options.insert(key, enum_options.toObject().value(key).toString());
                }
            } else {
                continue;
            }

            if (option_type == "enum") {
                option.type = LayerOptionType::Enum;
                if (!default_value.isString() || !option.enum_options.contains(default_value.toString())) {
                    continue;
                }
                option.default_values.insert(default_value.toString());
            } else {
                option.type = LayerOptionType::MultiEnum;
                if (!default_value.isArray()) {
                    continue;
                }
                for (const QJsonValue &item : default_value.toArray()) {
                    if (!item.isString() || !option.enum_options.contains(item.toString())) {
                        continue;
                    }
                    option.default_values.insert(item.toString());
                }
            }

        } else if (option_type == "open_file") {
            option.type = LayerOptionType::OpenFile;
            if (!default_value.isString()) {
                continue;
            }
            option.default_values.insert(default_value.toString());

        } else if (option_type == "save_file") {
            option.type = LayerOptionType::SaveFile;
            if (!default_value.isString()) {
                continue;
            }
            option.default_values.insert(default_value.toString());

        } else if (option_type == "string") {
            option.type = LayerOptionType::String;
            if(!default_value.isString()) {
                continue;
            }
            option.default_values.insert(default_value.toString());

        } else {
            continue;
        }

        options.append(option);
    }

    return options;
}

QJsonObject LayerOption::LoadOptionJson(const QString &file_path)
{
    QFile file(file_path);
    file.open(QFile::ReadOnly);
    QString data = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(data.toLocal8Bit());
    if (!document.isObject()) {
        return QJsonObject();
    }
    QJsonObject root = document.object();
    if (!root.value("layer_options").isObject()) {
        return QJsonObject();
    }
    return root.value("layer_options").toObject();
}
