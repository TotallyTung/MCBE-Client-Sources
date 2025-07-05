cmake_minimum_required(VERSION 3.24)

include(cmake/CPM.cmake)

project(PraxInternal)
set(CMAKE_CXX_STANDARD 23)
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")
set(CRYPTOPP_BUILD_TESTING OFF)
# NOMINMAX
add_compile_definitions(NOMINMAX)
add_compile_definitions(JM_XORSTR_DISABLE_AVX_INTRINSICS)

# EnTT config
add_compile_definitions(ENTT_SPARSE_PAGE=2048)
add_compile_definitions(ENTT_PACKED_PAGE=128)

if (${IS_DEBUG_BUILD})
        add_compile_definitions(__DEBUG__)
endif ()


# Bruh
if (CMAKE_BUILD_TYPE STREQUAL "Release")
        if (CMAKE_BINARY_DIR MATCHES "cmake-build-release-paid")
                add_compile_definitions(__RELEASEPAID__)
        elseif (CMAKE_BINARY_DIR MATCHES "cmake-build-release-free")
                add_compile_definitions(__RELEASEFREE__)
        endif ()
endif ()

# Debug options
add_link_options($<$<CONFIG:Debug>:/INCREMENTAL>)
add_compile_options($<$<CONFIG:Debug>:/Zi>)
# compile options for release to make it harder to reverse
add_compile_options($<$<CONFIG:Release>:/O2>) # optimize for speed
add_compile_options($<$<CONFIG:Release>:/Oi>) # enable intrinsic functions
add_compile_options($<$<CONFIG:Release>:/GL>) # enable link-time code generation
add_compile_options($<$<CONFIG:Release>:/Gy>) # separate functions for linker
add_compile_options($<$<CONFIG:Release>:/Gw>) # optimize global data
add_compile_options($<$<CONFIG:Release>:/GS>) # buffer security check
add_compile_options($<$<CONFIG:Release>:/Gm->) # disable minimal rebuild
add_compile_options($<$<CONFIG:Release>:/GF>) # enable string pooling
add_compile_options($<$<CONFIG:Release>:/fp:fast>) # optimize floating point
add_compile_options($<$<CONFIG:Release>:/GR->) # disable RTTI
add_compile_options($<$<CONFIG:Release>:/Gd>) # use __cdecl for all functions
add_compile_options($<$<CONFIG:Release>:/MT>) # use static multithreaded runtime
add_compile_options($<$<CONFIG:Release>:/Ob2>) # inline any suitable function

set(PRAX_VERSION "v1.2.4")
if (CMAKE_BINARY_DIR MATCHES "cmake-build-release-paid")
        set(PRAX_VERSION "${PRAX_VERSION}p")
endif ()
if (${IS_DEBUG_BUILD})
        set(PRAX_VERSION "${PRAX_VERSION}d")
endif ()
add_compile_definitions(PRAX_VERSION="${PRAX_VERSION}")

add_definitions(-DWIN32_LEAN_AND_MEAN)

include_directories(.)
include_directories(src)
include_directories(Includes/ImGui)
include_directories(Includes/Kiero)
include_directories(Includes/nlohmann)
include_directories(Includes/minhook/include)
include_directories(Includes/xor)
include_directories(Includes/glm)
include_directories(Includes/libhat)
include_directories(Includes/libhat/libhat)
include_directories(Includes/entt)
include_directories(Includes/magicenum)
include_directories(Includes/magic_vtable)
include_directories(Includes/Sockets)

file(GLOB_RECURSE minhookFiles "src/Includes/minhook/include/*.cpp")
file(GLOB_RECURSE imguiFiles "src/Includes/ImGui/*.cpp")
file(GLOB_RECURSE nlohmannFiles "src/Includes/nlohmann/*.cpp")
file(GLOB_RECURSE keiroFiles "src/Includes/Kiero/*.cpp")
file(GLOB_RECURSE xorFiles "src/Includes/xor/*.hpp")
file(GLOB_RECURSE glmFiles "src/Includes/glm/*.hpp")
file(GLOB_RECURSE libhatFiles "src/Includes/libhat/*.hpp" "src/Includes/libhat/includes/*.hpp")
file(GLOB_RECURSE enttFiles "src/Includes/entt/*.hpp")
file(GLOB_RECURSE magicenumFiles "src/Includes/magicenum/*.hpp")
file(GLOB_RECURSE magicvtableFiles "src/Includes/magic_vtable/*.hpp")
file(GLOB_RECURSE SocketsFiles "src/Includes/Sockets/*.cpp")
# file(GLOB_RECURSE ThemidaFiles "src/Includes/Themida/*.h")

