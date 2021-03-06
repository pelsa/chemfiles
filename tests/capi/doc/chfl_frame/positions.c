#include <chemfiles.h>
#include <stdlib.h>

int main() {
    // [example]
    CHFL_FRAME* frame = chfl_frame();

    chfl_vector_t* positions = NULL;
    uint64_t natoms = 0;
    chfl_frame_positions(frame, &positions, &natoms);

    for (uint64_t i=0; i<natoms; i++) {
        // use positions[i] here
    }

    chfl_frame_free(frame);
    // [example]
    return 0;
}
