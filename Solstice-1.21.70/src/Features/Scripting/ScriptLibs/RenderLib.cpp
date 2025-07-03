//
// Created by vastrakai on 9/24/2024.
//

#include "RenderLib.hpp"

#include <Hook/Hooks/RenderHooks/D3DHook.hpp>

void RenderLib::initialize(lua_State* L)
{

    getGlobalNamespace(L)
        .beginNamespace("ImGui") // ImGui in c++ is a namespace
            .addFunction("Begin", [](const char* name) { ImGui::Begin(name); })
            .addFunction("BeginChild", [](const char* name, ImVec2 size, bool border, ImGuiWindowFlags flags) { ImGui::BeginChild(name, size, border, flags); })
            .addFunction("BeginChildID", [](ImGuiID id, ImVec2 size, bool border, ImGuiWindowFlags flags) { ImGui::BeginChild(id, size, border, flags); })
            .addFunction("BeginChildFrame", [](ImGuiID id, ImVec2 size, ImGuiWindowFlags flags) { ImGui::BeginChildFrame(id, size, flags); })
            .addFunction("BeginCombo", [](const char* name, const char* previewValue, ImGuiComboFlags flags) { ImGui::BeginCombo(name, previewValue, flags); })
            .addFunction("BeginDragDropSource", [](ImGuiDragDropFlags flags) { ImGui::BeginDragDropSource(flags); })
            .addFunction("BeginDragDropTarget", []() { ImGui::BeginDragDropTarget(); })
            .addFunction("BeginGroup", []() { ImGui::BeginGroup(); })
            .addFunction("BeginMainMenuBar", []() { ImGui::BeginMainMenuBar(); })
            .addFunction("BeginMenu", [](const char* name, bool enabled) { ImGui::BeginMenu(name, enabled); })
            .addFunction("BeginMenuBar", []() { ImGui::BeginMenuBar(); })
            .addFunction("BeginPopup", [](const char* name) { ImGui::BeginPopup(name); })
            .addFunction("BeginPopupContextItem", [](const char* name, int mouseButton) { ImGui::BeginPopupContextItem(name, mouseButton); })
            .addFunction("BeginPopupContextVoid", [](const char* name, int mouseButton) { ImGui::BeginPopupContextVoid(name, mouseButton); })
            .addFunction("BeginTabBar", [](const char* name, ImGuiTabBarFlags flags) { ImGui::BeginTabBar(name, flags); })
            .addFunction("BeginTable", [](const char* name, int column, ImGuiTableFlags flags) { ImGui::BeginTable(name, column, flags); })
            .addFunction("BeginTooltip", []() { ImGui::BeginTooltip(); })
            .addFunction("Bullet", []() { ImGui::Bullet(); })
            .addFunction("BulletText", [](const char* text) { ImGui::BulletText(text); })
            .addFunction("Button", [](const char* label, ImVec2 size) { return ImGui::Button(label, size); })
            //.addFunction("Checkbox", [](const char* label, bool* v) { return ImGui::Checkbox(label, v); })
            .addFunction("CloseCurrentPopup", []() { ImGui::CloseCurrentPopup(); })
            .addFunction("CollapsingHeader", [](const char* label, ImGuiTreeNodeFlags flags) { return ImGui::CollapsingHeader(label, flags); })
            .addFunction("ColorButton", [](const char* desc_id, ImVec4 col, ImGuiColorEditFlags flags, ImVec2 size) { return ImGui::ColorButton(desc_id, col, flags, size); })
            .addFunction("Text", [](const char* text) { ImGui::Text(text); })
            .addFunction("End", []() { ImGui::End(); })
            .addFunction("GetBackgroundDrawList", []() { return ImGui::GetBackgroundDrawList(); })
            .addFunction("GetIO", []() { return &ImGui::GetIO(); })
            .addFunction("GetDrawData", []() { return ImGui::GetDrawData(); })
            .addFunction("SetNextWindowSize", [](ImVec2 size) { ImGui::SetNextWindowSize(size); })
            .addFunction("SetNextWindowPos", [](ImVec2 pos) { ImGui::SetNextWindowPos(pos); })
            .addFunction("SetNextWindowBgAlpha", [](float alpha) { ImGui::SetNextWindowBgAlpha(alpha); })
            .addFunction("SetNextWindowCollapsed", [](bool collapsed) { ImGui::SetNextWindowCollapsed(collapsed); })
            .addFunction("SetNextWindowFocus", []() { ImGui::SetNextWindowFocus(); })
            .addFunction("SetNextWindowContentSize", [](ImVec2 size) { ImGui::SetNextWindowContentSize(size); })
            // input stuff
            .addFunction("IsKeyDown", [](int key) { return ImGui::IsKeyDown(static_cast<ImGuiKey>(key)); })
            .addFunction("IsKeyPressed", [](int key) { return ImGui::IsKeyPressed(static_cast<ImGuiKey>(key)); })
            .addFunction("IsKeyReleased", [](int key) { return ImGui::IsKeyReleased(static_cast<ImGuiKey>(key)); })
            .addFunction("IsMouseDown", [](int button) { return ImGui::IsMouseDown(button); })
            .addFunction("IsMouseClicked", [](int button) { return ImGui::IsMouseClicked(button); })
            .addFunction("IsMouseReleased", [](int button) { return ImGui::IsMouseReleased(button); })
            .addFunction("GetMousePos", []() { return ImGui::GetMousePos(); })
        .endNamespace()
        .beginClass<ImDrawList>("ImDrawList")
            .addFunction("AddLine", [](ImDrawList* drawList, ImVec2 p1, ImVec2 p2, ImColor color, float thickness) {
                drawList->AddLine(p1, p2, color, thickness);
            })
            .addFunction("AddRect", [](ImDrawList* drawList, ImVec2 p1, ImVec2 p2, ImColor color, float rounding, int roundingCorners, float thickness) {
                drawList->AddRect(p1, p2, color, rounding, roundingCorners, thickness);
            })
            .addFunction("AddRectFilled", [](ImDrawList* drawList, ImVec2 p1, ImVec2 p2, ImColor color, float rounding, int roundingCorners) {
                drawList->AddRectFilled(p1, p2, color, rounding, roundingCorners);
            })
            .addFunction("AddCircle", [](ImDrawList* drawList, ImVec2 center, float radius, ImColor color, int numSegments, float thickness) {
                drawList->AddCircle(center, radius, color, numSegments, thickness);
            })
            .addFunction("AddCircleFilled", [](ImDrawList* drawList, ImVec2 center, float radius, ImColor color, int numSegments) {
                drawList->AddCircleFilled(center, radius, color, numSegments);
            })
            .addFunction("AddText", [](ImDrawList* drawList, ImVec2 pos, ImColor color, const char* text_begin, const char* text_end) {
                drawList->AddText(pos, color, text_begin, text_end);
            })
            .addFunction("AddTriangleFilled", [](ImDrawList* drawList, ImVec2 p1, ImVec2 p2, ImVec2 p3, ImColor color) {
                drawList->AddTriangleFilled(p1, p2, p3, color);
            })
            .addFunction("AddTriangle", [](ImDrawList* drawList, ImVec2 p1, ImVec2 p2, ImVec2 p3, ImColor color, float thickness) {
                drawList->AddTriangle(p1, p2, p3, color, thickness);
            })
            .addFunction("AddQuadFilled", [](ImDrawList* drawList, ImVec2 p1, ImVec2 p2, ImVec2 p3, ImVec2 p4, ImColor color) {
                drawList->AddQuadFilled(p1, p2, p3, p4, color);
            })
            .addFunction("AddQuad", [](ImDrawList* drawList, ImVec2 p1, ImVec2 p2, ImVec2 p3, ImVec2 p4, ImColor color, float thickness) {
                drawList->AddQuad(p1, p2, p3, p4, color, thickness);
            })
            .addFunction("AddImage", [](ImDrawList* drawList, const char* texturePath, ImVec2 p1, ImVec2 p2, ImVec2 uv0, ImVec2 uv1, ImColor color) {
                if (!FileUtils::fileExists(texturePath))
                    throw std::runtime_error("File does not exist: " + std::string(texturePath));
                void* texture = ImTextureCache::cacheOrGet(texturePath)->texture;
                drawList->AddImage(texture, p1, p2, uv0, uv1, color);
            })
            .addFunction("AddImageRounded", [](ImDrawList* drawList, const char* texturePath, ImVec2 p1, ImVec2 p2, ImVec2 uv0, ImVec2 uv1, ImColor color, float rounding, int roundingCorners) {
                if (!FileUtils::fileExists(texturePath))
                    throw std::runtime_error("File does not exist: " + std::string(texturePath));
                void* texture = ImTextureCache::cacheOrGet(texturePath)->texture;
                drawList->AddImageRounded(texture, p1, p2, uv0, uv1, color, rounding, roundingCorners);
            })
            .addFunction("AddConvexPolyFilled", [](ImDrawList* drawList, std::vector<ImVec2> points, ImColor color) {
                drawList->AddConvexPolyFilled(points.data(), points.size(), color);
            })
            .addFunction("AddPolyline", [](ImDrawList* drawList, std::vector<ImVec2> points, ImColor color, bool closed, float thickness) {
                drawList->AddPolyline(points.data(), points.size(), color, closed, thickness);
            })
        .endClass()
        .beginClass<ImGuiIO>("ImGuiIO")
            .addProperty("DisplaySize", &ImGuiIO::DisplaySize)
            .addProperty("DeltaTime", &ImGuiIO::DeltaTime)
        .endClass()
        .beginClass<std::vector<ImVec2>>("ImVec2Vector")
        .addFunction("size", &std::vector<ImVec2>::size)
        .addFunction("get", [](std::vector<ImVec2>* vec, size_t index) -> ImVec2
        {
            if (index < 1 || index > vec->size())
            {
                return ImVec2();
            }
            return (*vec)[index - 1];
        })
        .addFunction("__len", &std::vector<ImVec2>::size)
        .addFunction("__index", [](std::vector<ImVec2>* vec, size_t index) -> ImVec2
        {
            if (index < 1 || index > vec->size())
            {
                return ImVec2();
            }
            return (*vec)[index - 1];
        })
        .endClass()

    .beginClass<RenderUtils>("RenderUtils")
    .addStaticProperty("transform", &RenderUtils::transform)
    .addStaticFunction("drawOutlinedAABB", &RenderUtils::drawOutlinedAABB)
    .endClass()
    .beginClass<FrameTransform>("FrameTransform")
    .addProperty("mMatrix", &FrameTransform::mMatrix)
    .addProperty("mOrigin", &FrameTransform::mOrigin)
    .addProperty("mPlayerPos", &FrameTransform::mPlayerPos)
    .addProperty("mFov", &FrameTransform::mFov)
    .endClass()


    ;

    // Example lua code:
    // local drawList = ImGui.GetBackgroundDrawList()
    // drawList:AddRect(ImVec2(0, 0), ImVec2(100, 100), 0xFFFFFFFF, 0, 0, 1.0)

    ImTextureCache::freeCachedTextures();
};
