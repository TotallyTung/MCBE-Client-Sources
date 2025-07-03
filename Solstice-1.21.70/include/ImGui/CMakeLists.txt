target_sources(Solstice PRIVATE
        "imgui.cpp"
        "imgui_demo.cpp"
        "imgui_draw.cpp"
        "imgui_impl_dx11.cpp"
        "imgui_impl_dx12.cpp"
        "imgui_impl_win32.cpp"
        "imgui_tables.cpp"
        "imgui_widgets.cpp"
        "imgui_toggle.cpp"
        "imgui_toggle_palette.cpp"
        "imgui_toggle_renderer.cpp"
        "stb_image.h"
        "ImTween.h"
)

target_include_directories(Solstice PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
