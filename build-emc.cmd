cls
rmdir /s /q emc
mkdir emc
mkdir emc\assets
xcopy /S /Q /C /Y assets .\emc\assets
cd emc\assets
del *.dll
cd ..
emcmake cmake -DPLATFORM=emscripten -DCMAKE_BUILD_TYPE=Release ..
emmake make -j 3
explorer .
cd ..