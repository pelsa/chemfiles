#include <chemfiles.h>
#include <stdlib.h>

int main() {
    // [example]
    CHFL_RESIDUE* residue = chfl_residue("water", 3);

    if (residue == NULL) {
        /* handle error */
    }

    chfl_residue_free(residue);
    // [example]
    return 0;
}
