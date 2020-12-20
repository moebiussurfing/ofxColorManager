#include "ofxGuiPanelsLayout.h"

//-------------------------------------------
ofxGuiPanelsLayout::ofxGuiPanelsLayout()
{
    //-

    // path folder

    pathFolder = "settings/panels/";

    //-

    // control panels toogles

    params_Control.setName("CONTROL PANELS");

    //-

    // advanced

    group_Selected.set("APP MODE", 0, 0, NUM_GROUPS - 1);
    group_Save.set("save", false);
    group_autoLoad.set("auto load", true);
    group_autoSave.set("auto save", true);
    SHOW_draggable.set("show draggable", false);
    group_showAll.set("show all", false);
    group_hideAll.set("hide all", false);
    group_SaveAll.set("save all", false);

    //-

    // draggable points

    w = ofGetWidth();
    h = ofGetHeight();
    csvFileName = pathFolder + "controlPoints.csv";
    float marginLimits = 0;
    controlPoints.setLimitArea({marginLimits / 2.f, marginLimits / 2.f, w - marginLimits, h - marginLimits});
    ofShowCursor();

    //-

    addKeysListeners();

    //-

    // TEXT FLOW

#ifdef USE_GUI_TYPE_TEXT_FLOW
    TextFlow_setup();
    TextFlow_setColor(ofColor::white);
    TextFlow_setNumLines(30);
    TextFlow_setVisible(true);
    TextFlow_setPosition(glm::vec2 (400, 300));
    TextFlow_setTitle("-----KEYBOARD & PARAMS DEBUG-----");
    // TODO: draw rentangle with alpha
    // TODO: remake class without singleton pattern... to enable more than one logs
#endif

    //-

    // MOUSE RULER

#ifdef USE_MOUSE_RULER
    mouseRuler.setup();
    //mouseRuler.setOrig(0, 0);
    mouseRuler.toggleVisibility();
#endif

    //-
}

//------------

// TEXT FLOW
#ifdef USE_GUI_TYPE_TEXT_FLOW
//--------------------------------------------------------------------------
void ofxGuiPanelsLayout::TextFlow_setTitle(string _title)
{
    ofxTextFlow::setTitle(_title);
}

//--------------------------------------------------------------------------
void ofxGuiPanelsLayout::TextFlow_setPosition(glm::vec2 p)
{
    ofxTextFlow::setPosition(p);
}

//--------------------------------------------------------------------------
void ofxGuiPanelsLayout::TextFlow_print(std::string s)
{
    if (s == "clear") {
        ofxTextFlow::clear();
    }
    else
    {
        ofxTextFlow::addText(s);
    }
}

//--------------------------------------------------------------------------
void ofxGuiPanelsLayout::TextFlow_setup()
{
//    ofxTextFlow::addText("This is ofxTextFlow example");
//    ofxTextFlow::addText("Let's try press any key, or BACKSPACE");
//    ofxTextFlow::setTextColor(ofColor(50, 50, 50));
//    ofxTextFlow::setMaxLineNum(30);
    ofxTextFlow::setShowing(true);
//    ofxTextFlow::setPosition(glm::vec2 (100, 300));
}

//--------------------------------------------------------------------------
void ofxGuiPanelsLayout::TextFlow_setColor(ofColor colortext)
{
    ofxTextFlow::setTextColor(colortext);
}

//--------------------------------------------------------------------------
void ofxGuiPanelsLayout::TextFlow_setNumLines(int numMaxLines)
{
    ofxTextFlow::setMaxLineNum(numMaxLines);
}

//--------------------------------------------------------
void ofxGuiPanelsLayout::TextFlow_setVisible(bool visible)
{
    ofxTextFlow::setShowing(visible);
}
#endif

//-------------------------------------------------

// add params to groupParams before run setup
// (to add group params to gui panel after)

//-

// 1. ofxPanel

