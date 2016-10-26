/* Chemfiles, an efficient IO library for chemistry file formats
* Copyright (C) 2015 Guillaume Fraux
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/
*/
// clang-format off

#include "chemfiles.h"
#include "chemfiles/Selections.hpp"
#include "chemfiles/capi.hpp"
using namespace chemfiles;

static_assert(
    CHFL_MAX_SELECTION_SIZE == Match::MAX_MATCH_SIZE,
    "CHFL_MAX_SELECTION_SIZE should match Match::MAX_MATCH_SIZE"
);

struct CAPISelection {
    CAPISelection(Selection&& select): selection(std::move(select)), matches() {}
    Selection selection;
    std::vector<Match> matches;
};

CHFL_SELECTION* chfl_selection(const char* selection) {
    CHFL_SELECTION* c_selection = nullptr;
    CHFL_ERROR_GOTO(
        c_selection = new CAPISelection(Selection(std::string(selection)));
    )
    return c_selection;
error:
    delete c_selection;
    return nullptr;
}

chfl_status chfl_selection_size(const CHFL_SELECTION* const selection, uint64_t* size) {
    assert(selection != nullptr);
    CHFL_ERROR_CATCH(
        *size = selection->selection.size();
    )
}

chfl_status chfl_selection_evalutate(CHFL_SELECTION* const selection, const CHFL_FRAME* const frame, uint64_t* n_matches) {
    assert(selection != nullptr);
    CHFL_ERROR_CATCH(
        selection->matches = selection->selection.evaluate(*frame);
        *n_matches = selection->matches.size();
    )
}

chfl_status chfl_selection_matches(const CHFL_SELECTION* const selection, chfl_match_t* const matches, uint64_t n_matches) {
    assert(selection != nullptr);
    assert(n_matches == selection->matches.size());
    CHFL_ERROR_CATCH(
        auto size = selection->selection.size();
        for (uint64_t i=0; i<n_matches; i++) {
            matches[i].size = size;
            for (uint64_t j=0; j<size; j++) {
                matches[i].atoms[j] = selection->matches[i][j];
            }

            for (uint64_t j=size; j<CHFL_MAX_SELECTION_SIZE; j++) {
                matches[i].atoms[j] = static_cast<uint64_t>(-1);
            }
        }
    )
}

chfl_status chfl_selection_free(CHFL_SELECTION* selection) {
    delete selection;
    selection = nullptr;
    return CHFL_SUCCESS;
}