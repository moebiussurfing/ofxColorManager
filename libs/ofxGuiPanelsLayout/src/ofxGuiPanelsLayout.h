
// TODO:
// ++ improve system with any type with map
// + different positions of any panels for every group
// + could modify all addon enabling store panels in different sorting
// to do this we must store into de panel Data where are stored each item
// + should add group foldering minimize/maximize too on a group/app mode
//gui_main.getGroup("group name").maximize()
// + textFlow: faster fonts renders or fbo...
// + textBox: add rentagle awith alpha, faster render font

#pragma once

#include "ofMain.h"

#include "constants/OFX_SURFING_SETTINGS.h"

#include "ofxSurfingHelpers.h"

//--

#ifdef USE_GUI_TYPE_OFX_GUI
#include "ofxGui.h"
#endif

#ifdef USE_GUI_TYPE_OFX_DAT_GUI
#include "ofxDatGui.h"
#endif

#ifdef USE_GUI_TYPE_TEXT_BOX
#include "TextBox.h"
#endif

#ifdef USE_GUI_TYPE_TEXT_FLOW
#include "ofxTextFlow.h"
#endif

#ifdef USE_GUI_TYPE_TOGGLE_PARAM
#endif

#ifdef USE_GUI_TYPE_OFX_GUI_EXTENDED
#endif

#ifdef USE_GUI_TYPE_OFX_IM_UI
#endif

#ifdef USE_GUI_TYPE_OFX_OTHER
#endif

//-

#include "ofxControlPointManager.h"

#ifdef USE_MOUSE_RULER
#include "ofxMouseRuler.h"
#endif

#define NUM_GROUPS 10
// groups are like different app modes/states, to show different groups of gui panels

class ofxGuiPanelsLayout {

public:

    ofxGuiPanelsLayout();
    ~ofxGuiPanelsLayout();

    void update();
    void draw();
    void exit();

    enum GUI_TYPES
    {
        GUI_TYPE_OFX_GUI,
        GUI_TYPE_OFX_DAT_GUI,
        GUI_TYPE_OFX_GUI_EXTENDED,
        GUI_TYPE_OFX_IM_UI,
        GUI_TYPE_OFX_OTHER,
        GUI_TYPE_TOGGLE_PARAM,
        GUI_TYPE_TEXT_BOX
    };
    #define NUM_TYPES 7

    // 1. must use method to 'add' panels in this type sorting
    //TODO
    // should store in any orther
    //-

    int size_OFX_GUI = 0;
    int size_OFX_DAT = 0;
    int size_TOGGLES = 0;
    int size_OFX_TEXT_BOX = 0;

#ifdef USE_GUI_TYPE_OFX_GUI
    vector < ofxPanel * > guis_OFX_GUI;
    void addPanel(ofxPanel * gui);
#endif

#ifdef USE_GUI_TYPE_OFX_DAT_GUI
    vector < ofxDatGui * > guis_OFX_DAT_GUI;
    void addPanel(ofxDatGui * gui, std::string name);//must include a name
#endif

#ifdef USE_GUI_TYPE_TOGGLE_PARAM
    vector < ofParameter<bool> * > guis_TOGGLE;
    void addToggle(ofParameter<bool> * toggle);
#endif

#ifdef USE_GUI_TYPE_TEXT_BOX
    vector < TextBox * > guis_OFX_TEXT_BOX;
    void addTextBox(TextBox * textBox);
#endif

    int size_ALL_GUIS;
    int size_ALL_PANELS;//count all

    // 2. and then to call setup
    void setup();

    //-

    // data

    struct panelData
    {
        ofParameter<int> type;
        ofParameter<std::string> name;
        ofParameter<bool> state; //visible;
        ofParameter<int> id;//not used
        ofParameter<glm::vec2> position;
        ofParameterGroup params_data;//not used
    };
    vector <panelData> panels;
    ofParameterGroup params_Control;

    //-

    // control panel

    ofParameter<bool> SHOW_controlPanel = true;
    ofxPanel gui_Control;
    glm::vec2 gui_Control_pos;

    // advanced subpanel to edit advanced features

    ofParameter<bool> SHOW_advanced;
    ofxPanel gui_Control_Advanced;
    glm::vec2 gui_Control_Advanced_pos;
    ofParameterGroup params_ControlAdvanced;
    void Changed_parametersAdvance(ofAbstractParameter &e);

    //-

    // groups states
    // state of every panel for a group state

    ofParameter<int> group_Selected;
    int group_Selected_PRE = 0;
    vector <vector<int>> groupStates;
    ofParameter<bool> group_Save;
    ofParameter<bool> group_autoSave;
    ofParameter<bool> group_autoLoad;
    ofParameter<bool> group_showAll;
    ofParameter<bool> group_hideAll;
    ofParameter<bool> group_SaveAll;
    void Changed_parameters(ofAbstractParameter &e);

    //-

    // draggable points

    ofParameter<bool> SHOW_draggable = false;
    float w, h;
    ofxControlPointManager controlPoints;
    std::string csvFileName;
    void refreshDraggables();// set drag points from storing panels vector

    //--

    // TexfFlow
#ifdef USE_GUI_TYPE_TEXT_FLOW
    void TextFlow_setup();
    void TextFlow_print(std::string s);
    void TextFlow_setColor(ofColor colortext);
    void TextFlow_setNumLines(int numMaxLines);
    void TextFlow_setVisible(bool visible);
    void TextFlow_setMaxLines(int numMaxLines);
    void TextFlow_setPosition(glm::vec2 p);
    void TextFlow_setTitle(std::string title);
#endif

    //-

    void setPosition(int numPanel, ofVec2f p);//same for multiple gui types
    ofVec2f getPosition(int numPanel);//same for multiple gui types

    void savePanels();
    void loadPanels();
    void refreshPanels();

    void saveGroups();
    void loadGroups();
//    void saveGroup(int g);
//    void loadGroup(int g);

    std::string pathFolder;

    //-

private:

    void addKeysListeners();
    void removeKeysListeners();
    void keyPressed( ofKeyEventArgs& eventArgs );
    void keyReleased( ofKeyEventArgs& eventArgs );
    void mousePressed( int x, int y );

#ifdef USE_MOUSE_RULER
    ofxMouseRuler mouseRuler;
#endif

};