//-----------------------------------------------
void ofxGuiPanelsLayout::addPanel(ofxPanel *gui)
{
    int last = panels.size();
    panelData panel;
    panel.type.set("type", GUI_TYPE_OFX_GUI, 0, NUM_TYPES);
    panel.name.set("name", gui->getName());
    panel.id.set("id", last);
    panel.state.set(gui->getName(), true);
    glm::vec2 panelPos = glm::vec2(gui->getPosition());
    panel.position.set("pos", panelPos, glm::vec2(0, 0), glm::vec2(1440, 900));
    panels.push_back(panel);

    //-

    // add toggle to control panel for this added gui panel
    params_Control.add(panels[last].state);

    //-

    // pointer to gui
    guis_OFX_GUI.push_back(gui);

    //-

    // add default draggeable point
    ofVec2f v2;
    v2.set(panels[last].position.get().x, panels[last].position.get().y);
    controlPoints.addPoint(v2);

    //-
}

//-

// 2. ofxDatGui

#ifdef USE_GUI_TYPE_OFX_DAT_GUI
//------------------------------------------------------------------
void ofxGuiPanelsLayout::addPanel(ofxDatGui * gui, std::string name)
{
    int last = panels.size();
    panelData panel;
    panel.type.set("type", GUI_TYPE_OFX_DAT_GUI, 0, NUM_TYPES);
    panel.id.set("id", last);
    panel.name.set("name", name);
    panel.state.set(name, true);
    glm::vec2 panelPos = glm::vec2( gui->getPosition().x, gui->getPosition().y );
    panel.position.set("pos", panelPos, glm::vec2(0,0), glm::vec2(1440,900));
    panels.push_back(panel);

    //-

    // add toggle to control panel for this added gui panel
    params_Control.add( panels[last].state );

    //-

    // pointer to gui
    guis_OFX_DAT_GUI.push_back(gui);

    //-

    // add default draggeable point
    ofVec2f v2;
    v2.set(panels[last].position.get().x, panels[last].position.get().y);
    controlPoints.addPoint( v2 );

    //-
}
#endif

//-

// 3. toggle parameter

#ifdef USE_GUI_TYPE_TOGGLE_PARAM

//------------------------------------------------------------
void ofxGuiPanelsLayout::addToggle(ofParameter<bool> *toggle)
{
    // TODO: improve doing pointer linking to ofApp bools...

    int last = panels.size();
    panelData panel;
    panel.type.set("type", GUI_TYPE_TOGGLE_PARAM, 0, NUM_TYPES);
    panel.id.set("id", last);
    panel.name.set("name", toggle->getName());
    panel.state.set(toggle->getName(), true);
    //panel.state = toggle;
    //not used
    panel.position.set("pos", glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(1440, 900));//not used?
    panels.push_back(panel);

    //-

    // add toggle to control panel for this added gui panel
    params_Control.add(panels[last].state);

    //-

    // pointer to toggle ?
    guis_TOGGLE.push_back(toggle);

    //-

    // add default draggeable point (not used)
    ofVec2f v2;
    v2.set(panels[last].position.get().x, panels[last].position.get().y);
    controlPoints.addPoint(v2);

    //-
}

#endif

//-

// 4. TextBox

#ifdef USE_GUI_TYPE_TEXT_BOX
//----------------------------------------------------
void ofxGuiPanelsLayout::addTextBox(TextBox * textBox)
{
    int last = panels.size();
    panelData panel;
    panel.type.set("type", GUI_TYPE_TEXT_BOX, 0, NUM_TYPES);
    panel.name.set("name", textBox->getName());
    panel.id.set("id", last);
    panel.state.set(panel.name.get(), true);
    glm::vec2 panelPos = glm::vec2 (textBox->getPosition());
    panel.position.set("pos", panelPos, glm::vec2(0,0), glm::vec2(1440,900));
    panels.push_back(panel);

    //-

    // add toggle to control panel for this added gui panel
    params_Control.add( panels[last].state );

    //-

    // textBox pointers
    guis_OFX_TEXT_BOX.push_back(textBox);

    //-

    // add default draggeable point
    ofVec2f v2;
    v2.set(panels[last].position.get().x, panels[last].position.get().y);
    controlPoints.addPoint( v2 );

    //-
}
#endif

