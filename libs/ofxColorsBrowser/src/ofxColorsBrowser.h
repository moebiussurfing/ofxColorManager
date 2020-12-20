#pragma once

#include "ofMain.h"

#include "ofxOpenColor.h"
#include "ofxRectangleUtils.h"
#include "ofxRectangle.h"
#include "ofxFontStash.h"

using namespace ofx;

// internal shorcuts no add listeners or remove
#define KEY_SHORTCUTS_ENABLE

// sortings
enum
{
    SORTING_ORIGINAL,
    SORTING_NAME,
    SORTING_HUE,
    SORTING_BRIGHTNESS,
    SORTING_SATURATION
};

// palettes
enum
{
    OFX_PANTONE_COLORS,
    OFX_COLOR_NATIVE,
    OFX_OPEN_COLOR
};

// color struct
typedef struct
{
    string name;
    ofColor color;
    int position;//original position
} colorMapping_STRUCT;

//--

class ofxColorsBrowser
{

public:

    ofxFontStash font;

    ofxColorsBrowser();
    ~ofxColorsBrowser();

    void setup();
    void update();
    void draw();
    void exit();

    //----

    // API

    void load_Pantone_JSON();

    void setPosition(glm::vec2 p);
    void setPositionHelper(glm::vec2 p)
    {
        positionHelper = p;
    };

    void switch_palette_Type();
    void switch_sorted_Type();
    void set_palette_Type(int p);
    void set_sorted_Type(int p);

    // TODO: resize buttons to fit..
    void setSize(glm::vec2 size);
    void setRowsSize(int rows);
    void setBoxSize(float size);

    void setVisible(bool b);
    void setVisible_debugText(bool b);

    void set_ENABLE_clicks(bool b)
    {
        ENABLE_clicks = b;
        if (b)
        {
            addKeysListeners();
            addMouseListeners();
        }
        else
        {
            removeKeysListeners();
            removeMouseListeners();
        }
    }

    void set_ENABLE_keys(bool b)
    {
        ENABLE_keys = b;
        if (ENABLE_keys)
            addKeysListeners();
        else
            removeKeysListeners();
    }

    // pointer back
    void setup_colorBACK(ofFloatColor &c);

    // main palette getter
    vector<ofColor> getPalette();

    vector<std::string> pantoneNames;

    //--

private:

    // path for json colors file
    string path = "colorsBrowser/pantone-colors.json";

    //-

    // SETTINGS

    ofParameter<float> boxSize{"BOX SIZE", 15, 10, 100};//boxes
    ofParameter<float> boxPad{"PAD", 1, 0, 10};
    ofParameter<int> cardSize{"CARD SIZE", 7, 2, 100};// minimal card of colors
    ofParameter<int> cardsPerRow{"CARDS PER ROW", 4, 2, 100};
    ofParameter<bool> ENABLE_oneCard_MODE{"ONE CARD MODE", true};
    int perRow = 10;

    int cardNum = 0;
    int cardColor_size = 100;
    int cardColor_pad = 20;
    glm::vec2 cardPos;

    //--

    // MAIN STORAGE

    map<string, ofColor> colorNameMap;
    vector<colorMapping_STRUCT> colors_STRUCT;

    //--

    // PANTONE COLORS

    ofJson js;
    vector<ofColor> pantoneColors;


    //-

    // RECTANGLE MANAGER SYSTEM - OFXRECTANGLE
    //draggable, sortable, align...
    std::vector<ofxRectangle> rectangles;
    std::vector<ofRectangle *> selectedRects;
    ofxRectangle selectedRectsBoundingBox;
    ofxRectangle *draggingRectPtr;
    glm::vec2 dragStart;
    bool isSelecting;
    ofRectangle selectionRect;
    ofAlignHorz hAlign;
    ofAlignVert vAlign;
    ofRectangle *anchorRect;
    string keyboardCommands;
    bool showKeyboardCommands;
    std::vector<ofRectangle> packedRects;
    void rectangles_update();
    void rectangles_draw();

    //-

    // color converters
    // These string to hex conversions aren't trivial.
    static int stringToHex(string hex)
    {
        int aHex;
        stringstream convert(hex);
        convert >> std::hex >> aHex;
        return aHex;
    }
    static void hexToColor(ofColor &col, string hex)
    {
        string r = hex.substr(0, 2);
        int ri = stringToHex(r);
        string g = hex.substr(2, 2);
        int gi = stringToHex(g);
        string b = hex.substr(4, 2);
        int bi = stringToHex(b);
        col.set(ri, gi, bi);
    }

    //-

    void keyPressed(ofKeyEventArgs &eventArgs);
    void keyReleased(ofKeyEventArgs &eventArgs);
    void addKeysListeners();
    void removeKeysListeners();

    void mouseDragged(ofMouseEventArgs &eventArgs);
    void mousePressed(ofMouseEventArgs &eventArgs);
    void mouseReleased(ofMouseEventArgs &eventArgs);
    void addMouseListeners();
    void removeMouseListeners();

    //-

    // modes and states

    bool SHOW_debugText = false;
    bool SHOW_ColorsBrowse = true;
    bool ENABLE_clicks = true;
    bool ENABLE_keys = false;

    bool bShowDebug = false;//for rectangle manager

    //-

    // pointers back

    ofFloatColor color_BACK;
    ofFloatColor color_BACK_PRE;
    ofFloatColor *color_BACK_OFAPP;

    //-

    void refresh_Clicks();// to browsing by keys
    void grid_generate();
    void grid_create_boxes();
    void grid_clear();

    //-

    // grid position
    glm::vec2 position;
    // text debug positions
    glm::vec2 positionHelper;

    // 0:PANTONE COLORS 1:OFX_COLOR_NATIVE, 2:OFX_OPEN_COLOR
    int MODE_COLOR;

    // 0:ORIGINAL, 1:NAME, 2:HUE, 3:BRIGHTNESS, 4:SATURATION, 5:NEXT
    int MODE_SORTING;

    // last clicked color box
    string currName = "";
    int currColor = -1;
    int currColor_OriginalPos = -1;
};
