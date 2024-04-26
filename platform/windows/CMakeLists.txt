# Set the path to the SDL2 installation
set(SDL2_DIR "C:/SDL2-2.24.0")

set(SDL2_IMAGE_DIR "C:/SDL2_image-2.6.2")

set(SDL2_TTF_DIR "C:/SDL2_ttf-2.20.1")

set(SDL2_MIXER_DIR "C:/SDL2_mixer-2.6.2")

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

# Find FreeType Libraries
#find_package(Freetype REQUIRED)

#SET(CMAKE_CXX_FLAGS "-I C:/Users/Mohamed/Documents/freetype-VER-2-6-5/include -L C:/Users/Mohamed/Documents/freetype-VER-2-6-5/lib -lfreetype")

set(FDSOURCES
    src/FileDialog/tinyfiledialogs.c
    src/FileDialog/tinyfiledialogs.h
)

# Include directories
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    #${FREETYPE_INCLUDE_DIRS}
)

# Add the source files
add_executable(${CMAKE_PROJECT_NAME} ${SOURCESFILES} ${MYTINYGUISOURCES} ${FDSOURCES} ${TINYXMLSOURCES} ${IMGUI})

# Link libraries
target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    
    #${FREETYPE_LIBRARIES}
)
