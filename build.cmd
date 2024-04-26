cls
rmdir /s /q win
mkdir win
xcopy /S /Q /C /Y assets .\win
cd win
cmake -G="MinGW Makefiles" -DPLATFORM=windows -DCMAKE_BUILD_TYPE=Release ..
mingw32-make -j 3
#sexplorer .
cd .. 