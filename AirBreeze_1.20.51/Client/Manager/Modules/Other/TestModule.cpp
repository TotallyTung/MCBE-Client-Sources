#include "TestModule.h"
#include "../../Manager.h"
#include "../../Client.h"
#include "../../../Include/imgui/imgui.h"
#include "../../../Utils/Render/ImGuiUtils.h"
#include "../../../Include/imgui/imgui_notify.h"

// Load Fonts
#include "../../../Include/imgui/fa_solid_900.h"


static bool isInitialized = false;
void TestModule::drawFPSCounter() {
    // Check if initialization is done
    if (!isInitialized) {
        // Perform one-time initialization here

        isInitialized = true;
    }


    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);

    static bool isDragging = false;
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        isDragging = true;
    }
    else if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        isDragging = false;
    }

    if (isDragging) {
        ImVec2 windowPos = ImGui::GetWindowPos();
        windowPos.x += ImGui::GetIO().MouseDelta.x;
        windowPos.y += ImGui::GetIO().MouseDelta.y;
        ImGui::SetWindowPos(windowPos);
    }


  
    if (ImGui::Begin("Shadow"))
    {
        static float shadow_thickness = 100.0f;
        static ImVec4 shadow_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        static bool shadow_filled = false;
        static ImVec4 shape_color = ImVec4(1.f, 0.f, 0.f, 1.0f);
        static float shape_rounding = 0.0f;
        static ImVec2 shadow_offset(0.0f, 0.0f);
        static ImVec4 background_color = ImVec4(0.f, 0.f, 0.f, 1.0f);
        static bool wireframe = true;
        static bool aa = true;
        static int poly_shape_index = 0;

        /**/
        // setting
        ImGui::Checkbox("Shadow filled", &shadow_filled);
        ImGui::SameLine();

        ImGui::Checkbox("Wireframe shapes", &wireframe);
        ImGui::SameLine();

        ImGui::Checkbox("Anti-aliasing", &aa);

        ImGui::DragFloat("Shadow Thickness", &shadow_thickness, 1.0f, 0.0f, 100.0f, "%.02f");
        ImGui::SliderFloat2("Offset", (float*)&shadow_offset, -32.0f, 32.0f);
        ImGui::SameLine();

        ImGui::ColorEdit4("Background Color", &background_color.x);
        ImGui::ColorEdit4("Shadow Color", &shadow_color.x);
        ImGui::ColorEdit4("Shape Color", &shape_color.x);
        ImGui::DragFloat("Shape Rounding", &shape_rounding, 1.0f, 0.0f, 20.0f, "%.02f");
        ImGui::Combo("Convex shape", &poly_shape_index, "Shape 1\0Shape 2\0Shape 3\0Shape 4\0Shape 4 (winding reversed)");


        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImDrawListFlags old_flags = draw_list->Flags;

        if (aa)
            draw_list->Flags |= ~ImDrawListFlags_AntiAliasedFill;
        else
            draw_list->Flags &= ~ImDrawListFlags_AntiAliasedFill;

        // Fill a strip of background
        draw_list->AddRectFilled(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y), ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetWindowContentRegionMax().x, ImGui::GetCursorScreenPos().y + 200.0f), ImGui::GetColorU32(background_color));

        // Rectangle
        {
            ImVec2 p = ImGui::GetCursorScreenPos();
            ImGui::Dummy(ImVec2(200.0f, 200.0f));

            ImVec2 r1(p.x + 50.0f, p.y + 50.0f);
            ImVec2 r2(p.x + 150.0f, p.y + 150.0f);
            ImDrawFlags draw_flags = shadow_filled ? ImDrawFlags_None : ImDrawFlags_ShadowCutOutShapeBackground;
            draw_list->AddShadowRect(r1, r2, ImGui::GetColorU32(shadow_color), shadow_thickness, shadow_offset, draw_flags, shape_rounding);

            if (wireframe)
                draw_list->AddRect(r1, r2, ImGui::GetColorU32(shape_color), shape_rounding);
            else
                draw_list->AddRectFilled(r1, r2, ImGui::GetColorU32(shape_color), shape_rounding);
        }

        ImGui::SameLine();

        // Circle
        {
            ImVec2 p = ImGui::GetCursorScreenPos();
            ImGui::Dummy(ImVec2(200.0f, 200.0f));

            // FIXME-SHADOWS: Offset forced to zero when shadow is not filled because it isn't supported
            float off = 10.0f;
            ImVec2 r1(p.x + 50.0f + off, p.y + 50.0f + off);
            ImVec2 r2(p.x + 150.0f - off, p.y + 150.0f - off);
            ImVec2 center(p.x + 100.0f, p.y + 100.0f);
            ImDrawFlags draw_flags = shadow_filled ? ImDrawFlags_None : ImDrawFlags_ShadowCutOutShapeBackground;
            draw_list->AddShadowCircle(center, 50.0f, ImGui::GetColorU32(shadow_color), shadow_thickness, shadow_filled ? shadow_offset : ImVec2(0.0f, 0.0f), draw_flags, 0);

            if (wireframe)
                draw_list->AddCircle(center, 50.0f, ImGui::GetColorU32(shape_color), 0);
            else
                draw_list->AddCircleFilled(center, 50.0f, ImGui::GetColorU32(shape_color), 0);
        }

        ImGui::SameLine();

        // Convex shape
        {
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImGui::Dummy(ImVec2(200.0f, 200.0f));

            const ImVec2 poly_centre(pos.x + 50.0f, pos.y + 100.0f);
            ImVec2 poly_points[8];
            int poly_points_count = 0;

            switch (poly_shape_index)
            {
            default:
            case 0:
            {
                poly_points[0] = ImVec2(poly_centre.x - 32.0f, poly_centre.y);
                poly_points[1] = ImVec2(poly_centre.x - 24.0f, poly_centre.y + 24.0f);
                poly_points[2] = ImVec2(poly_centre.x, poly_centre.y + 32.0f);
                poly_points[3] = ImVec2(poly_centre.x + 24.0f, poly_centre.y + 24.0f);
                poly_points[4] = ImVec2(poly_centre.x + 32.0f, poly_centre.y);
                poly_points[5] = ImVec2(poly_centre.x + 24.0f, poly_centre.y - 24.0f);
                poly_points[6] = ImVec2(poly_centre.x, poly_centre.y - 32.0f);
                poly_points[7] = ImVec2(poly_centre.x - 32.0f, poly_centre.y - 32.0f);
                poly_points_count = 8;
                break;
            }
            case 1:
            {
                poly_points[0] = ImVec2(poly_centre.x + 40.0f, poly_centre.y - 20.0f);
                poly_points[1] = ImVec2(poly_centre.x, poly_centre.y + 32.0f);
                poly_points[2] = ImVec2(poly_centre.x - 24.0f, poly_centre.y - 32.0f);
                poly_points_count = 3;
                break;
            }
            case 2:
            {
                poly_points[0] = ImVec2(poly_centre.x - 32.0f, poly_centre.y);
                poly_points[1] = ImVec2(poly_centre.x, poly_centre.y + 32.0f);
                poly_points[2] = ImVec2(poly_centre.x + 32.0f, poly_centre.y);
                poly_points[3] = ImVec2(poly_centre.x, poly_centre.y - 32.0f);
                poly_points_count = 4;
                break;
            }
            case 3:
            {
                poly_points[0] = ImVec2(poly_centre.x - 4.0f, poly_centre.y - 20.0f);
                poly_points[1] = ImVec2(poly_centre.x + 12.0f, poly_centre.y + 2.0f);
                poly_points[2] = ImVec2(poly_centre.x + 8.0f, poly_centre.y + 16.0f);
                poly_points[3] = ImVec2(poly_centre.x, poly_centre.y + 32.0f);
                poly_points[4] = ImVec2(poly_centre.x - 16.0f, poly_centre.y - 32.0f);
                poly_points_count = 5;
                break;
            }
            case 4: // Same as test case 3 but with reversed winding
            {
                poly_points[0] = ImVec2(poly_centre.x - 16.0f, poly_centre.y - 32.0f);
                poly_points[1] = ImVec2(poly_centre.x, poly_centre.y + 32.0f);
                poly_points[2] = ImVec2(poly_centre.x + 8.0f, poly_centre.y + 16.0f);
                poly_points[3] = ImVec2(poly_centre.x + 12.0f, poly_centre.y + 2.0f);
                poly_points[4] = ImVec2(poly_centre.x - 4.0f, poly_centre.y - 20.0f);
                poly_points_count = 5;
                break;
            }
            }

            // FIXME-SHADOWS: Offset forced to zero when shadow is not filled because it isn't supported
            ImDrawFlags draw_flags = shadow_filled ? ImDrawFlags_None : ImDrawFlags_ShadowCutOutShapeBackground;
            draw_list->AddShadowConvexPoly(poly_points, poly_points_count, ImGui::GetColorU32(shadow_color), shadow_thickness, shadow_filled ? shadow_offset : ImVec2(0.0f, 0.0f), draw_flags);

            if (wireframe)
                draw_list->AddPolyline(poly_points, poly_points_count, ImGui::GetColorU32(shape_color), true, 1.0f);
            else
                draw_list->AddConvexPolyFilled(poly_points, poly_points_count, ImGui::GetColorU32(shape_color));
        }
        draw_list->Flags = old_flags;
    }

    ImGui::End();
    ImGui::PopStyleVar();
}



void TestModule::onImGuiRender() {
    drawFPSCounter();
}

void TestModule::onEnable(void) {
    auto instance = Minecraft::getClientInstance();
    auto list = instance->getLocalPlayer()->level->getRuntimeActorList();

    //auto angles = Vec2<float>((rot.x) * -(3.14f / 180.f), (rot.y + 90.f) * (3.14f / 180.f));
    
    //auto motion = Vec3<float>(std::cos(angles.y) * std::cos(angles.x) * 1.f, std::sin(angles.x) * 1.f, std::sin(angles.y) * std::cos(angles.x) * 1.f);

    for (Actor* actor : list) {
        Utils::debugOutput("getRuntimeActorList working", true);
    }

}