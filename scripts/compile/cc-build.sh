# For all supported OS versions:
export GTEST_ROOT="/usr/bin/gtest"
export PATH="/usr/bin/thrift/bin:${PATH}"
export PATH="/usr/bin/odb/bin:${PATH}"

# Create build directory.
cd "${HOME}/CodeCompass"
mkdir -p "build"
cd "build"

# Run CMake
cmake .. \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
  -DCMAKE_INSTALL_PREFIX="${HOME}/CodeCompassInstall" \
  -DDATABASE='sqlite' \
  -DTEST_DB='sqlite:database=ccdb.sqlite' \
  -DCMAKE_BUILD_TYPE='Debug' \
  -DLLVM_DIR="/usr/lib/llvm-11/cmake" \
  -DClang_DIR="/usr/lib/cmake/clang-11"

# -DDATABASE=sqlite|pgsql
# To specify linker for building CodeCompass use
#   -DCODECOMPASS_LINKER=<path_to_linker>

# Build project.
make -j $(nproc)

# Copy files to install directory.
make install

