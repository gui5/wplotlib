#include <wx/wx.h>
namespace wpl {
struct pen_theme_t {
    wxColour color;
    int width;
    wxPenStyle style;

    pen_theme_t() :
        width(1), style(wxPenStyle::wxPENSTYLE_SOLID) {}

    pen_theme_t(wxColour color, int width, const wxPenStyle& style) :
        color(color), width(width), style(style) {}
    ~pen_theme_t() = default;
};

struct font_theme_t {
    int size;
    wxFontFamily family;
    wxFontStyle style;
    wxFontWeight weight;
    bool underlined;
    wxString font_face;
    wxFontEncoding encoding;

    font_theme_t() :
        size(10), family(wxFontFamily::wxFONTFAMILY_DEFAULT),
        style(wxFontStyle::wxFONTSTYLE_NORMAL),
        weight(wxFontWeight::wxFONTWEIGHT_NORMAL),
        underlined(false), font_face(wxEmptyString),
        encoding(wxFONTENCODING_DEFAULT) {}

    font_theme_t(int size, wxFontFamily family, wxFontStyle style,
        wxFontWeight weight, bool underlined, const wxString& font_face,
        wxFontEncoding encoding) :
        size(size), family(family), style(style), weight(weight),
        underlined(underlined), font_face(font_face), encoding(encoding) {}

    ~font_theme_t() = default;
};

struct brush_theme_t {
    wxColour color;
    wxBrushStyle style;

    brush_theme_t() :
        style(wxBrushStyle::wxBRUSHSTYLE_SOLID) {}

    brush_theme_t(wxColour color, wxBrushStyle style) :
        color(color), style(style) {}

    ~brush_theme_t() = default;
};


class style_t {
public:
    style_t() = default;
    ~style_t() = default;


    const wxPen& pen()noexcept {
        return _pen;
    }
    const wxFont& font()noexcept {
        return _font;
    }
    const wxBrush& brush()noexcept {
        return _brush;
    }

    void set_pen_theme(const pen_theme_t& theme)noexcept {
        _pen = wxPen(theme.color, theme.width, theme.style);
    }

    void set_font_theme(const font_theme_t& theme)noexcept {
        _font = wxFont(theme.size, theme.family, theme.style, theme.weight,
            theme.underlined, theme.font_face, theme.encoding);
    }

    void set_brush_theme(const brush_theme_t& theme)noexcept {
        _brush = wxBrush(theme.color, theme.style);
    }

private:
    wxPen _pen;
    wxFont _font;
    wxBrush _brush;
};
}