# Set the path to the SDL2 installation
set(SDL2_DIR "C:/SDL2-2.24.0")

set(SDL2_IMAGE_DIR "C:/SDL2_image-2.6.2")
set(SDL2_image_DIR "C:/SDL2_image-2.6.2/cmake")

set(SDL2_TTF_DIR "C:/SDL2_ttf-2.20.1")
set(SDL2_ttf_DIR "C:/SDL2_ttf-2.20.1/cmake")

set(SDL2_MIXER_DIR "C:/SDL2_mixer-2.6.2")
set(SDL2_mixer_DIR "C:/SDL2_mixer-2.6.2/cmake")

# Set the Emscripten toolchain file
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/emscripten_toolchain.cmake")

list(APPEND CMAKE_PREFIX_PATH ${SDL2_DIR})
list(APPEND CMAKE_PREFIX_PATH ${SDL2_IMAGE_DIR})
list(APPEND CMAKE_PREFIX_PATH ${SDL2_TTF_DIR})
list(APPEND CMAKE_PREFIX_PATH ${SDL2_MIXER_DIR})

# Find SDL2
find_package(SDL2 REQUIRED)

# Find SDL2_image
find_package(SDL2_image REQUIRED)

# Find SDL2_ttf
find_package(SDL2_ttf REQUIRED)

# Find SDL2_mixer
find_package(SDL2_mixer REQUIRED)

# Include directories
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
)

# Set compilation flags for Emscripten
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -lSDL -s USE_SDL_IMAGE=2 -s ALLOW_MEMORY_GROWTH=1 --use-preload-plugins -s SDL2_IMAGE_FORMATS=['png'] -s GL_UNSAFE_OPTS=0 -s USE_SDL_TTF=2 -s USE_FREETYPE=1 -s EXPORTED_FUNCTIONS=['_main',_malloc,_free] -s EXPORTED_RUNTIME_METHODS=['ccall'] --preload-file assets/. --shell-file assets/index.html")


set(UPLOADLIB src/FileDialog/emscripten_browser_file.h)

# Add the source files
add_executable(${CMAKE_PROJECT_NAME} ${SOURCESFILES} ${MYTINYGUISOURCES} ${TINYXMLSOURCES} ${IMGUI} ${UPLOADLIB})

# Link libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
)

# Set the output format to WebAssembly
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    SUFFIX ".html"
)