// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "editor_fonts.h"

#include "builtin_fonts.gen.h"
#include "core/os/dir_access.h"
#include "editor_scale.h"
#include "editor_settings.h"
#include "scene/resources/default_theme/default_theme.h"
#include "scene/resources/dynamic_font.h"

static void configure_dynamic_font_data(
    Ref<DynamicFontData>& font_data,
    const bool antialiased,
    const DynamicFontData::Hinting hinting
) {
    font_data->set_antialiased(antialiased);
    font_data->set_hinting(hinting);
    font_data->set_force_autohinter(true);
}

static Ref<DynamicFontData> create_dynamic_font_data_from_file(
    const String& font_path
) {
    Ref<DynamicFontData> font_data;
    if (!font_path.empty()) {
        font_data.instance();
        font_data->set_font_path(font_path);
    }
    return font_data;
}

static Ref<DynamicFontData> create_dynamic_font_data_from_source(
    const unsigned char* font_bytes,
    const int font_bytes_length
) {
    Ref<DynamicFontData> font_data;
    font_data.instance();
    font_data->set_font_ptr(font_bytes, font_bytes_length);
    return font_data;
}

static void add_fallbacks(
    Ref<DynamicFont> font,
    const Vector<Ref<DynamicFontData>>& fallback_font_data
) {
    for (int index = 0; index < fallback_font_data.size(); index++) {
        const Ref<DynamicFontData>& font_data = fallback_font_data[index];
        font->add_fallback(font_data);
    }
}

static Ref<DynamicFont> create_font(
    const Ref<DynamicFontData>& font_data,
    const int font_size
) {
    Ref<DynamicFont> font;
    font.instance();
    font->set_size(font_size);
    font->set_use_filter(true);
    font->set_use_mipmaps(true);
    font->set_font_data(font_data);
    font->set_spacing(DynamicFont::SPACING_TOP, -static_cast<int>(EDSCALE));
    font->set_spacing(DynamicFont::SPACING_BOTTOM, -static_cast<int>(EDSCALE));
    return font;
}

static Ref<DynamicFont> create_font(
    const Ref<DynamicFontData>& custom_font_data,
    const Ref<DynamicFontData>& builtin_font_data,
    const int font_size,
    const Vector<Ref<DynamicFontData>>& fallback_font_data
) {
    Ref<DynamicFont> font;
    if (custom_font_data.is_valid()) {
        font = create_font(custom_font_data, font_size);
        font->add_fallback(builtin_font_data);
    } else {
        font = create_font(builtin_font_data, font_size);
    }
    add_fallbacks(font, fallback_font_data);
    return font;
}

static DynamicFontData::Hinting get_hinting() {
    const int hinting_setting =
        EditorSettings::get_singleton()->get("interface/editor/font_hinting");
    switch (hinting_setting) {
        case 0:
            // The "Auto" setting uses the setting that best matches the OS'
            // font rendering:
            // - macOS doesn't use font hinting.
            // - Windows uses ClearType, which is in between "Light" and
            // "Normal" hinting.
            // - Linux has configurable font hinting, but most distributions
            // including Ubuntu default to "Light".
#ifdef MACOS_ENABLED
            return DynamicFontData::HINTING_NONE;
#else  // !MACOS_ENABLED
            return DynamicFontData::HINTING_LIGHT;
#endif // !MACOS_ENABLED
        case 1:
            return DynamicFontData::HINTING_NONE;
        case 2:
            return DynamicFontData::HINTING_LIGHT;
        default:
            return DynamicFontData::HINTING_NORMAL;
    }
}

