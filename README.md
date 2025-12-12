# Pi Pico projects

## Setting up a build environment

### On Mac

You must install [Homebrew](https://brew.sh/).

Install the following packages through Homebrew:

* Cmake: `brew install cmake`
* ARM toolchain:
  * ~~`brew tap ArmMbed/homebrew-formulae`~~
  * ~~`brew install arm-none-eabi-gcc`~~
  * `brew install --cask gcc-arm-embedded`

### On Linux (Debian/Ubuntu)

`$ sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential`

## Setting up the SDK

The SDK is set up as submodules to this project. To get everything set up, simply run
`git submodule update --init  --recursive`.

## Updating the SDK

To update all SDK components to the latest commit, run:

```shell
$ git submodule update --remote --merge
$ git add .
$ git commit -m "Update RP2040 SDK"
$ git push
```
