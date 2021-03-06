#include <chemfiles.h>
#include <stdlib.h>
#include <assert.h>

int main() {
    // [example]
    CHFL_CELL* cell = chfl_cell((chfl_vector_t){10, 10, 10});

    double volume = 0;
    chfl_cell_volume(cell, &volume);
    assert(volume == 1000);

    chfl_cell_free(cell);
    // [example]
    return 0;
}
