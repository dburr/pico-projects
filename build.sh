#!/bin/bash

set -ex
source pico_sdk_env.sh
for PROJECT in adxl343_i2c adxl343_spi blink blink_w hid-test; do
  echo "Building $PROJECT"
  cd $PROJECT
  mkdir -p build && cd build
  cmake -G "${PICO_CMAKE_GENERATOR}" -DPICO_BOARD=${PICO_BOARD} ..
  cmake --build . -- -j$(nproc)
  cd ..
