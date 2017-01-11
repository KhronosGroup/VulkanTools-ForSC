#!/bin/bash

set -ev

if [[ $(uname) == "Linux" ]]; then
    cores=$(ncpus)
elif [[ $(uname) == "Darwin" ]]; then
    cores=$(sysctl -n hw.ncpu)
fi

#
# build layers
#
./update_external_sources_android.sh || exit 1
./android-generate.sh || exit 1
ndk-build -j $cores || exit 1

#
# create vkreplay APK
#
android update project -s -p . -t "android-23" || exit 1
ant -buildfile vkreplay debug || exit 1

#
# build cube-with-layers
#
(
pushd ../demos/android || exit 1
android update project -s -p . -t "android-23" || exit 1
ndk-build -j $cores || exit 1
ant -buildfile cube-with-layers debug || exit 1
popd
)

#
# build vktrace
#
(
pushd ..
./update_external_sources.sh -g -s || exit 1
mkdir -p build || exit 1
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_LOADER=Off -DBUILD_TESTS=Off -DBUILD_LAYERS=Off -DBUILD_VKTRACEVIEWER=Off -DBUILD_LAYERSVT=Off -DBUILD_DEMOS=Off -DBUILD_VKJSON=Off -DBUILD_VIA=Off -DBUILD_VKTRACE_LAYER=Off -DBUILD_VKTRACE_REPLAY=Off -DBUILD_VKTRACE=On .. || exit 1
make -j $cores vktrace || exit 1
popd
)

#
# build vktrace32
#
(
pushd ..
mkdir -p build32 || exit 1
cd build32
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_LOADER=Off -DBUILD_TESTS=Off -DBUILD_LAYERS=Off -DBUILD_VKTRACEVIEWER=Off -DBUILD_LAYERSVT=Off -DBUILD_DEMOS=Off -DBUILD_VKJSON=Off -DBUILD_VIA=Off -DBUILD_VKTRACE_LAYER=Off -DBUILD_VKTRACE_REPLAY=Off -DBUILD_X64=Off -DBUILD_VKTRACE=On .. || exit 1
make -j $cores vktrace || exit 1
popd
)

exit 0