void register_editor_fonts(Ref<Theme> theme) {
    // Get editor font settings.
    const int main_font_size = static_cast<int>(
        static_cast<float>(EDITOR_GET("interface/editor/main_font_size"))
        * EDSCALE
    );
    const int code_font_size = static_cast<int>(
        static_cast<float>(EDITOR_GET("interface/editor/code_font_size"))
        * EDSCALE
    );
    const int help_font_size = static_cast<int>(
        static_cast<float>(EDITOR_GET("text_editor/help/help_font_size"))
        * EDSCALE
    );
    const int help_source_font_size = static_cast<int>(
        static_cast<float>(EDITOR_GET("text_editor/help/help_source_font_size"))
        * EDSCALE
    );
    const int help_title_font_size = static_cast<int>(
        static_cast<float>(EDITOR_GET("text_editor/help/help_title_font_size"))
        * EDSCALE
    );
    const int output_font_size = static_cast<int>(
        static_cast<float>(EDITOR_GET("run/output/font_size")) * EDSCALE
    );
    const bool antialiased =
        EditorSettings::get_singleton()->get("interface/editor/font_antialiased"
        );
    const DynamicFontData::Hinting hinting = get_hinting();

    // Fixed and relative size values.
    const int title_font_size  = main_font_size + static_cast<int>(2 * EDSCALE);
    const int rulers_font_size = static_cast<int>(8 * EDSCALE);
    const int rotation_control_font_size = static_cast<int>(14 * EDSCALE);
    const int expression_font_size       = static_cast<int>(
        (static_cast<float>(EDITOR_GET("interface/editor/code_font_size")) - 1)
        * EDSCALE
    );

    // Get custom editor font data.
    DirAccess* dir = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
    String main_font_path =
        EditorSettings::get_singleton()->get("interface/editor/main_font");
    if (!dir->file_exists(main_font_path)) {
        // Clear main font setting.
        EditorSettings::get_singleton()->set_manually(
            "interface/editor/main_font",
            ""
        );
        main_font_path = "";
    }
    Ref<DynamicFontData> custom_main_font_data =
        create_dynamic_font_data_from_file(main_font_path);

    String main_font_bold_path =
        EditorSettings::get_singleton()->get("interface/editor/main_font_bold");
    if (!dir->file_exists(main_font_bold_path)) {
        // Clear main font bold setting.
        EditorSettings::get_singleton()->set_manually(
            "interface/editor/main_font_bold",
            ""
        );
        main_font_bold_path = "";
    }
    Ref<DynamicFontData> custom_bold_font_data =
        create_dynamic_font_data_from_file(main_font_bold_path);

    String code_font_path =
        EditorSettings::get_singleton()->get("interface/editor/code_font");
    if (!dir->file_exists(code_font_path)) {
        // Clear code font setting.
        EditorSettings::get_singleton()->set_manually(
            "interface/editor/code_font",
            ""
        );
        code_font_path = "";
    }
    Ref<DynamicFontData> custom_code_font_data =
        create_dynamic_font_data_from_file(code_font_path);
    memdelete(dir);

    // Configure user custom font data.
    if (custom_main_font_data.is_valid()) {
        configure_dynamic_font_data(
            custom_main_font_data,
            antialiased,
            hinting
        );
    }
    if (custom_bold_font_data.is_valid()) {
        configure_dynamic_font_data(
            custom_bold_font_data,
            antialiased,
            hinting
        );
    }
    if (custom_code_font_data.is_valid()) {
        configure_dynamic_font_data(
            custom_code_font_data,
            antialiased,
            hinting
        );
        // Don't force auto hinting for source code font.
        custom_code_font_data->set_force_autohinter(false);
    }

    // Get built-in font data.
    Ref<DynamicFontData> main_font_data = create_dynamic_font_data_from_source(
        _font_NotoSansUI_Regular,
        _font_NotoSansUI_Regular_size
    );
    configure_dynamic_font_data(main_font_data, antialiased, hinting);
    Ref<DynamicFontData> bold_font_data = create_dynamic_font_data_from_source(
        _font_NotoSansUI_Bold,
        _font_NotoSansUI_Bold_size
    );
    configure_dynamic_font_data(bold_font_data, antialiased, hinting);
    Ref<DynamicFontData> droid_fallback_font_data =
        create_dynamic_font_data_from_source(
            _font_DroidSansFallback,
            _font_DroidSansFallback_size
        );
    configure_dynamic_font_data(droid_fallback_font_data, antialiased, hinting);
    Ref<DynamicFontData> japanese_font_data =
        create_dynamic_font_data_from_source(
            _font_DroidSansJapanese,
            _font_DroidSansJapanese_size
        );
    configure_dynamic_font_data(japanese_font_data, antialiased, hinting);
    Ref<DynamicFontData> arabic_font_data =
        create_dynamic_font_data_from_source(
            _font_NotoNaskhArabicUI_Regular,
            _font_NotoNaskhArabicUI_Regular_size
        );
    configure_dynamic_font_data(arabic_font_data, antialiased, hinting);
    Ref<DynamicFontData> hebrew_font_data =
        create_dynamic_font_data_from_source(
            _font_NotoSansHebrew_Regular,
            _font_NotoSansHebrew_Regular_size
        );
    configure_dynamic_font_data(hebrew_font_data, antialiased, hinting);
    Ref<DynamicFontData> thai_font_data = create_dynamic_font_data_from_source(
        _font_NotoSansThaiUI_Regular,
        _font_NotoSansThaiUI_Regular_size
    );
    configure_dynamic_font_data(thai_font_data, antialiased, hinting);
    Ref<DynamicFontData> hindi_font_data = create_dynamic_font_data_from_source(
        _font_NotoSansDevanagariUI_Regular,
        _font_NotoSansDevanagariUI_Regular_size
    );
    configure_dynamic_font_data(hindi_font_data, antialiased, hinting);
    Ref<DynamicFontData> code_font_data = create_dynamic_font_data_from_source(
        _font_Hack_Regular,
        _font_Hack_Regular_size
    );
    configure_dynamic_font_data(code_font_data, antialiased, hinting);

    // Create fallback font data.
    Vector<Ref<DynamicFontData>> fallback_font_data;
    fallback_font_data.push_back(arabic_font_data);
    fallback_font_data.push_back(hebrew_font_data);
    fallback_font_data.push_back(thai_font_data);
    fallback_font_data.push_back(hindi_font_data);
    fallback_font_data.push_back(japanese_font_data);
    fallback_font_data.push_back(droid_fallback_font_data);

    // Create fonts.
    const Ref<DynamicFont> main_font = create_font(
        custom_main_font_data,
        main_font_data,
        main_font_size,
        fallback_font_data
    );
    const Ref<DynamicFont> bold_font = create_font(
        custom_bold_font_data,
        bold_font_data,
        main_font_size,
        fallback_font_data
    );
    const Ref<DynamicFont> title_font = create_font(
        custom_bold_font_data,
        bold_font_data,
        title_font_size,
        fallback_font_data
    );
    const Ref<DynamicFont> documentation_font = create_font(
        custom_main_font_data,
        main_font_data,
        help_font_size,
        fallback_font_data
    );
    const Ref<DynamicFont> documentation_bold_font = create_font(
        custom_bold_font_data,
        bold_font_data,
        help_font_size,
        fallback_font_data
    );
    const Ref<DynamicFont> documentation_title_font = create_font(
        custom_bold_font_data,
        bold_font_data,
        help_title_font_size,
        fallback_font_data
    );
    const Ref<DynamicFont> documentation_code_font = create_font(
        custom_code_font_data,
        code_font_data,
        help_source_font_size,
        fallback_font_data
    );
    const Ref<DynamicFont> rulers_font = create_font(
        custom_main_font_data,
        main_font_data,
        rulers_font_size,
        fallback_font_data
    );
    const Ref<DynamicFont> rotation_control_font = create_font(
        custom_main_font_data,
        main_font_data,
        rotation_control_font_size,
        fallback_font_data
    );
    const Ref<DynamicFont> source_font = create_font(
        custom_code_font_data,
        code_font_data,
        code_font_size,
        fallback_font_data
    );
    const Ref<DynamicFont> expression_font = create_font(
        custom_code_font_data,
        code_font_data,
        expression_font_size,
        fallback_font_data
    );
    const Ref<DynamicFont> output_source_font = create_font(
        custom_code_font_data,
        code_font_data,
        output_font_size,
        fallback_font_data
    );
    const Ref<DynamicFont> status_source_font = create_font(
        custom_code_font_data,
        code_font_data,
        main_font_size,
        fallback_font_data
    );

    // Set theme fonts.
    theme->set_default_theme_font(main_font);
    theme->set_font("main", "EditorFonts", main_font);
    theme->set_font("bold", "EditorFonts", bold_font);
    theme->set_font("title", "EditorFonts", title_font);
    theme->set_font("doc", "EditorFonts", documentation_font);
    theme->set_font("doc_bold", "EditorFonts", documentation_bold_font);
    theme->set_font("doc_title", "EditorFonts", documentation_title_font);
    theme->set_font("doc_source", "EditorFonts", documentation_code_font);
    theme->set_font("rulers", "EditorFonts", rulers_font);
    theme->set_font("rotation_control", "EditorFonts", rotation_control_font);
    theme->set_font("source", "EditorFonts", source_font);
    theme->set_font("expression", "EditorFonts", expression_font);
    theme->set_font("output_source", "EditorFonts", output_source_font);
    theme->set_font("status_source", "EditorFonts", status_source_font);
}