//-------------------------------------------------
void ofxGuiPanelsLayout::setup()
{
    ofLogNotice("ofxGuiPanelsLayout") << "setup()";

    // add group params to gui panel

    // 1. gui advanced

    params_ControlAdvanced.setName("ADVANCED");
    params_ControlAdvanced.add(group_Selected);
    params_ControlAdvanced.add(group_showAll);
    params_ControlAdvanced.add(group_hideAll);
    params_ControlAdvanced.add(group_Save);
    params_ControlAdvanced.add(group_autoLoad);
    params_ControlAdvanced.add(group_autoSave);
    params_ControlAdvanced.add(SHOW_draggable);
    params_ControlAdvanced.add(group_SaveAll);
    gui_Control_Advanced.setup(params_ControlAdvanced);
    gui_Control_Advanced.setPosition(5, 400);
    //params_ControlAdvanced.minimize();

    //-

    // 2. gui control toggles

    SHOW_advanced.set("ADVANCED", true);
    params_Control.add(SHOW_advanced);
    gui_Control.setup(params_Control);
    gui_Control.setPosition(ofGetWidth() - 205, 0);
    //gui_Control.loadFromFile("settings.xml");

    //-

    ofAddListener(params_Control.parameterChangedE(), this, &ofxGuiPanelsLayout::Changed_parameters);
    ofAddListener(params_ControlAdvanced.parameterChangedE(), this, &ofxGuiPanelsLayout::Changed_parametersAdvance);

    //-

    // erase groups

    groupStates.reserve(NUM_GROUPS);
    groupStates.resize(NUM_GROUPS);
    for (int i = 0; i < NUM_GROUPS; i++)
    {
        groupStates[i].resize(panels.size());
        for (int p = 0; p < panels.size(); p++)
        {
            groupStates[i][p] = false;
        }
    }

    //-

    // load json groups

    loadGroups();

    //-

    //    // default harcoded positions
    //
    //    controlPoints.clear();
    //
    //    for (int i = 0; i < panels.size(); i++)
    //    {
    //        float x, y;
    //        int pad = 5;
    //        int panelW = 200;
    //        x = (panelW+pad) + i * (panelW+pad);
    //        y = 10;
    //
    //        glm::vec2 v = glm::vec2( x, y );
    //        cout << "v:" << v << endl;
    //        panels[i].position = v;
    //
    //        controlPoints.addPoint(v);
    //    }

    //-

    // load json data positions

    loadPanels();

    //-

    // set positions

#ifdef USE_GUI_TYPE_OFX_GUI
    size_OFX_GUI = guis_OFX_GUI.size();
    ofLogVerbose("ofxGuiPanelsLayout") << "size_OFX_GUI " << size_OFX_GUI;
#endif
#ifdef USE_GUI_TYPE_OFX_DAT_GUI
    size_OFX_DAT = guis_OFX_DAT_GUI.size();
    ofLogVerbose("ofxGuiPanelsLayout") << "size_OFX_DAT " << size_OFX_DAT;
#endif
#ifdef USE_GUI_TYPE_TOGGLE_PARAM
    size_TOGGLES = guis_TOGGLE.size();
    ofLogVerbose("ofxGuiPanelsLayout") << "size_TOGGLES " << size_TOGGLES;
#endif
#ifdef USE_GUI_TYPE_TEXT_BOX
    size_OFX_TEXT_BOX = guis_OFX_TEXT_BOX.size();
    ofLogVerbose("ofxGuiPanelsLayout") << "size_OFX_TEXT_BOX " << size_OFX_TEXT_BOX;
#endif

    //    size_ALL_PANELS = size_ALL_GUIS + size_TOGGLES + size_OFX_TEXT_BOX;
    size_ALL_PANELS = size_ALL_GUIS;
#ifdef USE_GUI_TYPE_TOGGLE_PARAM
    size_ALL_PANELS += size_TOGGLES;
#endif
#ifdef USE_GUI_TYPE_TEXT_BOX
    size_ALL_PANELS += size_OFX_TEXT_BOX;
#endif
    ofLogVerbose("ofxGuiPanelsLayout") << "size_ALL_PANELS " << size_ALL_PANELS;


    //    size_ALL_GUIS = size_OFX_GUI + size_OFX_DAT;
    size_ALL_GUIS = size_OFX_GUI;
#ifdef USE_GUI_TYPE_OFX_DAT_GUI
    size_ALL_GUIS += size_OFX_DAT;
#endif
    ofLogVerbose("ofxGuiPanelsLayout") << "size_ALL_GUIS " << size_ALL_GUIS;

    int start, finish;

    //-

#ifdef USE_GUI_TYPE_OFX_GUI
    // 1. guis_OFX_GUI
    start = 0;
    finish = size_OFX_GUI;
    for (int i = 0; i < finish; i++)
    {
        guis_OFX_GUI[i]->setPosition(panels[i].position.get().x, panels[i].position.get().y);
    }
#endif

    //-

#ifdef USE_GUI_TYPE_OFX_DAT_GUI
    // 2. guis_OFX_DAT_GUI
    start = size_OFX_GUI;
    finish = size_ALL_GUIS;
    for (int i = start; i < finish; i++)
    {
        int first = i - start;
        guis_OFX_DAT_GUI[first]->setPosition( panels[i].position.get().x, panels[i].position.get().y );
    }
#endif

    //-

#ifdef USE_GUI_TYPE_TOGGLE_PARAM
    // 3. guis_TOGGLE
    start = size_ALL_GUIS;
    finish = size_ALL_GUIS + size_TOGGLES;
    for (int i = start; i < finish; i++)
    {
        // we do not need handle positions for toggles
    }
#endif

    //-

#ifdef USE_GUI_TYPE_TEXT_BOX
    // 4. size_OFX_TEXT_BOX
    start = size_ALL_GUIS + size_TOGGLES;
    finish = size_ALL_PANELS;
    for (int i = start; i < finish; i++)
    {
        int first = i - start;
        guis_OFX_TEXT_BOX[first]->setPosition( panels[i].position.get() );
    }
#endif

    //-
}

