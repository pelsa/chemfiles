#include <chemfiles.h>
#include <stdlib.h>

int main() {
    // [example]
    CHFL_FRAME* frame = chfl_frame();
    CHFL_CELL* cell = chfl_cell((chfl_vector_t){10, 10, 12});

    chfl_frame_set_cell(frame, cell);

    chfl_cell_free(cell);
    chfl_frame_free(frame);
    // [example]
    return 0;
}
