#pragma once
//
// Created by vastrakai on 9/4/2024.
//

// Only for showing changelogs in the GUI
class UpdateForm : public ModuleBase<UpdateForm>
{
public:
    UpdateForm() : ModuleBase("UpdateForm", "Shows the changelogs of the latest update", ModuleCategory::Visual, 0, true)
    {
        mNames = {
            {Lowercase, "updateform"},
            {LowercaseSpaced, "update form"},
            {Normal, "UpdateForm"},
            {NormalSpaced, "Update Form"}
        };
    }

    std::string mOldHash = "";
    std::string mCurrentHash = "";
    std::vector<std::string> mCommits;
    bool mGatheredCommits = false;
    uint64_t mLastEnable = 0;
    bool mRequestingCommits = false;
    HttpRequest mRequest;

    void getCommits();
    static void onHttpResponse(HttpResponseEvent event);

    void onEnable() override;
    void onDisable() override;
    void onRenderEvent(class RenderEvent& event);
    void onMouseEvent(class MouseEvent& event);
    void onKeyEvent(class KeyEvent& event);

};