//-------------------------------------------
void ofxGuiPanelsLayout::refreshDraggables()
{
    //    // get positions from gui panels
    //    refreshPanels();

    // get drag points from store vector
    controlPoints.clear();
    for (int i = 0; i < panels.size(); i++)
    {
        controlPoints.addPoint(ofVec2f(panels[i].position));
    }
}

//-------------------------------------------
void ofxGuiPanelsLayout::update()
{
    if (SHOW_draggable)
    {
        vector<ofVec2f> dp = controlPoints.getPoints();
        for (int i = 0; i < dp.size(); i++)
        {
            panels[i].position = glm::vec2(dp[i]);
        }

        //-

        // set positions

        int start;
        int finish;

        // 1. guis_OFX_GUI
#ifdef USE_GUI_TYPE_OFX_GUI
        finish = size_OFX_GUI;
        for (int i = 0; i < finish; i++)
        {
            guis_OFX_GUI[i]->setPosition(panels[i].position.get().x, panels[i].position.get().y);
        }
#endif

        // 2. guis_OFX_DAT_GUI
#ifdef USE_GUI_TYPE_OFX_DAT_GUI
        start = size_OFX_GUI;
        finish = size_ALL_GUIS;
        for (int i = start; i < finish; i++)
        {
            int first = i - start;
            guis_OFX_DAT_GUI[i - start]->setPosition( panels[i].position.get().x, panels[i].position.get().y );
        }
#endif

        //    // 3. guis_TOGGLE
        //    for (int i = size_ALL_GUIS; i < size_ALL_GUIS + size_TOGGLES; i++)
        //    {
        //    }

        // 4. size_OFX_TEXT_BOX
#ifdef USE_GUI_TYPE_TEXT_BOX
        start = size_ALL_GUIS + size_TOGGLES;
        finish = size_ALL_PANELS;
        for (int i = start; i < finish; i++)
        {
            int first = i - start;
            guis_OFX_TEXT_BOX[first]->setPosition( panels[i].position.get() );
        }
#endif
        //-
    }
}

