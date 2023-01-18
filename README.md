# Vulkan Ecosystem Tools

The repository contains the following Vulkan Tools:
- [Vulkan Configurator](vkconfig/README.md)
- [`VK_LAYER_LUNARG_api_dump`, `VK_LAYER_LUNARG_device_simulation`, `VK_LAYER_LUNARG_screenshot` and `VK_LAYER_LUNARG_monitor` layers](layersvt/README.md)
- [Vulkan Installation Analyzer](via/README.md)

These tools have binaries included within the [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/).

VkTrace and VkReplay have been deprecated and replaced by [gfxreconstruct](https://github.com/LunarG/gfxreconstruct).
Both VkTrace and VkReplay have been removed from VulkanTools and can now be found in the [vktrace](https://github.com/LunarG/vktrace) archive.
Both these tools are also no longer part of the [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/).

## Vulkan Tools for SC

This repository also contains the following tools to assist in Vulkan SC development:

- [`VK_LAYER_KHRONOS_json_gen`](layersvt/README.md)
- [Json Gen Layer Tests](jsonGenLayerTests/README.md)

**NOTE**: This repository is a downstream fork of [LunarG/VulkanTools](https://github.com/LunarG/VulkanTools)
which is periodically rebased.

Issues related to the `VK_LAYER_KHRONOS_json_gen` layer should be reported as issues in this repository.
All other issues should be reported to the upstream repository.

## CI Build Status
| Build Status |
|:------------|
| [![Build Status](https://github.com/KhronosGroup/VulkanTools-ForSC/actions/workflows/ci_build.yml/badge.svg?branch=sc_main)](https://github.com/KhronosGroup/VulkanTools-ForSC/actions) |

## Contributing

If you intend to contribute, the preferred work flow is for you to develop your contribution
in a fork of this repo in your GitHub account and then submit a pull request.
Please see the [CONTRIBUTING](CONTRIBUTING.md) file in this repository for more details

## How to Build and Run

[BUILD.md](BUILD.md) includes directions for building all the components and running the tests.

## Version Tagging Scheme

Updates to the `VulkanTools-ForSC` repository which correspond to a new Vulkan specification release are tagged using the following format: `v<`_`version`_`>` (e.g., `v1.1.96`).

**Note**: Marked version releases have undergone thorough testing but do not imply the same quality level as SDK tags. SDK tags follow the `sdk-<`_`version`_`>.<`_`patch`_`>` format (e.g., `sdk-1.1.92.0`).

This scheme was adopted following the 1.1.96 Vulkan specification release.

## License
This work is released as open source under a [Apache-style license](LICENSE.txt) from Khronos including a Khronos copyright.

## Acknowledgements
While this project has been developed primarily by LunarG, Inc., there are many other companies and individuals making this possible: Valve Corporation, funding project development.

The VK_LAYER_Khronos_json_gen layer has been contributed by NVIDIA CORPORATION.
