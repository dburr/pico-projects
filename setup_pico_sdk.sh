#!/usr/bin/env bash
# =============================================================================
# setup_pico_sdk.sh — Download and install the Raspberry Pi Pico SDK
# for use in a Jenkins build workflow.
#
# Usage:
#   ./setup_pico_sdk.sh [SDK_VERSION]
#
#   SDK_VERSION  Optional. A git tag (e.g. "2.1.0") or "master" for latest
#                stable. Defaults to "master".
#
# The script:
#   1. Installs required build dependencies via pacman (idempotent).
#   2. Clones (or updates) the pico-sdk into PICO_INSTALL_DIR.
#   3. Initialises all submodules.
#   4. Writes a pico_sdk_env.sh file that Jenkins can source to expose
#      PICO_SDK_PATH and other variables to subsequent build steps.
#
# Jenkins pipeline usage — Declarative:
#
#   stage('Setup Pico SDK') {
#       steps {
#           sh 'bash setup_pico_sdk.sh 2.1.0'
#       }
#   }
#   stage('Build') {
#       steps {
#           // Source the env file so cmake can find the SDK
#           sh '''
#               source pico_sdk_env.sh
#               mkdir -p build && cd build
#               cmake ..
#               make -j$(nproc)
#           '''
#       }
#   }
#
# Jenkins pipeline usage — Scripted (withEnv):
#
#   def picoEnv = readFile('pico_sdk_env.sh')
#       .readLines()
#       .findAll { it.startsWith('export ') }
#       .collect { it.replaceFirst('export ', '') }
#   withEnv(picoEnv) {
#       sh 'cmake ...'
#   }
# =============================================================================

set -euo pipefail

# ---------------------------------------------------------------------------
# Configuration — override any of these via environment variables before
# calling the script, or edit the defaults here.
# ---------------------------------------------------------------------------

# Where to install the SDK on the Jenkins agent / build node.
# Using WORKSPACE keeps it inside the job directory for easy cleanup.
# Change to a shared path (e.g. /opt/pico-sdk) to cache across jobs.
PICO_INSTALL_DIR="${PICO_INSTALL_DIR:-${WORKSPACE:-$(pwd)}/pico-sdk}"

# SDK version: a git tag such as "2.1.0", "2.0.0", or "master" for HEAD.
SDK_VERSION="${1:-${PICO_SDK_VERSION:-master}}"

# Output file that downstream steps source to pick up env vars.
ENV_FILE="${ENV_FILE:-pico_sdk_env.sh}"

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

log() { echo "[pico-sdk-setup] $*"; }

# ---------------------------------------------------------------------------
# 1. Dependency check / install (Arch Linux — pacman)
# ---------------------------------------------------------------------------

# Arch package names for the Pico toolchain:
#   arm-none-eabi-gcc      — ARM cross-compiler (community repo)
#   arm-none-eabi-newlib   — C runtime / newlib for bare-metal ARM
#   cmake, git, python, make — standard build tools
#
# All packages are installed with --needed so pacman is a no-op when they
# are already present, keeping the step idempotent.

PACMAN_PACKAGES=(
    git
    cmake
    python
    make
    arm-none-eabi-gcc
    arm-none-eabi-newlib
)

log "Checking build dependencies..."

MISSING_PKGS=()
for pkg in "${PACMAN_PACKAGES[@]}"; do
    pacman -Qi "${pkg}" &>/dev/null || MISSING_PKGS+=("${pkg}")
done

