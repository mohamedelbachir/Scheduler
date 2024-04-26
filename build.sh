clear
rm -rf linux
mkdir linux
cp -R -f assets/. ./linux
cd linux
rm *.dll
cmake -DPLATFORM=linux -DCMAKE_BUILD_TYPE=Release ..
echo "next"
make -j 6