set(includeFiles "${ThemidaFiles} ${minhookFiles}
${imguiFiles} ${keiroFiles} ${nlohmannFiles}
${xorFiles} ${glmFiles} ${libhatFiles}
${enttFiles} ${magicenumFiles} ${magicvtableFiles}
${SocketsFiles}")

include_directories(includeFiles)

file(GLOB_RECURSE srcFiles "src/*.cpp")

function(add_resources out_var)
        set(result)
        foreach(in_f ${ARGN})
                file(RELATIVE_PATH src_f ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${in_f})
                set(out_f "${PROJECT_BINARY_DIR}/${in_f}.o")
                add_custom_command(OUTPUT ${out_f}
                        COMMAND ld -r -b binary -o ${out_f} ${src_f}
                        DEPENDS ${in_f}
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                        COMMENT "Building object ${out_f}"
                        VERBATIM
                )
                list(APPEND result ${out_f})
        endforeach()
        set(${out_var} "${result}" PARENT_SCOPE)
endfunction()

add_resources(resource_files
        # Inter
        # resources/fonts/inter/Inter-Black.ttf
        resources/fonts/inter/Inter-Bold.ttf
        # resources/fonts/inter/Inter-ExtraBold.ttf
        resources/fonts/inter/Inter-ExtraLight.ttf
        resources/fonts/inter/Inter-Light.ttf
        resources/fonts/inter/Inter-Medium.ttf
        resources/fonts/inter/Inter-Regular.ttf
        resources/fonts/inter/Inter-SemiBold.ttf
        resources/fonts/inter/Inter-Thin.ttf
        resources/fonts/inter/Sarabun-Light.ttf

        # jetbrains mono
        # resources/fonts/jetbrains-mono/JetBrainsMono-Bold.ttf
        # resources/fonts/jetbrains-mono/JetBrainsMono-BoldItalic.ttf
        # resources/fonts/jetbrains-mono/JetBrainsMono-ExtraBold.ttf
        # resources/fonts/jetbrains-mono/JetBrainsMono-ExtraBoldItalic.ttf
        # resources/fonts/jetbrains-mono/JetBrainsMono-ExtraLight.ttf
        # resources/fonts/jetbrains-mono/JetBrainsMono-ExtraLightItalic.ttf
        # resources/fonts/jetbrains-mono/JetBrainsMono-Italic.ttf
        # resources/fonts/jetbrains-mono/JetBrainsMono-Light.ttf
        # resources/fonts/jetbrains-mono/JetBrainsMono-LightItalic.ttf
        # resources/fonts/jetbrains-mono/JetBrainsMono-Medium.ttf
        # resources/fonts/jetbrains-mono/JetBrainsMono-MediumItalic.ttf
        resources/fonts/jetbrains-mono/JetBrainsMono-Regular.ttf
        # resources/fonts/jetbrains-mono/JetBrainsMono-Thin.ttf
        # resources/fonts/jetbrains-mono/JetBrainsMono-ThinItalic.ttf

        # greycliff cf
        resources/fonts/greycliff-cf/GreycliffCF-Bold.ttf
        resources/fonts/greycliff-cf/GreycliffCF-DemiBold.ttf
        # resources/fonts/greycliff-cf/GreycliffCF-DemiBoldOblique.ttf
        # resources/fonts/greycliff-cf/GreycliffCF-ExtraBold.ttf
        resources/fonts/greycliff-cf/GreycliffCF-ExtraBoldOblique.ttf
        resources/fonts/greycliff-cf/GreycliffCF-ExtraLightOblique.ttf
        resources/fonts/greycliff-cf/GreycliffCF-ExtraLight.ttf
        # resources/fonts/greycliff-cf/GreycliffCF-Heavy.ttf
        # resources/fonts/greycliff-cf/GreycliffCF-HeavyOblique.ttf
        resources/fonts/greycliff-cf/GreycliffCF-Light.ttf
        resources/fonts/greycliff-cf/GreycliffCF-LightOblique.ttf
        resources/fonts/greycliff-cf/GreycliffCF-Medium.ttf
        resources/fonts/greycliff-cf/GreycliffCF-Regular.ttf
        # resources/fonts/greycliff-cf/GreycliffCF-Regular-Oblique.ttf
        resources/fonts/greycliff-cf/GreycliffCF-Thin.ttf
        resources/fonts/greycliff-cf/GreycliffCF-ThinOblique.ttf

        # icons
        resources/fonts/icons/icons-v1.ttf
)

add_library(PraxInternal SHARED src/dllmain.cpp ${srcFiles} ${resource_files}
        src/Prax/SDK/Game/Entity/Components/ActorOwnerComponent.h
        src/Prax/SDK/Game/Network/Other/MobEffectPacket.h)

add_subdirectory(Includes/Kiero/)
add_subdirectory(Includes/ImGui/)
add_subdirectory(Includes/minhook/)
add_subdirectory(Includes/xor/)
add_subdirectory(Includes/glm/)
add_subdirectory(Includes/libhat/)
add_subdirectory(Includes/Sockets/)
#add_subdirectory(Includes/Luau/)
add_subdirectory(cryptopp-cmake)
target_include_directories(PraxInternal PRIVATE /Includes/Kiero/)
target_include_directories(PraxInternal PRIVATE /Includes/ImGui/)
target_include_directories(PraxInternal PRIVATE /Includes/nlohmann/)
target_include_directories(PraxInternal PRIVATE /Includes/xor/)
target_include_directories(PraxInternal PRIVATE /Includes/glm/)
target_include_directories(PraxInternal PRIVATE /Includes/libhat/include)
target_include_directories(PraxInternal PRIVATE /Includes/magicenum)
target_include_directories(PraxInternal PRIVATE /Includes/magic_vtable)
# target_include_directories(PraxInternal PRIVATE /Includes/Themida)
target_include_directories(PraxInternal PRIVATE /Includes/Sockets)
#target_include_directories(PraxInternal PRIVATE /Includes/Luau)
#target_include_directories(PraxInternal PRIVATE /Includes/Luau/Compiler/include)
#target_include_directories(PraxInternal PRIVATE /Includes/Luau/VM/include)

target_link_libraries(PraxInternal #[[Luau.Compiler Luau.VM Luau.Ast]] cryptopp sioclient minhook libhat Kiero d3d12.lib d3d11.lib d2d1.lib dxgi.lib dxguid.lib dwrite.lib winmm.lib dbghelp.lib)

set_property(TARGET PraxInternal cryptopp sioclient minhook libhat Kiero PROPERTY
        MSVC_RUNTIME_LIBRARY "$<$<CONFIG:Debug>:MultiThreadedDLL>$<$<CONFIG:Release>:MultiThreaded>")

target_precompile_headers(PraxInternal PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:<Src/pch.cpp$<ANGLE-R>>"
        )


