#!/bin/bash

set -ex
source pico_sdk_env.sh
for PROJECT in adxl343_i2c adxl343_spi blink; do
  echo "Building $PROJECT"
  pushd $PROJECT >/dev/null
  mkdir -p build && pushd build >/dev/null
  cmake -G "${PICO_CMAKE_GENERATOR}" -DPICO_BOARD=${PICO_BOARD} ..
  cmake --build . -- -j$(nproc)
  popd >/dev/null
  popd >/dev/null
done
