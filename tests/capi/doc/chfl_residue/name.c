#include <chemfiles.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int main() {
    // [example]
    CHFL_RESIDUE* residue = chfl_residue("water", 3);

    char name[32] = {0};
    chfl_residue_name(residue, name, sizeof(name));
    assert(strcmp(name, "water") == 0);

    chfl_residue_free(residue);
    // [example]
    return 0;
}
