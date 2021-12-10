#pragma once


// Struct to describe a four-component color in the RGBA format.
struct Color
{
    FLOAT r;
    FLOAT g;
    FLOAT b;
    FLOAT a;
};


// Struct to describe a font (struct defined in the games).
struct Font_s
{
    INT fontName;
    INT pixelHeight;
    INT glyphCount;
    INT material;
    INT glowMaterial;
    INT glyphs;
};


typedef VOID (*R_ADDCMDDRAWTEXT)(LPCSTR text, INT maxChars, Font_s *font, FLOAT x, FLOAT y, FLOAT xScale, FLOAT yScale, FLOAT rotation, CONST PFLOAT color, INT style);
typedef VOID (*R_ADDCMDDRAWSTRETCHPIC)(FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT s0, FLOAT t0, FLOAT s1, FLOAT t1, CONST PFLOAT color, HANDLE material);
typedef Font_s *(*R_REGISTERFONT)(LPCSTR font, INT imageTrack);
typedef HANDLE (*MATERIAL_REGISTERHANDLE)(LPCSTR name, INT imageTrack);


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
    static CONST Color s_ColorWhite;
    static CONST Color s_ColorBlack;
    static CONST FLOAT s_MenuX;
    static CONST FLOAT s_MenuY;
    static CONST FLOAT s_MenuWidth;
    static CONST FLOAT s_MenuHeight;
    static CONST FLOAT s_Padding;
    static CONST FLOAT s_TitleHeight;
    static CONST FLOAT s_LineHeight;

    static R_REGISTERFONT R_RegisterFont;
    static MATERIAL_REGISTERHANDLE Material_RegisterHandle;

    // Default constructor.
    HudElem() {}

    // Constructor.
    HudElem(FLOAT fX, FLOAT fY, CONST Color &color);

    // Virtual destructor.
    virtual ~HudElem() {}

    // Draw the element (one implementation per element).
    virtual VOID Draw() = 0;

    FLOAT GetX() CONST { return m_fX; }
    FLOAT GetY() CONST { return m_fY; }
    CONST Color &GetColor() CONST { return m_Color; }

    VOID SetX(FLOAT fX) { m_fX = fX; }
    VOID SetY(FLOAT fY) { m_fY = fY; }
    VOID SetColor(CONST Color &color) { m_Color = color; }
    VOID SetColor(FLOAT fR, FLOAT fG, FLOAT fB, FLOAT fA);
    VOID SetAlpha(FLOAT fAlpha) { m_Color.a = fAlpha; }

    static VOID SetFont(Font_s *pFont) { s_pFont = pFont; }
    static VOID SetMaterialHandle(HANDLE hMaterial) { s_hMaterial = hMaterial; }

    static VOID SetDrawTextFnPtr(R_ADDCMDDRAWTEXT pFn) { R_AddCmdDrawText = pFn; }
    static VOID SetDrawRectangleFnPtr(R_ADDCMDDRAWSTRETCHPIC pFn) { R_AddCmdDrawStretchPic = pFn; }
protected:
    FLOAT m_fX;
    FLOAT m_fY;
    Color m_Color;

    static Font_s *s_pFont;
    static HANDLE s_hMaterial;

    static R_ADDCMDDRAWTEXT R_AddCmdDrawText;
    static R_ADDCMDDRAWSTRETCHPIC R_AddCmdDrawStretchPic;
};
