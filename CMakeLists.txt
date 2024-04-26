cmake_minimum_required(VERSION 3.10)
project(Scheduler)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#global source files
set(SOURCESFILES 
    src/main.cpp
    src/MainApp.cpp
    src/MainApp.h
    src/parseAppData.h
    src/ParsingFile.cpp
    src/ParsingFile.h
    src/ScheduleAlgorithm.cpp
    src/ScheduleFileSaving.cpp
    src/Utils.h
    src/utility.h

    #screen
    src/Screens/DisplayGranttState.cpp
    src/Screens/ScheduleItem.cpp
    src/Screens/ScheduleItem.h
    src/Screens/AboutState.cpp
    src/Screens/AboutState.h
    src/Screens/MainAppState.cpp
    src/Screens/MainAppState.h
    src/Screens/SplashScreen.cpp
    src/Screens/SplashScreen.h
    src/Screens/TaskState.cpp
    src/Screens/TaskState.h
)

set(IMGUI 
    src/imgui/imconfig.h
    src/imgui/imgui.cpp
    src/imgui/imgui.h
    src/imgui/imgui_demo.cpp
    src/imgui/imgui_draw.cpp
    src/imgui/imgui_impl_sdl.cpp
    src/imgui/imgui_impl_sdl.h
    src/imgui/imgui_impl_sdlrenderer.cpp
    src/imgui/imgui_impl_sdlrenderer.h
    src/imgui/imgui_internal.h
    src/imgui/imgui_tables.cpp
    src/imgui/imgui_widgets.cpp
    src/imgui/imstb_rectpack.h
    src/imgui/imstb_textedit.h
    src/imgui/imstb_truetype.h

    # weird FT_Memory_Face not found on Windows
    #src/imgui/misc/cpp/imgui_stdlib.cpp
    #src/imgui/misc/cpp/imgui_stdlib.h
    #src/imgui/misc/freetype/imgui_freetype.cpp
    #src/imgui/misc/freetype/imgui_freetype.h

    src/imgui/implot.cpp
    src/imgui/implot_demo.cpp
    src/imgui/implot_items.cpp
    src/imgui/implot.h
    src/imgui/implot_internal.h

    #icons
    src/IconHeader/IconsMaterialDesign.h
)

set(MYTINYGUISOURCES 
    
    #MytinyGUI
    src/MyTinyGUI/FontManager.cpp
    src/MyTinyGUI/Texture.cpp
    src/MyTinyGUI/TextureManager.cpp
    src/MyTinyGUI/Timer.cpp
    src/MyTinyGUI/UI_Button.cpp
    src/MyTinyGUI/UI_Ctexture.cpp
    src/MyTinyGUI/UI_Image.cpp 
    src/MyTinyGUI/UI_Text.cpp
    src/MyTinyGUI/Window.cpp
    src/MyTinyGUI/InputManager.cpp
    src/MyTinyGUI/include/AppState.h
    src/MyTinyGUI/include/AppStateMachine.h
    src/MyTinyGUI/include/FontManager.h
    src/MyTinyGUI/include/Texture.h
    src/MyTinyGUI/include/TextureManager.h
    src/MyTinyGUI/include/Timer.h
    src/MyTinyGUI/include/UI_Button.h
    src/MyTinyGUI/include/UI_Ctexture.h
    src/MyTinyGUI/include/UI_Image.h
    src/MyTinyGUI/include/UI_Text.h
    src/MyTinyGUI/include/Vector2D.h
    src/MyTinyGUI/include/Widget.h
    src/MyTinyGUI/include/Widgets.h
    src/MyTinyGUI/include/Window.h
    src/MyTinyGUI/InputManager.h
)

set(TINYXMLSOURCES
    #tinyxml
    src/tinyxml/tinystr.cpp
    src/tinyxml/tinystr.h
    src/tinyxml/tinyxml.cpp
    src/tinyxml/tinyxml.h
    src/tinyxml/tinyxmlerror.cpp
    src/tinyxml/tinyxmlparser.cpp
)

# Set the default platform to native
set(PLATFORM "native" CACHE STRING "Specify the build platform (native or emscripten)")

# Include the platform-specific CMake file
if (PLATFORM STREQUAL "emscripten")
    include(platform/emscripten/CMakeLists.txt)
elseif (PLATFORM STREQUAL "windows")
    include(platform/windows/CMakeLists.txt)
elseif (PLATFORM STREQUAL "linux")
    include(platform/linux/CMakeLists.txt)
else()
    message(FATAL_ERROR "Unsupported platform: ${PLATFORM}")
endif()