//-------------------------------------------
void ofxGuiPanelsLayout::draw()
{
    //-

    // draw gui control panels & drag points

    if (SHOW_controlPanel)
        gui_Control.draw();
    if (SHOW_advanced)
        gui_Control_Advanced.draw();
    if (SHOW_draggable)
        controlPoints.draw();

    //-

    // TODO: must use change_listeners only, not here
    // TODO: could use toggle setVisible to not do it every frame
    // TODO: mainly indatGui could use toggle setVisible to not do it every frame

    for (int i = 0; i < size_ALL_PANELS; i++)
    {
        int start;
        int finish;

        // 1. ofxGui's
#ifdef USE_GUI_TYPE_OFX_GUI
        start = 0;
        finish = size_OFX_GUI;
        if (i < finish)
        {
            if (panels[i].state)
                guis_OFX_GUI[i]->draw();
        }
#endif

        // 2. ofxDatGui's
#ifdef USE_GUI_TYPE_OFX_DAT_GUI
        // TODO: handle states with listeners not in all frames...
        start = size_OFX_GUI;
        finish = size_ALL_GUIS;
        if ( i>=start && i<finish )
        {
            int first = i-start;
            guis_OFX_DAT_GUI[first]->setVisible(panels[i].state);
            //            cout << "ofxDatGui:" << first << "VISIBLE" << endl;
        }
#endif

        // 3. toggle's
#ifdef USE_GUI_TYPE_TOGGLE_PARAM
        start = size_ALL_GUIS;
        finish = size_ALL_GUIS + size_TOGGLES;
        if (i >= start && i < finish)
        {

        }
#endif

        // 4. textBox's
#ifdef USE_GUI_TYPE_TEXT_BOX
        start = size_ALL_GUIS+size_TOGGLES;
        finish = size_ALL_PANELS;
        if ( i>=start && i<finish )
        {
            int first = i - start;

            if (panels[i].state)
            {
                guis_OFX_TEXT_BOX[first]->setVisible(true);
                guis_OFX_TEXT_BOX[first]->draw();
            }
            else
            {
                guis_OFX_TEXT_BOX[first]->setVisible(false);
            }
        }
#endif

        //-

    }

}

//-------------------------------------------
void ofxGuiPanelsLayout::addKeysListeners()
{
    ofAddListener(ofEvents().keyPressed, this, &ofxGuiPanelsLayout::keyPressed);
    ofAddListener(ofEvents().keyReleased, this, &ofxGuiPanelsLayout::keyReleased);
}

//-------------------------------------------
void ofxGuiPanelsLayout::removeKeysListeners()
{
    ofRemoveListener(ofEvents().keyPressed, this, &ofxGuiPanelsLayout::keyPressed);
    ofRemoveListener(ofEvents().keyReleased, this, &ofxGuiPanelsLayout::keyReleased);
}

//-------------------------------------------
void ofxGuiPanelsLayout::keyPressed(ofKeyEventArgs &eventArgs)
{
    const int &key = eventArgs.key;
    //    ofLogNotice("ofxGuiPanelsLayout") << "key: " << key;

    if (key == ' ')
    {
    }

    //	if (key == OF_KEY_BACKSPACE) {
    //        ofxTextFlow::clear();
    //    }
    //    else
    //    {
    //        ofxTextFlow::addText("key code: " + ofToString(key));
    //    }

    if (key == 'm')
    {
#ifdef USE_MOUSE_RULER
        mouseRuler.toggleVisibility();
#endif
    }

}

//-------------------------------------------
void ofxGuiPanelsLayout::keyReleased(ofKeyEventArgs &eventArgs)
{
    if (eventArgs.key == ' ')
    {
    }
}

//-------------------------------------------
//void ofxGuiPanelsLayout::mousePressed( int x, int y )
//{
//}

