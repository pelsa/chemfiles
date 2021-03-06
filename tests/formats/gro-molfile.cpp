#include "catch.hpp"
#include "helpers.hpp"
#include "chemfiles.hpp"
using namespace chemfiles;

TEST_CASE("Read files in Gromacs .gro format using Molfile", "[Molfile]"){
    Trajectory file("data/gro/ubiquitin.gro");
    Frame frame = file.read();

    CHECK(frame.natoms() == 1405);
    auto positions = frame.positions();
    CHECK(approx_eq(positions[0], vector3d(24.93, 24.95, 18.87), 1e-2));
    CHECK(approx_eq(positions[1], vector3d(25.66, 25.37, 18.33), 1e-2));
    CHECK(approx_eq(positions[678], vector3d(27.57, 32.25, 37.53), 1e-2));
}
