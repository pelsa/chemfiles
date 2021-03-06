#include <chemfiles.h>
#include <stdlib.h>
#include <assert.h>

int main() {
    // [example]
    CHFL_TOPOLOGY* topology = chfl_topology();

    chfl_topology_resize(topology, 67);

    uint64_t atoms = 0;
    chfl_topology_atoms_count(topology, &atoms);
    assert(atoms == 67);

    chfl_topology_free(topology);
    // [example]
    return 0;
}
