#pragma once


// Struct to describe a four-component color in the RGBA format.
struct Color
{
    float r;
    float g;
    float b;
    float a;
};


// Struct to describe a font (struct defined in the games).
struct Font_s
{
    int fontName;
    int pixelHeight;
    int glyphCount;
    int material;
    int glowMaterial;
    int glyphs;
};


typedef void (*R_ADDCMDDRAWTEXT)(const char *text, int maxChars, Font_s *font, float x, float y, float xScale, float yScale, float rotation, const float *color, int style);
typedef void (*R_ADDCMDDRAWSTRETCHPIC)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, const float *color, HANDLE material);
typedef Font_s *(*R_REGISTERFONT)(const char *font, int imageTrack);
typedef HANDLE (*MATERIAL_REGISTERHANDLE)(const char *name, int imageTrack);


#define CHAR_UP "\x14"
#define CHAR_DOWN "\x15"
#define CHAR_X "\x3"
#define CHAR_LB "\x5"
#define CHAR_RB "\x6"
#define CHAR_RS "\x11"


// Interface that the other elements inherit from.
class HudElem
{
public:
    static const Color s_ColorWhite;
    static const Color s_ColorBlack;
    static const float s_MenuX;
    static const float s_MenuY;
    static const float s_MenuWidth;
    static const float s_MenuHeight;
    static const float s_Padding;
    static const float s_TitleHeight;
    static const float s_LineHeight;

    static R_REGISTERFONT R_RegisterFont;
    static MATERIAL_REGISTERHANDLE Material_RegisterHandle;

    // Default constructor.
    HudElem() {}

    // Constructor.
    HudElem(float fX, float fY, const Color &color);

    // Virtual destructor.
    virtual ~HudElem() {}

    // Draw the element (one implementation per element).
    virtual void Draw() = 0;

    float GetX() const { return m_fX; }
    float GetY() const { return m_fY; }
    const Color &GetColor() const { return m_Color; }

    void SetX(float fX) { m_fX = fX; }
    void SetY(float fY) { m_fY = fY; }
    void SetColor(const Color &color) { m_Color = color; }
    void SetColor(float fR, float fG, float fB, float fA);
    void SetAlpha(float fAlpha) { m_Color.a = fAlpha; }

    static void SetFont(Font_s *pFont) { s_pFont = pFont; }
    static void SetMaterialHandle(HANDLE hMaterial) { s_hMaterial = hMaterial; }

    static void SetDrawTextFnPtr(R_ADDCMDDRAWTEXT pFn) { R_AddCmdDrawText = pFn; }
    static void SetDrawRectangleFnPtr(R_ADDCMDDRAWSTRETCHPIC pFn) { R_AddCmdDrawStretchPic = pFn; }
protected:
    float m_fX;
    float m_fY;
    Color m_Color;

    static Font_s *s_pFont;
    static HANDLE s_hMaterial;

    static R_ADDCMDDRAWTEXT R_AddCmdDrawText;
    static R_ADDCMDDRAWSTRETCHPIC R_AddCmdDrawStretchPic;
};
