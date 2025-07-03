// Created by Tony on 2024-10-10 10:01:54

// TessellatorRenderUtils.cpp
#include "TessRenderUtils.h"
#include "../SDK/GameData.h"

void TessellatorRenderUtils::drawBox(const Vec3<float>& lower, const Vec3<float>& upper, const UIColor& color, float lineWidth, bool fill, bool outline, bool edgeOnly) {
    if (RenderUtils::screenContext3D == nullptr || RenderUtils::blendMaterial == nullptr)
        return;

    Tessellator* tess = RenderUtils::getTessellator3D();
    Vec3<float> origin = mc.getClientInstance()->getLevelRenderer()->levelRendererPlayer->cameraPos1;

    Vec3<float> diff(upper.x - lower.x, upper.y - lower.y, upper.z - lower.z);
    Vec3<float> vertices[8];
    for (int i = 0; i < 8; ++i) {
        vertices[i] = Vec3<float>(
            lower.x + (i & 1 ? diff.x : 0),
            lower.y + (i & 2 ? diff.y : 0),
            lower.z + (i & 4 ? diff.z : 0)
        );
    }

    if (fill) {
        MC_Color fillColor = color.toMC_Color();
        fillColor.a *= 0.5f; // Make fill slightly transparent
        RenderUtils::setColor(fillColor);

        static const std::vector<std::vector<int>> faces = {
            {0, 1, 3, 2}, {4, 5, 7, 6}, {0, 4, 6, 2},
            {1, 5, 7, 3}, {0, 1, 5, 4}, {2, 3, 7, 6}
        };

        tess->begin(VertextFormat::QUAD, 24); // 6 faces * 4 vertices
        for (const auto& face : faces) {
            for (int i : face) {
                Vec3<float> vertex = vertices[i].sub(origin);
                tess->vertex(vertex.x, vertex.y, vertex.z);
            }
        }
        tess->renderMeshImmediately(RenderUtils::screenContext3D, RenderUtils::blendMaterial);
    }

    if (outline) {
        MC_Color lineColor = color.toMC_Color();
        lineColor.a = 1.0f;  // Full opacity for outlines
        RenderUtils::setColor(lineColor);

        static const std::vector<std::pair<int, int>> edges = {
            {0, 1}, {1, 3}, {3, 2}, {2, 0},
            {4, 5}, {5, 7}, {7, 6}, {6, 4},
            {0, 4}, {1, 5}, {3, 7}, {2, 6}
        };

        if (edgeOnly) {
            tess->begin(VertextFormat::LINE_LIST, 24); // 12 edges * 2 vertices
            for (const auto& edge : edges) {
                for (int i : {edge.first, edge.second}) {
                    Vec3<float> vertex = vertices[i].sub(origin);
                    tess->vertex(vertex.x, vertex.y, vertex.z);
                }
            }
            tess->renderMeshImmediately(RenderUtils::screenContext3D, RenderUtils::blendMaterial);
        }
        else {
            // Draw face outlines
            static const std::vector<std::vector<int>> faceOutlines = {
                {0, 1, 3, 2, 0}, {4, 5, 7, 6, 4},
                {0, 4}, {1, 5}, {3, 7}, {2, 6}
            };

            for (const auto& faceOutline : faceOutlines) {
                tess->begin(VertextFormat::LINE_STRIP, faceOutline.size());
                for (int i : faceOutline) {
                    Vec3<float> vertex = vertices[i].sub(origin);
                    tess->vertex(vertex.x, vertex.y, vertex.z);
                }
                tess->renderMeshImmediately(RenderUtils::screenContext3D, RenderUtils::blendMaterial);
            }
        }
    }
}

void TessellatorRenderUtils::drawLine(const Vec3<float>& start, const Vec3<float>& end, const UIColor& color, float lineWidth) {
    if (RenderUtils::screenContext3D == nullptr || RenderUtils::blendMaterial == nullptr)
        return;

    Tessellator* tess = RenderUtils::getTessellator3D();
    Vec3<float> origin = mc.getClientInstance()->getLevelRenderer()->levelRendererPlayer->cameraPos1;
    MC_Color mcColor = color.toMC_Color();

    tess->begin(VertextFormat::LINE_STRIP, 2);
    RenderUtils::setColor(mcColor);
    tess->vertex(start.x - origin.x, start.y - origin.y, start.z - origin.z);
    tess->vertex(end.x - origin.x, end.y - origin.y, end.z - origin.z);
    tess->renderMeshImmediately(RenderUtils::screenContext3D, RenderUtils::blendMaterial);
}