//-------------------------------------------
void ofxGuiPanelsLayout::Changed_parameters(ofAbstractParameter &e)
{
    string WIDGET_name = e.getName();
    //    ofLogNotice("ofxGuiPanelsLayout") << "Changed_parameters: " << WIDGET_name << ": " << e;

    int start;
    int finish;

    for (int p = 0; p < panels.size(); p++)
    {
        if (WIDGET_name == panels[p].name.get())
        {
            ofLogVerbose("ofxGuiPanelsLayout") << "PANEL: " << WIDGET_name << ": " << e;

            //            if (WIDGET_name == "DEBUG CAMERAS")
            //            {
            //                ofLogVerbose("ofxGuiPanelsLayout") << "+ DEBUG CAMERAS DETECTED";
            //                if (panels[size_ALL_GUIS+size_TOGGLES].state)
            //                {
            //                    guis_OFX_TEXT_BOX[0]->setVisible(true);
            //                }
            //                else
            //                {
            //                    guis_OFX_TEXT_BOX[0]->setVisible(false);
            //                }
            //            }

            //-

            // handle toggles

            start = size_ALL_GUIS;
            finish = size_ALL_GUIS + size_TOGGLES;
            for (int i = start; i < finish; i++)
            {
                int first = i - start;
                if (WIDGET_name == guis_TOGGLE[first]->getName())
                {
                    //ofLogVerbose("ofxGuiPanelsLayout") << "+ TOGGLE_" << ofToString(first) << " DETECTED";
                    if (panels[i].state)
                    {
                        guis_TOGGLE[first]->set(true);
                    }
                    else
                    {
                        guis_TOGGLE[first]->set(false);
                    }
                }

            }
        }
    }
}

//-------------------------------------------
void ofxGuiPanelsLayout::Changed_parametersAdvance(ofAbstractParameter &e)
{
    string WIDGET_name = e.getName();
    //    ofLogNotice("ofxGuiPanelsLayout") << "Changed_parametersAdvance: " << WIDGET_name << ": " << e;

    // save group of panels states

    if (WIDGET_name == "save" && group_Save)
    {
        ofLogVerbose("ofxGuiPanelsLayout") << "Changed_parametersAdvance: " << WIDGET_name << ": " << e;
        group_Save = false;

        for (int p = 0; p < panels.size(); p++)
        {
            groupStates[group_Selected][p] = panels[p].state;
        }
    }

        //-

        // load group (all panels states)

    else if (WIDGET_name == "APP MODE" && (group_Selected_PRE != group_Selected))
    {
        ofLogNotice("ofxGuiPanelsLayout") << "Changed_parametersAdvance: " << WIDGET_name << ": " << e;

        if (group_Selected < 0)
            group_Selected = 0;
        else if (group_Selected > NUM_GROUPS - 1)
            group_Selected = NUM_GROUPS - 1;

        // auto save
        for (int p = 0; p < panels.size() && group_autoSave; p++)
        {
            groupStates[group_Selected_PRE][p] = panels[p].state;
        }
        group_Selected_PRE = group_Selected;

        if (group_autoLoad)
        {
            // auto load
            for (int p = 0; p < panels.size(); p++)
            {
                panels[p].state = groupStates[group_Selected][p];
            }
        }
    }

        //-

        // show draggable

    else if (WIDGET_name == "show draggable")
    {
        ofLogVerbose("ofxGuiPanelsLayout") << "Changed_parametersAdvance: " << WIDGET_name << ": " << e;

        if (SHOW_draggable)
        {
            refreshPanels();
            refreshDraggables();
        }
        else
        {
            savePanels();
        }
    }

        //-

        // show all

    else if (WIDGET_name == "show all")
    {
        ofLogVerbose("ofxGuiPanelsLayout") << "Changed_parametersAdvance: " << WIDGET_name << ": " << e;
        for (int i = 0; i < panels.size(); i++)
        {
            panels[i].state = group_showAll;
        }
    }

        //-

        // hide all

    else if (WIDGET_name == "hide all" && group_hideAll)
    {
        ofLogVerbose("ofxGuiPanelsLayout") << "Changed_parametersAdvance: " << WIDGET_name << ": " << e;
        for (int i = 0; i < panels.size(); i++)
        {
            group_hideAll = false;
            group_showAll = false;//will auto call hide in show all listener
            //            panels[i].state = false;
        }
    }
        //-

        // save all settings like on exit

    else if (WIDGET_name == "save all")
    {
        if (group_SaveAll)
        {
            group_SaveAll = false;
            ofLogVerbose("ofxGuiPanelsLayout") << "Changed_parametersAdvance: " << WIDGET_name << ": " << e;
            savePanels();
            saveGroups();
        }
    }

    //-

}

