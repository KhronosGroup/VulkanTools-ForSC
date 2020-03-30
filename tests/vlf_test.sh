#!/bin/bash

# vlf_test.sh
# This script will run the demo vulkaninfo with the demo_layer and capture the output.
# This layer will output all of the API names used by the application, and this script
# will search the output for a certain amount of APIs that are known to be used by this
# demo.  If the threshold is met, the test will indicate PASS, else FAILURE. This
# script requires a path to the Vulkan-Tools build directory so that it can locate
# vulkaninfo and the mock ICD. The path can be defined using the environment variable
# VULKAN_TOOLS_BUILD_DIR or using the command-line argument -t or --tools.
#
# The vulkan loader build dir can be specified using the -l or --loader command line arg.
# This might be needed if a loader is not installed on the system, or is out-of-date.

# Track unrecognized arguments.
UNRECOGNIZED=()

# Parse the command-line arguments.
while [[ $# -gt 0 ]]
do
   KEY="$1"
   case $KEY in
      -t|--tools)
      VULKAN_TOOLS_BUILD_DIR="$2"
      shift
      shift
      ;;
      -l|--loader)
      VULKAN_LOADER_BUILD_DIR="$2"
      shift
      shift
      ;;
      *)
      UNRECOGNIZED+=("$1")
      shift
      ;;
   esac
done

# Reject unrecognized arguments.
if [[ ${#UNRECOGNIZED[@]} -ne 0 ]]; then
   echo "ERROR: $0:$LINENO"
   ehco "Unrecognized command-line arguments: ${UNRECOGNIZED[*]}"
   exit 1
fi

if [ -z ${VULKAN_TOOLS_BUILD_DIR+x} ]; then
   echo "ERROR: $0:$LINENO"
   echo "Vulkan-Tools build directory is undefined."
   echo "Please set VULKAN_TOOLS_BUILD_DIR or use the -t|--tools <path> command line option."
   exit 1
fi

if [ ! -z ${VULKAN_LOADER_BUILD_DIR+x} ]; then
   echo 1@@@ $LD_LIBRARY_PATH
   export LD_LIBRARY_PATH="${VULKAN_LOADER_BUILD_DIR}/install/lib:${LD_LIBRARY_PATH}"
fi
echo 2@@@ $LD_LIBRARY_PATH

if [ -t 1 ] ; then
    RED='\033[0;31m'
    GREEN='\033[0;32m'
    NC='\033[0m' # No Color
else
    RED=''
    GREEN=''
    NC=''
fi

cd $(dirname "${BASH_SOURCE[0]}")

VULKANINFO="$VULKAN_TOOLS_BUILD_DIR/install/bin/vulkaninfo"

export VK_LAYER_PATH="../layers:../layersvt"
export VK_ICD_FILENAMES="$VULKAN_TOOLS_BUILD_DIR/icd/VkICD_mock_icd.json"
export VK_INSTANCE_LAYERS=VK_LAYER_LUNARG_demo_layer
(
echo 1============
set
echo 2============
echo /home/travis/build/LunarG/VulkanTools/Vulkan-Tools/*build
echo 3============
ls /home/travis/build/LunarG
echo 4============
ls /home/travis/build/LunarG/VulkanTools
echo 5============
ls /home/travis/build/LunarG/VulkanTools/dbuild/
echo 6============
ls /home/travis/build/LunarG/VulkanTools/Vulkan-Tools
echo 7============
ls /home/travis/build/LunarG/VulkanTools/Vulkan-Tools/dbuild
echo 8============
ls /home/travis/build/LunarG/VulkanTools/Vulkan-Tools/build
echo 9============
ls $VULKAN_TOOLS_BUILD_DIR/..
echo 10============
ls $VULKAN_TOOLS_BUILD_DIR
echo 11============
ls $VULKAN_LOADER_BUILD_DIR/..
echo 12============
ls $VULKAN_LOADER_BUILD_DIR
echo 13============
)
"$VULKANINFO" --show-formats > file.tmp

printf "$GREEN[ RUN      ]$NC $0\n"
if [ -f file.tmp ]
then
    count=$(grep vkGetPhysicalDeviceFormatProperties file.tmp | wc -l)
    if [ $count -gt 50 ]
    then
        printf "$GREEN[  PASSED  ]$NC $0\n"
    else
        tail -10 file.tmp
        printf "$RED[  FAILED  ]$NC $0\n"
        rm file.tmp
        exit 1
    fi
fi

rm file.tmp

exit 0
