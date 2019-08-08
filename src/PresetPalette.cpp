//
// Created by moebiussurfing on 7/31/19.
//

#include "PresetPalette.h"


//--------------------------------------------------------------
void PresetPalette::update() {

}


//--------------------------------------------------------------
void PresetPalette::draw()
{
//    guiVisible = true;
//
//    //-
//
//    // GUI
//
//    this->mouseOverGui = false;
//    if (this->guiVisible) {
//        this->mouseOverGui = this->gui_imGui();
//    }
//    if (this->mouseOverGui) {
//    } else {
//    }

    //--
}


//--------------------------------------------------------------
void PresetPalette::setup()
{
    //--

    // GUI

    //-

    // font

    ImGuiIO& io = ImGui::GetIO();
    string inputPath;
    inputPath = ofFilePath::getAbsolutePath("assets/fonts/PragmataProR_0822.ttf");
    const char* myPath = inputPath.c_str();
    ImFontConfig config;
    //    config.OversampleH = 3;
    //    config.OversampleV = 1;
    //    config.GlyphExtraSpacing.x = 1.0f;
    io.Fonts->AddFontFromFileTTF(myPath, 13.0f, &config);

    // create
    this->gui2.setup();
    this->guiVisible = true;

    // theme colors
//    gui_imGui_theme();

    //--
}


//--------------------------------------------------------------
bool PresetPalette::gui_imGui()
{
    float widgetFactor = 0.9;
    int gui2_w = 200;

//    auto mainSettings = ofxImGui::Settings();
    this->gui2.begin();

    auto palSettings = ofxImGui::Settings();
    palSettings.windowPos = ofVec2f(800, 400);
    palSettings.windowSize = ofVec2f(gui2_w, 300);

    if (ofxImGui::BeginWindow("PALETTES", palSettings, false))
    {
        // create
        static bool saved_palette_inited = false;
        static ImVec4 saved_palette[4];//same than Pantone palette
        if (!saved_palette_inited)
            for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
            {
                saved_palette[n].x = ofRandom(1.0);
                saved_palette[n].y = ofRandom(1.0);
                saved_palette[n].z = ofRandom(1.0);
                saved_palette[n].w = 1.0f;//alpha
            }
        saved_palette_inited = true;

        //-

        // draw palette

        if (ImGui::CollapsingHeader("PALETTE1"))
        {
            for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
            {
                //                ImGui::PushItemWidth(guiWidth * 0.2);
                ImGui::PushID(n);

                ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);

                if (ImGui::ColorButton("##palette", saved_palette[n],
                        ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip,
                        ImVec2(20, 20)))
                {
//                    color = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w); // Preserve alpha!

                    //-

                    ofLogNotice("PresetPalette") << "ImGui: PALETTE PICKED !" << endl;
                }

                //                ImGui::PopItemWidth();
                ImGui::PopID();
            }
        }
    }
    ofxImGui::EndWindow(palSettings);

    this->gui2.end();

//    return mainSettings.mouseOverGui;
    return palSettings.mouseOverGui;

}


//--------------------------------------------------------------
void PresetPalette::setName(string &_name)
{
    name_BACK = &_name;
}


//--------------------------------------------------------------
void PresetPalette::setCurveName(string &_curve)
{
    curveName_BACK = &_curve;
}

//--------------------------------------------------------------
void PresetPalette::setBackgroundColor(ofColor _background)
{
    backCol = _background;
}

////--------------------------------------------------------------
//void PresetPalette::setBackgroundColor(ofColor &_background)
//{
//    background_BACK = &_background;
//}

////--------------------------------------------------------------
//void PresetPalette::setBackgroundColor(ofParameter<ofFloatColor> &_background)
////void PresetPalette::setBackgroundColor(ofColor &_background)
//{
//    background_BACK = &_background;
//}


//--------------------------------------------------------------
void PresetPalette::setPalette(vector<ofColor> &_palette)
{
    palette_BACK = &_palette;
}

//--------------------------------------------------------------
vector<ofColor> PresetPalette::getPalette()
{
    return (*palette_BACK);
}

//--------------------------------------------------------------
ofColor PresetPalette::getBackground()
{
    return presetData.background;
}

//--------------------------------------------------------------
void PresetPalette::preset_load(string p)
{
    ofLogNotice("PresetPalette::preset_load") << p;
    string path = preset_path+p+".json";

    ofFile file(path);
    if (file.exists())
    {
        jsonin ji(file);
        ji >> presetData;

        ofLogNotice("PresetPalette::preset_load") << "presetData.name      : " << presetData.name;
        ofLogNotice("PresetPalette::preset_load") << "presetData.curveName : " << presetData.curveName;
        ofLogNotice("PresetPalette::preset_load") << "presetData.background: " << presetData.background;

        //        curvesTool.load(preset_path + presetData.curveName + ".yml");
        //
        //        if (color_backGround_SETAUTO)
        //        {
        //            color_backGround = presetData.background;
        //        }

        ofLogNotice("PresetPalette::preset_load") << "presetData.palette.size(): " << presetData.palette.size();

        //        palette_clear();
        //        for (int i=0; i<presetData.palette.size(); i++)
        //        {
        //            ofColor c;
        //            c = presetData.palette[i];
        //            ofLogNotice("PresetPalette::preset_load") << "addColor:" << ofToString(c) <<" ["<<i<<"]";
        //            palette_addColor(c);
        //        }

        //        ofLogNotice("PresetPalette::preset_load") << "DONE! preset_load  :" << p;
        //        ofLogNotice("PresetPalette::preset_load") << "palette.size()     :" << palette.size()<<endl;
        //        ofLogNotice("PresetPalette::preset_load") << "btns_palette.size():" << btns_palette.size()<<endl;


        (*name_BACK) = presetData.name;
        (*curveName_BACK) = presetData.curveName;
        (*palette_BACK) = presetData.palette;
    }
    else
    {
        ofLogNotice("PresetPalette::preset_load") << "FILE '" << path << "' NOT FOUND";
    }
}


