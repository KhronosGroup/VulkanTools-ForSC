
#pragma once

#include <QEvent>
#include <QMainWindow>
#include <QSettings>
#include <QSplitter>

#include "active_layers_widget.h"
#include "layer_locations_widget.h"
#include "layer_settings_widget.h"
#include "override_settings.h"

class LayerManager : public QMainWindow
{
    Q_OBJECT

public:
    LayerManager();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void resetAll();
    void saveAll();

private:
    QSettings settings;
    OverrideSettings override_settings;

    QSplitter *outer_split;
    QSplitter *inner_split;
    LayerLocationsWidget *locations;
    ActiveLayersWidget *active_layers;
    LayerSettingsWidget *layer_settings;
};
