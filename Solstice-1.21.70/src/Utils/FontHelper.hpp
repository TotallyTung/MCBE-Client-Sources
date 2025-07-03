#pragma once
//
// Created by vastrakai on 6/29/2024.
//


class FontHelper {
public:
    static inline std::map<std::string, ImFont*> Fonts;

    static void load();
    /// <summary>
    /// Pushes the preferred font to the ImGui stack.
    /// </summary>
    /// <param name="large">Whether to push the large font.</param>
    static void pushPrefFont(bool large = false, bool bold = false, bool mForcePSans = false);
    /// <summary>
    /// Gets the appropriate font, according to interface settings.
    /// </summary>
    /// <param name="large">Whether to get the large font.</param>
    static ImFont* getFont(bool large = false, bool bold = false, bool mForcePSans = false);
    /// <summary>
    /// Pops the font from the ImGui stack.
    /// </summary>
    static void popPrefFont();
};