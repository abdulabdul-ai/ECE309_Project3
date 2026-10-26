// Mudassir Mohammed
// Project 3
// Date 10/26/2026
// src/tools/workspace.cpp — YOU IMPLEMENT (spec §4.4).
//
// Workspace::resolve: a purely lexical check, steps 1–6 of the spec.
// Do not touch the filesystem here (no exists(), canonical(), ...).
// The constructor, root() and check_symlinks() are provided in
// workspace_provided.cpp.

#include "tools/workspace.h"

namespace fs = std::filesystem;

// This function returns true and sets *out to be the path taken if the path is valid 
// and false it the path is invalid and sets *reason to a valid explanation.
bool Workspace::resolve(std::string_view user_path, fs::path* out,
                        std::string* reason) const {
    // If its empty, return false and set reason as "empty path"
    if (user_path.empty()){
        *reason = "empty path";
        return false;
    }

    // If the path contains a null character, return false and 
    // set reason as "null character".
    for (std::size_t i = 0; i < user_path.size(); i++){
        if (user_path[i] == '\0'){
            *reason = "null character";
            return false;
        }
    }

    
    const fs::path p = user_path;
    // If the path is absolute or has root path return false and set 
    // reason as "non-workspace-relative path".
    if (p.is_absolute() || p.has_root_path()){
        *reason = "non-workspace-relative path";
        return false;
    }

    // Remove duplicate slashes, and fold "x/.." pairs
    const fs::path n = p.lexically_normal();

    // If the start of the path is "..", return false and 
    //set reason as "climbs above root".
    if (*n.begin() == ".."){
        *reason = "climbs above root";
        return false;
    }

    //the path is ".", set the *out as the root of the directory
    if (n == "."){
        *out = root();

    } else {
        *out = (root()/n);
    }
    // return true
    return true;
}
