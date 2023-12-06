// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef GDSCRIPT_TEXT_DOCUMENT_H
#define GDSCRIPT_TEXT_DOCUMENT_H

#include "core/os/file_access.h"
#include "core/reference.h"
#include "lsp.hpp"

class GDScriptTextDocument : public Reference {
    GDCLASS(GDScriptTextDocument, Reference)

protected:
    static void _bind_methods();

    FileAccess* file_checker;

    void didOpen(const Variant& p_param);
    void didClose(const Variant& p_param);
    void didChange(const Variant& p_param);
    void didSave(const Variant& p_param);

    void sync_script_content(const String& p_path, const String& p_content);
    void show_native_symbol_in_editor(const String& p_symbol_id);

    Array native_member_completions;

private:
    Array find_symbols(
        const lsp::TextDocumentPositionParams& p_location,
        List<const lsp::DocumentSymbol*>& r_list
    );
    lsp::TextDocumentItem load_document_item(const Variant& p_param);
    void notify_client_show_symbol(const lsp::DocumentSymbol* symbol);

public:
    Variant nativeSymbol(const Dictionary& p_params);
    Array documentSymbol(const Dictionary& p_params);
    Array completion(const Dictionary& p_params);
    Dictionary resolve(const Dictionary& p_params);
    Dictionary rename(const Dictionary& p_params);
    Array foldingRange(const Dictionary& p_params);
    Array codeLens(const Dictionary& p_params);
    Array documentLink(const Dictionary& p_params);
    Array colorPresentation(const Dictionary& p_params);
    Variant hover(const Dictionary& p_params);
    Array definition(const Dictionary& p_params);
    Variant declaration(const Dictionary& p_params);
    Variant signatureHelp(const Dictionary& p_params);

    void initialize();

    GDScriptTextDocument();
    virtual ~GDScriptTextDocument();
};

#endif