//-------------------------------------------
void ofxGuiPanelsLayout::savePanels()
{
    // get positions from gui panels to storing vector
    refreshPanels();

    //-

    // 1. panels

    ofLogNotice("ofxGuiPanelsLayout") << "savePanels()";
    ofJson js;

    for (int p = 0; p < panels.size(); p++)
    {
        // TODO: must refresh gui position from outside class..
        //because can has been moved..

        ofJson pt;
        pt["panel"] = panels[p].name.get();
        pt["x"] = (int) panels[p].position->x;
        pt["y"] = (int) panels[p].position->y;
        js.push_back(pt);
    }
    ofSavePrettyJson(pathFolder + "panels_positions.json", js);

    //-

    // 2. control

    gui_Control_pos = gui_Control.getPosition();
    gui_Control_Advanced_pos = gui_Control_Advanced.getPosition();

    ofLogNotice("ofxGuiPanelsLayout") << "save control positions";
    ofJson jsC;
    bool b = SHOW_advanced;
    jsC["advanced mode"] = b;
    jsC["control_x"] = (int) gui_Control_pos.x;
    jsC["control_y"] = (int) gui_Control_pos.y;
    jsC["advanced_x"] = (int) gui_Control_Advanced_pos.x;
    jsC["advanced_y"] = (int) gui_Control_Advanced_pos.y;
    jsC["group_Selected"] = (int) group_Selected.get();
    ofSavePrettyJson(pathFolder + "control_positions.json", jsC);

    //-
}

//-------------------------------------------
void ofxGuiPanelsLayout::loadPanels()
{
    // load positions from json

    ofLogNotice("ofxGuiPanelsLayout") << "loadPanels()";

    ofJson js;
    ofFile file(pathFolder + "panels_positions.json");
    if (file.exists())
    {
        file >> js;
        ofLogVerbose("ofxGuiPanelsLayout") << js;
        ofLogVerbose("ofxGuiPanelsLayout") << endl << "-----" << endl;

        int i = 0;
        for (auto &position: js)
        {
            if (!position.empty() && (i < panels.size()))
            {
                float x, y;
                x = position["x"];
                y = position["y"];
                std::string name = position["panel"];
                ofLogVerbose("ofxGuiPanelsLayout") << "PANEL:" << name << " x:" << x << " y:" << y << endl;

                glm::vec2 v = glm::vec2(x, y);
                panels[i].position = v;
            }
            i++;
        }
    }
    else
    {
        ofLogError() << "json file not found";
    }

    //-

    ofLogNotice("ofxGuiPanelsLayout") << "load control positions";
    ofFile file2(pathFolder + "control_positions.json");
    if (file2.exists())
    {
        ofJson jsC;
        file2 >> jsC;
        bool b = jsC["advanced mode"];
        SHOW_advanced.set(b);
        gui_Control_pos.x = (int) jsC["control_x"];
        gui_Control_pos.y = (int) jsC["control_y"];
        gui_Control_Advanced_pos.x = (int) jsC["advanced_x"];
        gui_Control_Advanced_pos.y = (int) jsC["advanced_y"];

        group_Selected = (int) jsC["group_Selected"];//recall selected group/app mode on last session
        ofLogNotice("ofxGuiPanelsLayout") << "group_Selected:" << group_Selected;

        gui_Control.setPosition(gui_Control_pos.x, gui_Control_pos.y);
        gui_Control_Advanced.setPosition(gui_Control_Advanced_pos.x, gui_Control_Advanced_pos.y);
    }
    else
    {
        ofLogError() << "json file not found";
    }

    //-

    // set drag points from storing panels vector

    refreshDraggables();

    //-
}

