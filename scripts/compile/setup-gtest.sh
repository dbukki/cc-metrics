mkdir gtest
cp -R /usr/src/googletest/* ./gtest

cd gtest
mkdir build
cd build

cmake .. -DCMAKE_INSTALL_PREFIX=/usr/bin/gtest
make install -j $(nproc)
