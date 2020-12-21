//
//  colorSpace.h
//  colorStudy_day6
//
//  Created by Makoto Amano on 2020/02/20.
//

#ifndef colorSpace_h
#define colorSpace_h

namespace ofx{
namespace colorMorph{

#define M_PI 3.14159265358979323846264338327950288

enum class colorType{
    RGB, HSL, HSV, HSB, LUV, LAB, HunterLAB, LCH, CMY, CMYK, YXY, XYZ
};

namespace detail{
template <typename T>
T calcColor(T a, T b, float ratio){
    T color = glm::mix(a, b, ratio);
    return color;
}

inline float distance(const glm::vec3& pos1, const glm::vec3& pos2){
    float distance = sqrt(pow(pos2.x - pos1.x, 2.0) + pow(pos2.y - pos1.y, 2.0) + pow(pos2.z - pos1.z, 2.0));
    return distance;
}
}//namespace detail

namespace convert{
const static float eps = 216.0 / 24389.0;
const static float kappa = 24389.0 / 27.0;
const static glm::vec3 whiteReference= {95.047, 100.000, 108.883};

//--------------------------------------------------------------
inline float HUE2RGB(float v1, float v2, float vh){
    if (vh < 0) vh += 1;
    if (vh > 1) vh -= 1;
    if (6 * vh < 1) return v1 + (v2 - v1) * 6 * vh;
    if (2 * vh < 1) return v2;
    if (3 * vh < 2) return v1 + (v2 - v1)*(2.0 / 3.0 - vh) * 6;
    return v1;
}

//--------------------------------------------------------------
inline glm::vec3 RGB2XYZ(const ofColor& color){
    glm::vec3 XYZ;
    float r = color.r / 255.0;
    float g = color.g / 255.0;
    float b = color.b / 255.0;

    r = ((r > 0.04045) ? pow((r + 0.055) / 1.055, 2.4) : (r / 12.92)) * 100.0;
    g = ((g > 0.04045) ? pow((g + 0.055) / 1.055, 2.4) : (g / 12.92)) * 100.0;
    b = ((b > 0.04045) ? pow((b + 0.055) / 1.055, 2.4) : (b / 12.92)) * 100.0;

    XYZ.x = r*0.4124564 + g*0.3575761 + b*0.1804375;
    XYZ.y = r*0.2126729 + g*0.7151522 + b*0.0721750;
    XYZ.z = r*0.0193339 + g*0.1191920 + b*0.9503041;
    return XYZ;
}

inline ofColor   XYZ2RGB(const glm::vec3& XYZ){
    ofColor color;
    float x = XYZ.x / 100.0;
    float y = XYZ.y / 100.0;
    float z = XYZ.z / 100.0;

    float r = x * 3.2404542 + y * -1.5371385 + z * -0.4985314;
    float g = x * -0.9692660 + y * 1.8760108 + z * 0.0415560;
    float b = x * 0.0556434 + y * -0.2040259 + z * 1.0572252;

    r = ((r > 0.0031308) ? (1.055*pow(r, 1 / 2.4) - 0.055) : (12.92*r)) * 255.0;
    g = ((g > 0.0031308) ? (1.055*pow(g, 1 / 2.4) - 0.055) : (12.92*g)) * 255.0;
    b = ((b > 0.0031308) ? (1.055*pow(b, 1 / 2.4) - 0.055) : (12.92*b)) * 255.0;
    
    color.r = r;
    color.g = g;
    color.b = b;
    return color;
}

//--------------------------------------------------------------
inline ofColor RGB2RGB(const ofColor& color){
    return color;
}

//--------------------------------------------------------------
inline glm::vec3 RGB2HSL(const ofColor& color){
    glm::vec3 HSL;
    float r = color.r / 255.0;
    float g = color.g / 255.0;
    float b = color.b / 255.0;

    float min = std::min(r, std::min(g, b));
    float max = std::max(r, std::max(g, b));
    float delta = max - min;
    
    HSL.z = (max + min) / 2;
    if (delta == 0){
        HSL.x = HSL.y = 0;
    }
    else {
        if (HSL.z < 0.5) {
            HSL.y = delta / (max + min) * 100;
        }
        else {
            HSL.y = delta / (1 - abs(2 * HSL.z - 1)) * 100;
        }

        if (r == max) {
            HSL.x =  (g - b) / delta;
        }else if (g == max) {
            HSL.x = (b - r) / delta + 2;
        }else if (b == max) {
            HSL.x = (r - g) / delta + 4;
        }
        HSL.x = fmod(60 * HSL.x + 360, 360);
    }
    HSL.z *= 100;
    
    return HSL;
}

inline ofColor   HSL2RGB(const glm::vec3& HSL){
    ofColor color;
    float h = HSL.x / 360;
    float s = HSL.y / 100;
    float l = HSL.z / 100;

    if (HSL.y == 0) {
        color.r = color.g = color.b = HSL.z * 255;
    }else {
        float temp1, temp2;

        temp2 = (l < 0.5) ? (l*(1 + s)) : (l + s - (s*l));
        temp1 = 2 * l - temp2;

        color.r = 255 * HUE2RGB(temp1, temp2, h + 1.0 / 3.0);
        color.g = 255 * HUE2RGB(temp1, temp2, h);
        color.b = 255 * HUE2RGB(temp1, temp2, h - 1.0 / 3.0);
    }
    return color;
}

//--------------------------------------------------------------
inline glm::vec3 RGB2HSV(const ofColor& color){
    glm::vec3 HSV;
    float r = color.r / 255.0;
    float g = color.g / 255.0;
    float b = color.b / 255.0;
    
    float min = std::min(r, std::min(g, b));
    float max = std::max(r, std::max(g, b));
    float delta = max - min;
    
    HSV.z = max;
    HSV.y = (max > 1e-3) ? (delta / max) : 0;
    
    if (delta == 0) {
        HSV.x = 0;
    }
    else {
        if (r == max) {
            HSV.x = (g - b) / delta;
        }
        else if (g == max) {
            HSV.x = 2 + (b - r) / delta;
        }
        else if (b == max) {
            HSV.x = 4 + (r - g) / delta;
        }
        
        HSV.x *= 60;
        HSV.x = fmod(HSV.x + 360, 360);
    }
    return HSV;
}

inline ofColor HSV2RGB(const glm::vec3& HSV){
    ofColor color;
    int range = (int)floor(HSV.x / 60);
    float c = HSV.z*HSV.y;
    float x = c*(1 - std::abs(fmod(HSV.x / 60, 2) - 1));
    float m = HSV.z - c;
    
    switch (range) {
        case 0:
            color.r = (c + m) * 255;
            color.g = (x + m) * 255;
            color.b = m * 255;
            break;
        case 1:
            color.r = (x + m) * 255;
            color.g = (c + m) * 255;
            color.b = m * 255;
            break;
        case 2:
            color.r = m * 255;
            color.g = (c + m) * 255;
            color.b = (x + m) * 255;
            break;
        case 3:
            color.r = m * 255;
            color.g = (x + m) * 255;
            color.b = (c + m) * 255;
            break;
        case 4:
            color.r = (x + m) * 255;
            color.g = m * 255;
            color.b = (c + m) * 255;
            break;
        default:        // case 5:
            color.r = (c + m) * 255;
            color.g = m * 255;
            color.b = (x + m) * 255;
            break;
    }
    return color;
}

//--------------------------------------------------------------
inline glm::vec3 RGB2HSB(const ofColor& color){
    glm::vec3 HSB;
    glm::vec3 HSV;
    
    HSV = RGB2HSV(color);
    
    HSB.x = HSV.x;
    HSB.y = HSV.y;
    HSB.z = HSV.z;
    return HSB;
}

inline ofColor HSB2RGB(const glm::vec3& HSB){
    ofColor color;
    glm::vec3 HSV;
    
    HSV.x = HSB.x;
    HSV.y = HSB.y;
    HSV.z = HSB.z;
    color = HSV2RGB(HSV);
    return color;
}

//--------------------------------------------------------------
inline glm::vec3 RGB2LUV(const ofColor& color){
    glm::vec3 LUV;
    const glm::vec3 white = whiteReference;
    glm::vec3 XYZ;
    
    XYZ = RGB2XYZ(color);
    
    float y = XYZ.y / white.y;
    float temp = (XYZ.x + 15 * XYZ.y + 3 * XYZ.z);
    float tempr = (white.x + 15 * white.y + 3 * white.z);
    
    
    LUV.x = (y > eps) ? (116 * cbrt(y) - 16) : (kappa*y);
    LUV.y = 52 * LUV.x * (((temp > 1e-3) ? (XYZ.x / temp) : 0) - white.x / tempr);
    LUV.z = 117 * LUV.x * (((temp > 1e-3) ? (XYZ.y / temp) : 0) - white.y / tempr);
    return LUV;
}

inline ofColor LUV2RGB(const glm::vec3 LUV){
    ofColor color;
    glm::vec3 white = whiteReference;
    glm::vec3 XYZ;
    
    float y = (LUV.x > eps*kappa) ? pow((LUV.x + 16) / 116, 3) : (LUV.x / kappa);
    float tempr = white.x + 15 * white.y + 3 * white.z;
    float up = 4 * white.x / tempr;
    float vp = 9 * white.y / tempr;
    
    float a = 1. / 3. * (52 * LUV.x / (LUV.y+ 13 * LUV.x*up) - 1);
    float b = -5 * y;
    float x = (y*(39 * LUV.x / (LUV.z + 13 * LUV.x*vp) - 5) - b) / (a + 1. / 3.);
    float z = x*a + b;
    
    XYZ.x = x * 100;
    XYZ.y = y * 100;
    XYZ.z = z * 100;
    
    color = XYZ2RGB(XYZ);
    return color;
}

//--------------------------------------------------------------
inline glm::vec3 RGB2LAB(const ofColor& color){
    glm::vec3 LAB;
    glm::vec3 XYZ;
    
    XYZ = RGB2XYZ(color);

    float x = XYZ.x / 95.047;
    float y = XYZ.y / 100.00;
    float z = XYZ.z / 108.883;

    x = (x > 0.008856) ? cbrt(x) : (7.787 * x + 16.0 / 116.0);
    y = (y > 0.008856) ? cbrt(y) : (7.787 * y + 16.0 / 116.0);
    z = (z > 0.008856) ? cbrt(z) : (7.787 * z + 16.0 / 116.0);

    LAB.x = (116.0 * y) - 16;
    LAB.y = 500 * (x - y);
    LAB.z = 200 * (y - z);
    return LAB;
}

inline ofColor LAB2RGB(const glm::vec3& LAB){
    ofColor color;
    float y = (LAB.x + 16.0) / 116.0;
    float x = LAB.y / 500.0 + y;
    float z = y - LAB.z / 200.0;
    
    float x3 = pow(x, 3.);
    float y3 = pow(y, 3.);
    float z3 = pow(z, 3.);
    
    x = ((x3 > 0.008856) ? x3 : ((x - 16.0 / 116.0) / 7.787)) * 95.047;
    y = ((y3 > 0.008856) ? y3 : ((y - 16.0 / 116.0) / 7.787)) * 100.0;
    z = ((z3 > 0.008856) ? z3 : ((z - 16.0 / 116.0) / 7.787)) * 108.883;
    
    glm::vec3 XYZ = {x, y, z};
    color = XYZ2RGB(XYZ);
    return color;
}

//--------------------------------------------------------------
inline glm::vec3 RGB2HunterLAB(const ofColor& color){
    glm::vec3 HunterLAB;
    glm::vec3 XYZ;
    
    XYZ = RGB2XYZ(color);
    
    HunterLAB.x = 10.0*sqrt(XYZ.y);
    HunterLAB.y = (XYZ.y != 0) ? (17.5*(1.02*XYZ.x - XYZ.y) / sqrt(XYZ.y)) : 0;
    HunterLAB.z = (XYZ.y != 0) ? (7.0*(XYZ.y - 0.847*XYZ.z) / sqrt(XYZ.y)) : 0;
    return HunterLAB;
}

inline ofColor HunterLAB2RGB(const glm::vec3& HunterLAB){
    ofColor color;
    float x = (HunterLAB.y / 17.5) *(HunterLAB.x / 10.0);
    float y = HunterLAB.x*HunterLAB.x / 100;
    float z = HunterLAB.z / 7.0 * HunterLAB.x / 10.0;
    
    glm::vec3 XYZ = {(x+y)/1.02, y, -(z-y)/0.847};
    color = XYZ2RGB(XYZ);
    return color;
}

//--------------------------------------------------------------
inline glm::vec3 RGB2LCH(const ofColor& color){
    glm::vec3 LCH;
    glm::vec3 LAB;
    LAB = RGB2LAB(color);
    
    float l = LAB.x;
    float c = sqrt(LAB.y*LAB.y + LAB.z*LAB.z);
    float h = atan2(LAB.z, LAB.y);
    
    h = h / M_PI * 180.;
    if (h < 0) {
        h += 360.;
    }
    else if (h >= 360.) {
        h -= 360.;
    }
    LCH.x = l;
    LCH.y = c;
    LCH.z = h;
    return LCH;
}

inline ofColor   LCH2RGB(const glm::vec3& LCH){
    ofColor color;
    glm::vec3 LAB, LCH_;
    LCH_ = LCH;
    
    LCH_.z = LCH.z * (M_PI / 180.);
    
    LAB.x = LCH_.x;
    LAB.y = cosf(LCH_.z)*LCH_.y;
    LAB.z = sinf(LCH_.z)*LCH_.y;
    
    color = LAB2RGB(LAB);
    return color;
}

//--------------------------------------------------------------
inline glm::vec3 RGB2CMY(const ofColor& color){
    glm::vec3 CMY;
    CMY.x = 1. - color.r / 255.;
    CMY.y = 1. - color.g / 255.;
    CMY.z = 1. - color.b / 255.;
    return CMY;
}

inline ofColor CMY2RGB(const glm::vec3 CMY){
    ofColor color;
    color.r = (1. - CMY.x) * 255;
    color.g = (1. - CMY.y) * 255;
    color.b = (1. - CMY.z) * 255;
    return color;
}

//--------------------------------------------------------------
inline glm::vec4 RGB2CMYK(const ofColor& color){
    glm::vec4 CMYK;
    
    float r = color.r / 255.;
    float g = color.g / 255.;
    float b = color.b / 255.;
    
    float k = 1.0;
    k = std::min(k, 1 - r);
    k = std::min(k, 1 - g);
    k = std::min(k, 1 - b);
    
    CMYK.a = k;
    
    CMYK.r = (1. - r - k) / (1. - k);
    CMYK.g = (1. - g - k) / (1. - k);
    CMYK.b = (1. - b - k) / (1. - k);
    return CMYK;
}

inline ofColor CMYK2RGB(const glm::vec4 CMYK){
    ofColor color;
    float r = 1. - (CMYK.r*(1. - CMYK.a) + CMYK.a);
    float g = 1. - (CMYK.g*(1. - CMYK.a) + CMYK.a);
    float b = 1. - (CMYK.b*(1. - CMYK.a) + CMYK.a);
    color.r = r * 255;
    color.g = g * 255;
    color.b = b * 255;
    return color;
}

//--------------------------------------------------------------
inline glm::vec3 RGB2YXY(const ofColor& color){
    glm::vec3 YXY;
    glm::vec3 XYZ;
    
    XYZ = RGB2XYZ(color);
    
    float temp = XYZ.x + XYZ.y + XYZ.z;
    YXY.x = XYZ.y;
    YXY.y = (temp==0) ? 0 : (XYZ.x / temp);
    YXY.z = (temp==0) ? 0 : (XYZ.y / temp);
    return YXY;
}

inline ofColor YXY2RGB(const glm::vec3& YXY){
    ofColor color;
    glm::vec3 RGB;
    glm::vec3 XYZ;
           
    XYZ.x = YXY.y*(YXY.x / YXY.z);
    XYZ.y = YXY.x;
    XYZ.z = (1 - YXY.y - YXY.z)*(YXY.x / YXY.z);
    color = XYZ2RGB(XYZ);
    return color;
}

}//namespace convert

namespace morph{

inline ofColor colorMorph(const glm::vec3& target, const glm::vec3& pos1, const ofColor& col1, const glm::vec3& pos2, const ofColor& col2, colorType type){
    ofColor color;
    float distance = detail::distance(pos1, pos2);
    float targetDistance   = detail::distance(target, pos1);
    float ratio = targetDistance / distance;
    
    if(type == colorType::RGB){
        color = col1.getLerped(col2, ratio);
    }else if(type == colorType::HSL){
        glm::vec3 HSL, HSL1, HSL2;
        HSL1  = convert::RGB2HSL(col1);
        HSL2  = convert::RGB2HSL(col2);
        HSL   = detail::calcColor(HSL1, HSL2, ratio);
        color = convert::HSL2RGB(HSL);
    }else if(type == colorType::HSV){
        glm::vec3 HSV, HSV1, HSV2;
        HSV1  = convert::RGB2HSV(col1);
        HSV2  = convert::RGB2HSV(col2);
        HSV   = detail::calcColor(HSV1, HSV2, ratio);
        color = convert::HSV2RGB(HSV);
    }else if(type == colorType::HSB){
        glm::vec3 HSB, HSB1, HSB2;
        HSB1  = convert::RGB2HSB(col1);
        HSB2  = convert::RGB2HSB(col2);
        HSB   = detail::calcColor(HSB1, HSB2, ratio);
        color = convert::HSB2RGB(HSB);
    }else if(type == colorType::LUV){
        glm::vec3 LUV, LUV1, LUV2;
        LUV1  = convert::RGB2LUV(col1);
        LUV2  = convert::RGB2LUV(col2);
        LUV   = detail::calcColor(LUV1, LUV2, ratio);
        color = convert::LUV2RGB(LUV);
    }else if(type == colorType::LAB){
        glm::vec3 LAB, LAB1, LAB2;
        LAB1  = convert::RGB2LAB(col1);
        LAB2  = convert::RGB2LAB(col2);
        LAB   = detail::calcColor(LAB1, LAB2, ratio);
        color = convert::LAB2RGB(LAB);
    }else if(type == colorType::HunterLAB){
        glm::vec3 HunterLAB, HunterLAB1, HunterLAB2;
        HunterLAB1  = convert::RGB2HunterLAB(col1);
        HunterLAB2  = convert::RGB2HunterLAB(col2);
        HunterLAB   = detail::calcColor(HunterLAB1, HunterLAB2, ratio);
        color       = convert::HunterLAB2RGB(HunterLAB);
    }else if(type == colorType::LCH){
        glm::vec3 LCH, LCH1, LCH2;
        LCH1  = convert::RGB2LCH(col1);
        LCH2  = convert::RGB2LCH(col2);
        LCH   = detail::calcColor(LCH1, LCH2, ratio);
        color = convert::LCH2RGB(LCH);
    }else if(type == colorType::CMY){
        glm::vec3 CMY, CMY1, CMY2;
        CMY1  = convert::RGB2CMY(col1);
        CMY2  = convert::RGB2CMY(col2);
        CMY   = detail::calcColor(CMY1, CMY2, ratio);
        color = convert::CMY2RGB(CMY);
    }else if(type == colorType::CMYK){
        glm::vec4 CMYK, CMYK1, CMYK2;
        CMYK1  = convert::RGB2CMYK(col1);
        CMYK2  = convert::RGB2CMYK(col2);
        CMYK   = detail::calcColor(CMYK1, CMYK2, ratio);
        color  = convert::CMYK2RGB(CMYK);
    }else if(type == colorType::YXY){
        glm::vec3 YXY, YXY1, YXY2;
        YXY1  = convert::RGB2YXY(col1);
        YXY2  = convert::RGB2YXY(col2);
        YXY   = detail::calcColor(YXY1, YXY2, ratio);
        color = convert::YXY2RGB(YXY);
    }else if(type == colorType::XYZ){
        glm::vec3 XYZ, XYZ1, XYZ2;
        XYZ1  = convert::RGB2XYZ(col1);
        XYZ2  = convert::RGB2XYZ(col2);
        XYZ   = detail::calcColor(XYZ1, XYZ2, ratio);
        color = convert::XYZ2RGB(XYZ);
    }
    return color;
}

}//namespace morph

}//namespace colorMorph
}//namespace ofx



#endif /* colorSpace_h */