//-------------------------------------------
void ofxGuiPanelsLayout::refreshPanels()
{
    // get position from gui panels to storing vector
    // must refresh gui position from outside class..
    //because can has been moved..

    int start;
    int finish;

    for (int i = 0; i < size_ALL_PANELS; i++)
    {
        glm::vec2 pos;

        // 1. guis_OFX_GUI
#ifdef USE_GUI_TYPE_OFX_GUI
        if (i < size_OFX_GUI)
        {
            pos = glm::vec2(guis_OFX_GUI[i]->getPosition().x,
                guis_OFX_GUI[i]->getPosition().y);
        }
#endif

        // 2. guis_OFX_DAT_GUI
#ifdef USE_GUI_TYPE_OFX_DAT_GUI
        start = size_OFX_GUI;
        finish = size_ALL_GUIS;
        if ( i>=start && i<finish )
        {
            int first = i - start;
            pos = glm::vec2 ( guis_OFX_DAT_GUI[first]->getPosition().x,guis_OFX_DAT_GUI[first]->getPosition().y );
        }
#endif

        //        // 3. guis_TOGGLE
        //        start = size_ALL_GUIS;
        //        finish = size_ALL_GUIS+size_TOGGLES;
        //        if ( i>=start && i<finish )
        //        {
        //        }

        // 4. size_OFX_TEXT_BOX
#ifdef USE_GUI_TYPE_TEXT_BOX
        start = size_ALL_GUIS+size_TOGGLES;
        finish = size_ALL_PANELS;
        if ( i>=start && i<finish )
        {
            int first = i - start;
//            guis_OFX_TEXT_BOX[first]->setPosition( panels[i].position.get() );

            pos = guis_OFX_TEXT_BOX[first]->getPosition();
        }
#endif
        //-

        // apply readed positions from objects
        panels[i].position = pos;

        //-
    }
}

//-------------------------------------------
void ofxGuiPanelsLayout::saveGroups()
{
    ofLogNotice("ofxGuiPanelsLayout") << "saveGroups()";
    ofJson jsKit;

    for (int g = 0; g < NUM_GROUPS; g++)
    {
        ofJson jsG;
        for (int p = 0; p < panels.size(); p++)
        {
            ofJson jsP;
            jsP[ofToString(p)] = groupStates[g][p];
            jsG.push_back(jsP);
        }
        jsKit.push_back(jsG);
    }
    ofSavePrettyJson(pathFolder + "group_states.json", jsKit);
}

//-------------------------------------------
void ofxGuiPanelsLayout::loadGroups()
{
    ofLogNotice("ofxGuiPanelsLayout") << "loadGroups()";
    ofJson js;

    ofFile file(pathFolder + "group_states.json");
    if (file.exists())
    {
        file >> js;
        int g = 0;
        for (auto &stroke: js)
        {
            if (!stroke.empty())
            {
                int pan = 0;
                for (auto &p: stroke)
                {
                    groupStates[g][pan] = p[ofToString(pan)];
                    pan++;
                }
            }
            g++;
        }
    }

    // auto load
    for (int p = 0; p < panels.size(); p++)
    {
        panels[p].state = groupStates[group_Selected][p];
    }
}

//-------------------------------------------
void ofxGuiPanelsLayout::exit()
{
    if (group_autoSave)
        group_Save = true;
    savePanels();
    saveGroups();

    removeKeysListeners();
}

//-------------------------------------------
ofxGuiPanelsLayout::~ofxGuiPanelsLayout()
{
    ofRemoveListener(params_Control.parameterChangedE(), this, &ofxGuiPanelsLayout::Changed_parameters);
    ofRemoveListener(params_ControlAdvanced.parameterChangedE(), this, &ofxGuiPanelsLayout::Changed_parametersAdvance);
}
