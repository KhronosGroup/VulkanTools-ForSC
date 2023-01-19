# JSON Generator Layer

## Overview

The JSON generator is a utility layer to dump the pipeline state from applications.

The pipeline state is serialized in a json file and the corresponding SPIR-V modules are also saved to file.
Once serialized, the pipeline state can be compiled offline by a Vulkan SC "Pipeline Cache Compiler" and used in a Vulkan SC
variant of the same application.

This layer implements the [VK_EXT_pipeline_properties](https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VK_EXT_pipeline_properties.html) Vulkan extension which can be used by the application
to query the identifier assigned to a particular pipeline.
This identifier can be stored with the application state and used to load the pipeline in the Vulkan SC application.

This layer uses generated code in [vulkan_json_data.hpp](./vulkan_json_data.hpp) to dump the structures in the JSON format.
To re-generate this file, please follow instructions in [here](https://github.com/KhronosGroup/VulkanSC-Docs/wiki/JSON-schema).

This layer also records information which can be used to create a `VkDeviceObjectReservationCreateInfo` for a Vulkan SC application.

### Supported features

- Dump entire pipeline state into an offline format - This consists of the JSON state and SPIR-V modules.
- Create a .hpp struct dump of `VkDeviceObjectReservationCreateInfo` that can be used with a Vulkan SC application.


### Enabling the layer

This layer needs to be enabled in a standard way a Khronos layer is enabled. More details on enabling the layer can be found 
[here](https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/LAYER_CONFIGURATION.md)

### Usage
This layer uses an environment variable called "VK_JSON_FILE_PATH", which is the location where the JSON/SPIR-V/hpp artifacts are dumped.

- Windows
 `Set this variable via the System Properties.`

- Linux
 `export VK_JSON_FILE_PATH=<path>`

### Tests
There are some unit tests provided with the layer to ensure that any changes made to the layer do not break existing functionality.
Documentation for the Json Generation Layer tests can be found in [jsonGenLayerTests/README.md](../jsonGenLayerTests/README.md).

### Known Issues