if [[ ${#MISSING_PKGS[@]} -gt 0 ]]; then
    log "Installing missing packages via pacman: ${MISSING_PKGS[*]}"
    sudo pacman -Sy --noconfirm --needed "${MISSING_PKGS[@]}"
else
    log "All dependencies already installed."
fi

# ---------------------------------------------------------------------------
# 2. Clone or update the pico-sdk repository
# ---------------------------------------------------------------------------

PICO_SDK_URL="https://github.com/raspberrypi/pico-sdk.git"

if [[ -d "${PICO_INSTALL_DIR}/.git" ]]; then
    log "pico-sdk already cloned at ${PICO_INSTALL_DIR}. Fetching updates..."
    git -C "${PICO_INSTALL_DIR}" fetch --tags --prune origin
    if [[ "${SDK_VERSION}" == "master" ]]; then
        git -C "${PICO_INSTALL_DIR}" checkout master
        git -C "${PICO_INSTALL_DIR}" pull --ff-only origin master
    else
        git -C "${PICO_INSTALL_DIR}" checkout "tags/${SDK_VERSION}" \
            --force 2>/dev/null || git -C "${PICO_INSTALL_DIR}" checkout "${SDK_VERSION}" --force
    fi
else
    log "Cloning pico-sdk (version: ${SDK_VERSION}) into ${PICO_INSTALL_DIR}..."
    mkdir -p "$(dirname "${PICO_INSTALL_DIR}")"
    if [[ "${SDK_VERSION}" == "master" ]]; then
        git clone --depth 1 "${PICO_SDK_URL}" "${PICO_INSTALL_DIR}"
    else
        git clone --branch "${SDK_VERSION}" --depth 1 "${PICO_SDK_URL}" "${PICO_INSTALL_DIR}"
    fi
fi

# ---------------------------------------------------------------------------
# 3. Initialise submodules (tinyusb, mbedtls, etc.)
# ---------------------------------------------------------------------------

log "Initialising submodules..."
git -C "${PICO_INSTALL_DIR}" submodule update --init --recursive

# ---------------------------------------------------------------------------
# 4. Resolve absolute path and actual SDK version/commit
# ---------------------------------------------------------------------------

PICO_SDK_PATH="$(realpath "${PICO_INSTALL_DIR}")"
PICO_SDK_COMMIT="$(git -C "${PICO_SDK_PATH}" rev-parse --short HEAD)"
PICO_SDK_TAG="$(git -C "${PICO_SDK_PATH}" describe --tags --exact-match 2>/dev/null || echo "${SDK_VERSION}")"
ARM_TOOLCHAIN_PATH="$(dirname "$(command -v arm-none-eabi-gcc)")"

log "SDK path   : ${PICO_SDK_PATH}"
log "SDK version: ${PICO_SDK_TAG} (${PICO_SDK_COMMIT})"
log "Toolchain  : ${ARM_TOOLCHAIN_PATH}"

# ---------------------------------------------------------------------------
# 5. Write the environment file for downstream Jenkins steps
# ---------------------------------------------------------------------------

log "Writing environment file: ${ENV_FILE}"

cat > "${ENV_FILE}" <<EOF
# Auto-generated by setup_pico_sdk.sh — do not edit manually.
# Source this file in subsequent build steps:
#   source pico_sdk_env.sh

# Core SDK path — required by CMake / pico_sdk_import.cmake
export PICO_SDK_PATH="${PICO_SDK_PATH}"

# ARM toolchain directory (added to PATH so cmake finds arm-none-eabi-gcc)
export PICO_TOOLCHAIN_PATH="${ARM_TOOLCHAIN_PATH}"
export PATH="${ARM_TOOLCHAIN_PATH}:\${PATH}"

# Target board — override in your pipeline if not building for the base Pico.
# Common values: pico, pico_w, pico2, adafruit_feather_rp2040
export PICO_BOARD="${PICO_BOARD:-pico}"

# CMake generator hint (Ninja is faster if available; falls back to make)
if command -v ninja >/dev/null 2>&1; then
    export PICO_CMAKE_GENERATOR="Ninja"
else
    export PICO_CMAKE_GENERATOR="Unix Makefiles"
fi

# Informational — captured for build artefact metadata / reproducibility
export PICO_SDK_VERSION="${PICO_SDK_TAG}"
export PICO_SDK_COMMIT="${PICO_SDK_COMMIT}"
EOF

log "Environment file written."

# ---------------------------------------------------------------------------
# 6. Validate the install
# ---------------------------------------------------------------------------

log "Validating install..."

INIT_CMAKE="${PICO_SDK_PATH}/pico_sdk_init.cmake"
if [[ ! -f "${INIT_CMAKE}" ]]; then
    log "ERROR: ${INIT_CMAKE} not found. SDK clone may be incomplete."
    exit 1
fi

arm-none-eabi-gcc --version | head -1 | sed "s/^/[pico-sdk-setup] Toolchain: /"

log "Pico SDK setup complete."
log ""
log "To use in your next build step:"
log "  source ${ENV_FILE}"
log "  mkdir -p build && cd build"
log "  cmake -G \"\${PICO_CMAKE_GENERATOR}\" -DPICO_BOARD=\${PICO_BOARD} .."
log "  cmake --build . -- -j\$(nproc)"
