# Find SDL2, SDL2_image, SDL2_ttf, and SDL2_mixer packages
find_package(SDL2 REQUIRED)
include(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)
#PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)

# Include directories for SDL2, SDL2_image, SDL2_ttf, and SDL2_mixer
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS} $)
#{SDL2MIXER_INCLUDE_DIRS}
set(FDSOURCES
    src/FileDialog/tinyfiledialogs.c
    src/FileDialog/tinyfiledialogs.h
)

# Create the executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCESFILES} ${MYTINYGUISOURCES} ${FDSOURCES} ${TINYXMLSOURCES} ${IMGUI})

# Link SDL2, SDL2_image, SDL2_ttf, and SDL2_mixer libraries
target_link_libraries(${CMAKE_PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2TTF_LIBRARIES}) 
#${SDL2MIXER_LIBRARIES}