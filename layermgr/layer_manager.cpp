
#include "layer_manager.h"

#include <QApplication>
#include <QBoxLayout>
#include <QFile>

int main(int argc, char **argv)
{
    QCoreApplication::setOrganizationName("LunarG");
    QCoreApplication::setOrganizationDomain("lunarg.com");
    QCoreApplication::setApplicationName("Vulkan Layer Manager");

    QApplication app(argc, argv);
    LayerManager manager;
    manager.show();
    return app.exec();
}

LayerManager::LayerManager()
{
    // Build the GUI
    //setWindowIcon(QIcon(":/layertool/icons/explicit.png"));
    setWindowTitle(tr("Vulkan Layer Manager"));

    QWidget *central_widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    outer_split = new QSplitter(Qt::Horizontal);
    inner_split = new QSplitter(Qt::Vertical);

    locations = new LayerLocationsWidget();
    inner_split->addWidget(locations);
    active_layers = new ActiveLayersWidget();
    inner_split->addWidget(active_layers);
    outer_split->addWidget(inner_split);
    layer_settings = new LayerSettingsWidget();
    outer_split->addWidget(layer_settings);

    connect(locations, &LayerLocationsWidget::pathsChanged, active_layers, &ActiveLayersWidget::updateAvailableLayers);
    connect(active_layers, &ActiveLayersWidget::enabledLayersUpdated, layer_settings, &LayerSettingsWidget::updateAvailableLayers);
    layout->addWidget(outer_split, 1);

    QHBoxLayout *button_layout = new QHBoxLayout();
    QPushButton *save_button = new QPushButton(tr("Save"));
    button_layout->addWidget(save_button, 0);
    QPushButton *reset_button = new QPushButton(tr("Reset"));
    button_layout->addWidget(reset_button, 0);

    button_layout->addStretch(1);
    QPushButton *exit_button = new QPushButton(tr("Exit"));
    button_layout->addWidget(exit_button, 0);

    connect(save_button, &QPushButton::clicked, this, &LayerManager::saveAll);
    connect(reset_button, &QPushButton::clicked, this, &LayerManager::resetAll);
    connect(exit_button, &QPushButton::clicked, this, &LayerManager::close);
    layout->addLayout(button_layout, 0);

    central_widget->setLayout(layout);
    setCentralWidget(central_widget);

    // Restore the state from the settings and the override settings
    QList<QPair<QString, LayerType>> custom_paths;
    int length = settings.beginReadArray("LayerPaths");
    for (int i = 0; i < length; ++i) {
        settings.setArrayIndex(i);
        QString location = settings.value("Path").toString();
        LayerType type = LayerEnum(settings.value("Type").toString());
        custom_paths.append(QPair<QString, LayerType>(location, type));
    }
    settings.endArray();
    locations->setCustomLayerPaths(custom_paths);
    locations->setUseCustomLayerPaths(settings.value("UseCustomPaths").toBool());

    if (settings.contains("ExpirationValue") && settings.contains("ExpirationUnit")) {
        active_layers->setExpiration(settings.value("ExpirationValue").toInt(), (DurationUnit) settings.value("ExpirationUnit").toInt());
    }
    active_layers->setEnabledLayers(override_settings.Layers());

    layer_settings->setSettingsValues(override_settings.LayerSettings());
    // TODO: Restore the active layer

    inner_split->restoreState(settings.value("InnerSplitState").toByteArray());
    outer_split->restoreState(settings.value("OuterSplitState").toByteArray());
    restoreGeometry(settings.value("WindowGeometry").toByteArray());
}

void LayerManager::closeEvent(QCloseEvent *event)
{
    // The settings are saved here, but the override settings are only saved when the user clicks "Save"
    settings.setValue("UseCustomPaths", locations->useCustomLayerPaths());

    settings.remove("LayerPaths");
    settings.beginWriteArray("LayerPaths");
    int i = 0;
    for (const QPair<QString, LayerType> &pair : locations->customLayerPaths()) {
        settings.setArrayIndex(i++);
        settings.setValue("Type", LayerString(pair.second));
        settings.setValue("Path", pair.first);
    }
    settings.endArray();

    settings.setValue("ExpirationValue", active_layers->expiration());
    settings.setValue("ExpirationUnit", (int) active_layers->expirationUnit());

    settings.setValue("InnerSplitState", inner_split->saveState());
    settings.setValue("OuterSplitState", outer_split->saveState());
    settings.setValue("WindowGeometry", saveGeometry());
}

void LayerManager::resetAll()
{
    active_layers->clearLayers();
}

void LayerManager::saveAll()
{
    // The override settings are saved here, but the regular settings are only saved when we close
    QList<QPair<QString, LayerType>> paths;
    if (locations->useCustomLayerPaths()) {
        paths = locations->customLayerPaths();
    }
    const QList<LayerManifest> &layers = active_layers->enabledLayers();
    override_settings.SaveLayers(paths, layers, active_layers->expiration());

    const QHash<QString, QHash<QString, LayerValue>> &settings = layer_settings->settings();
    override_settings.SaveSettings(settings);
}
