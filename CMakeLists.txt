set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_library(CoreLib
    source/EngineCore.cpp
    source/EngineCore.h
    source/Test.cpp
    ${CORE_MOC_FILES}
)

target_include_directories(CoreLib PUBLIC source)

find_package(Qt6 REQUIRED COMPONENTS OpenGL Widgets)

target_link_libraries(CoreLib 
    PUBLIC 
        Qt6::Widgets
        Qt6::OpenGL
        ResourceManagersLib
        ECSLib
) 