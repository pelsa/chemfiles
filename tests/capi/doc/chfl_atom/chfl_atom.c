#include <chemfiles.h>
#include <stdlib.h>

int main() {
    // [example]
    CHFL_ATOM* atom = chfl_atom("Na");

    if (atom == NULL) {
        /* handle error */
    }

    chfl_atom_free(atom);
    // [example]
    return 0;
}
