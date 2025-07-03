// Created by Tony on 2024-10-10 10:01:54

// TessellatorRenderUtils.h
#pragma once
#include "RenderUtils.h"

class TessellatorRenderUtils {
public:
    static void drawBox(const Vec3<float>& lower, const Vec3<float>& upper, const UIColor& color, float lineWidth = 1.0f, bool fill = true, bool outline = true, bool edgeOnly = false);
    static void drawLine(const Vec3<float>& start, const Vec3<float>& end, const UIColor& color, float lineWidth = 1.0f);
};