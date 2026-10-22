// src/tools/workspace.cpp — YOU IMPLEMENT (spec §4.4).
//
// Workspace::resolve: a purely lexical check, steps 1–6 of the spec.
// Do not touch the filesystem here (no exists(), canonical(), ...).
// The constructor, root() and check_symlinks() are provided in
// workspace_provided.cpp.

#include "tools/workspace.h"

namespace fs = std::filesystem;

bool Workspace::resolve(std::string_view user_path, fs::path* out,
                        std::string* reason) const {
    if (user_path.empty()){
        *reason = "empty path";
        return false;
    }
    //auto path = std::string_view.begin();
    for (std::size_t i = 0; i < user_path.size(); i++){
        if (user_path[i] == '\0'){
            *reason = "null character";
            return false;
        }
    }

    const fs::path p = user_path;
    if (p.is_absolute() || p.has_root_path()){
        *reason = "non-workspace-relative path";
        return false;
    }

    const fs::path n = p.lexically_normal();

    *reason = "Workspace::resolve not implemented yet";   // TODO
    return false;
}
