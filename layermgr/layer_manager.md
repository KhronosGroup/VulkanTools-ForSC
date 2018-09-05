
# Layer Manager

The Vulkan layer manager is an graphical application that allows a user to specify which layers will be loaded by Vulkan applications at runtime.
It provides an alternative to settings layers through environment variables or an applications layer selection.
In addition, it allows using layers from non-standard locations, selecting the ordering for implicit layers, and specifying settings for layers that the layer manager supports.

## Using the Layer Manager

The layer manager is a graphical user interface (GUI), and does not support any functionality through the system console. It may be launched from the console (as `vklayermanager`), but no further functionality will be available.

**Lenny's note:** I intend to add a note here about how to use the tool from the SDK/repo here, but I'm not going to write that until I actually know how that will be done.

The layer manager is divided into the following three sections.

### Layer Locations

At the top is the layer location selector.
By default, this list will display each of the standard system directories (or registries, on Windows) where a Vulkan application will search for layers.
Each location is labeled with either an "E", denoting that this location contains explicit layers, or an "I", denoting that this location contains implicit layers.
At the top of the layer locations selector is a checkbox labelled "Use custom layer paths".
When this checkbox is selected, the user gains the ability to choose the locations where layers will be searched.
The user can then add or remove paths with the "Add", "Remove", and "Clear" buttons on the right.
In addition, the "Search" button will allow the user to select a directory and the layer manager will scan that directory for json files that look like Vulkan layers, then prompt the user to ask if each path should actually be added.
This can be helpful when a user does not wish to manually find the exact location of layers.
When selecting layers manually, the selected layers will be saved each time the application is run, making it unnecessary to reset them every time.

### Active Layers

The bottom of the layer manager is the active layer selector.
This section allows a user to choose which layers are active for Vulkan applications, as well as selecting the order of those layers.
Any layers specified in this section will be active on any Vulkan application that is run on the current machine, with the current user.
The layer selector consists of three lists and a few other options.
The list on the left is the list of enabled layers.
The two lists on the right are the lists of disabled layers.
The top of those lists contains explicit layers, while the bottom contains implicit layers.
To enable a disabled layer, select it in one of the disabled layer lists and press the arrow button immediately to the left of that list.
The layer will be added to the bottom of the enabled layer list.
The layer can now be reordered by selecting it and pressing the up/down arrow buttons immediately to the left of the active layer list.
In addition, right below the up/down buttons, there is also a "Remove" button and a "Clear" button below that.
The "Remove" button will remove all selected layers from the enabled layer list, putting them back in the disabled layer lists.
The "Clear" button will disable all layers, regardless of selection.

There are two more tools at the top of the active layer selector: the expiration duration and the refresh button.
The refresh button is the simpler of these tools &mdash; it simply searches the paths specified by the layer locations widget for layers.
This can be useful when adding or removing layers in these locations while the layer manager is running, since the layer manager will not pick up these changes by default.
The expiration duration is a little more complex.
The expiration provides a mechanism so that the layer selections provided by the layer manager will expire (that is, they will stop having any effect) after a given time.
The prevents a user from setting specific layers as overrides and forgetting that these layers will still be enabled, days, months, or even years later.
The default expiration is 12 hours, meaning that all layer selections will no longer work 12 hours after the layer selections are saved.
Resaving layer selections does extend the expiration to 12 hours from the new save.
The expiration can be set to any number value, and the dropdown menu lets the user select an expiration in minutes, hours, or days.
In addition, the expiration can be disabled entirely by selecting "Never" from the dropdown.
Note that disabling the expiration entirely is not recommended as it makes it very easy to leave settings active and forget about them days or weeks into the future.
It's no fun debugging issues only to find that the wrong layers were enabled.

### Layer Settings

The pane on the right side is the layer settings selector.
Currently, the layer manager provides support for changing the settings of all LunarG layers, as well as other layers in standard validation.
Support for changing settings for other layers may be added in the future.

The panel provides a list of layers for which settings can be selected.
If the "Show enabled layers only" button is not checked, this will show all supported layers that are found.
If the checkbox is selected, the panel will only show settings for each layer that is currently in the enabled list from the active layers selector. A layer may be selected by clicking on it, to open the details for that layer.
Different layers will provide different settings, so it is impractical to give a list of settings for each layer, but a layer can have five types of settings:

* A true/false pairing &mdash; The user can select either true or false, but not both.
* A dropdown box &mdash; The user can select any one from a number of options.
* A multi-selection dropdown box &mdash; The user can select any number of options from a dropdown menu
* A test field &mdash; The user can enter any text
* A file &mdash; The user can enter any text as a filename, but a file selector button is present for convenience

**Lenny's note:** I haven't written the section on the criteria for getting outside layers to work with the tool, but that shouldn't be necessary for testing purposes (given that it requires support from the tool).

## Saving and Resetting

Finally, at the bottom of the layer manager, there are three buttons to control the tool: "Save", "Reset", and "Exit".
The "Save" button saves all changes that have been made in the tool.
No changes made in the layer manager will have any effect until the user hits that save button.
The "Reset" button does the opposite.
It discards all changes made since the last time the user saved.
The "Exit" button will quit the application (without saving).
It is effectively the same as pressing the "Reset" button and exiting the application.
Note that while all unsaved changes will be lost, all saved settings in the tool will be remembered the next time the layer manager is run.