//--------------------------------------------------------------
void PresetPalette::preset_save(string p)
{
    ofLogNotice("PresetPalette::preset_save") << "preset_save: " << p;
    string path = preset_path+p+".json";

    presetData.background = backCol;
    //    presetData.background = (*background_BACK);

    presetData.name = (*name_BACK);
    presetData.curveName = (*curveName_BACK);
    presetData.palette = (*palette_BACK);

    ofLogNotice("PresetPalette::preset_save") << "presetData.background: " << presetData.background;
    ofLogNotice("PresetPalette::preset_save") << "presetData.name      : " << presetData.name;
    ofLogNotice("PresetPalette::preset_save") << "presetData.curveName : " << presetData.curveName;
    ofLogNotice("PresetPalette::preset_save") << "presetData.palette   : " << ofToString(presetData.palette);

    //    curvesTool.save(preset_path+presetData.curveName+".yml");

    ofFile file(path, ofFile::WriteOnly);
    jsonout jo(file);
    jo << presetData;

    ofLogNotice("PresetPalette::preset_save") << "DONE! preset_save: " << p;
    cout << endl;
}


//--------------------------------------------------------------
void PresetPalette::gui_imGui_theme()
{
    // must be done after setup the gui

    ImGuiStyle *style = &ImGui::GetStyle();

    // my dark theme
    //    ImVec4* colors = ImGui::GetStyle().Colors;
    style->Colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style->Colors[ImGuiCol_WindowBg]               = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_ChildBg]                = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
    style->Colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    style->Colors[ImGuiCol_Border]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
    style->Colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_FrameBg]                = ImVec4(0.02f, 0.02f, 0.02f, 0.54f);
    style->Colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.26f, 0.26f, 0.26f, 0.68f);
    style->Colors[ImGuiCol_FrameBgActive]          = ImVec4(0.25f, 0.25f, 0.25f, 0.67f);
    style->Colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    style->Colors[ImGuiCol_TitleBgActive]          = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    style->Colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    style->Colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.12f, 0.11f, 0.11f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    style->Colors[ImGuiCol_CheckMark]              = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style->Colors[ImGuiCol_SliderGrab]             = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    style->Colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.74f, 0.75f, 0.77f, 0.79f);
    style->Colors[ImGuiCol_Button]                 = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered]          = ImVec4(0.19f, 0.19f, 0.19f, 0.79f);
    style->Colors[ImGuiCol_ButtonActive]           = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
    style->Colors[ImGuiCol_Header]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.31f);
    style->Colors[ImGuiCol_HeaderHovered]          = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive]           = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    style->Colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style->Colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.29f, 0.29f, 0.29f, 0.78f);
    style->Colors[ImGuiCol_SeparatorActive]        = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip]             = ImVec4(0.44f, 0.44f, 0.44f, 0.25f);
    style->Colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.68f, 0.68f, 0.68f, 0.67f);
    style->Colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.68f, 0.68f, 0.68f, 0.95f);
    style->Colors[ImGuiCol_PlotLines]              = ImVec4(0.81f, 0.79f, 0.79f, 1.00f);
    style->Colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.58f, 0.58f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram]          = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.35f);
    style->Colors[ImGuiCol_DragDropTarget]         = ImVec4(0.50f, 0.50f, 0.50f, 0.90f);
    style->Colors[ImGuiCol_NavHighlight]           = ImVec4(0.79f, 0.79f, 0.79f, 1.00f);
    style->Colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style->Colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style->Colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    //    // 2. defaut dark theme exported
    //    style->WindowMinSize = ImVec2(160, 65);
    //    style->FramePadding = ImVec2(4, 2);
    //    style->ItemSpacing = ImVec2(6, 2);
    //    style->ItemInnerSpacing = ImVec2(6, 4);
    //    style->Alpha = 1.0f;
    //    style->WindowRounding = 0.0f;
    //    style->FrameRounding = 0.0f;
    //    style->IndentSpacing = 6.0f;
    //    style->ItemInnerSpacing = ImVec2(2, 4);
    //    style->ColumnsMinSpacing = 50.0f;
    //    style->GrabMinSize = 14.0f;
    //    style->GrabRounding = 0.0f;
    //    style->ScrollbarSize = 12.0f;
    //    style->ScrollbarRounding = 0.0f;
}

