#include <streambuf>
#include <fstream>
#include <cstdio>

#include "catch.hpp"
#include "chemfiles.hpp"
using namespace chemfiles;

#include <boost/filesystem.hpp>
namespace fs=boost::filesystem;


TEST_CASE("Read files in XYZ format", "[XYZ]"){
    SECTION("Check nsteps"){
        Trajectory file1("data/xyz/trajectory.xyz");
        CHECK(file1.nsteps() == 2);

        Trajectory file2("data/xyz/helium.xyz");
        CHECK(file2.nsteps() == 397);

        Trajectory file3("data/xyz/topology.xyz");
        CHECK(file3.nsteps() == 1);
    }


    SECTION("Method style reading"){
        Trajectory file("data/xyz/helium.xyz");
        auto frame = file.read();
        CHECK(frame.natoms() == 125);
        // Check positions
        auto positions = frame.positions();
        CHECK(positions[0] == vector3d(0.49053, 8.41351, 0.0777257));
        CHECK(positions[124] == vector3d(8.57951, 8.65712, 8.06678));
        // Check topology
        auto topology = frame.topology();
        CHECK(topology.natoms() == 125);
        CHECK(topology[0] == Atom("He"));
    }

    SECTION("Read a specific step"){
        Trajectory file("data/xyz/helium.xyz");
        // Read frame at a specific positions
        auto frame = file.read_step(42);
        auto positions = frame.positions();
        CHECK(positions[0] == vector3d(-0.145821, 8.540648, 1.090281));
        CHECK(positions[124] == vector3d(8.446093, 8.168162, 9.350953));
        auto topology = frame.topology();
        CHECK(topology.natoms() == 125);
        CHECK(topology[0] == Atom("He"));

        frame = file.read_step(0);
        positions = frame.positions();
        CHECK(positions[0] == vector3d(0.49053, 8.41351, 0.0777257));
        CHECK(positions[124] == vector3d(8.57951, 8.65712, 8.06678));
    }

    SECTION("Read the whole file"){
        Trajectory file("data/xyz/helium.xyz");
        CHECK(file.nsteps() == 397);

        Frame frame;
        while (!file.done()){
            frame = file.read();
        }
        auto positions = frame.positions();
        CHECK(positions[0] == vector3d(-1.186037, 11.439334, 0.529939));
        CHECK(positions[124] == vector3d(5.208778, 12.707273, 10.940157));
    }

    SECTION("Read various files formatting"){
        Trajectory file("data/xyz/spaces.xyz");

        auto frame = file.read();
        auto positions = frame.positions();
        CHECK(positions[10] == vector3d(0.8336, 0.3006, 0.4968));
    }
}

// To use in loops in order to iterate over files in a specific directory.
struct directory_files_iterator {
    typedef fs::recursive_directory_iterator iterator;
    directory_files_iterator(fs::path p) : p_(p) {}

    iterator begin() { return fs::recursive_directory_iterator(p_); }
    iterator end() { return fs::recursive_directory_iterator(); }

    fs::path p_;
};

TEST_CASE("Errors in XYZ format", "[XYZ]"){
    for (auto entry : directory_files_iterator("data/xyz/bad/")){
        CHECK_THROWS_AS(
            // We can throw either when creating the trajectory, or when reading
            // the frame, depending on the type of error
            auto file = Trajectory(entry.path().string());
            file.read(),
        FormatError);
    }
}

TEST_CASE("Write files in XYZ format", "[XYZ]"){
    const auto expected_content =
    "4\n"
    "Written by the chemfiles library\n"
    "A 1 2 3\n"
    "B 1 2 3\n"
    "C 1 2 3\n"
    "D 1 2 3\n"
    "6\n"
    "Written by the chemfiles library\n"
    "A 4 5 6\n"
    "B 4 5 6\n"
    "C 4 5 6\n"
    "D 4 5 6\n"
    "E 4 5 6\n"
    "F 4 5 6\n";

    Frame frame(4);
    auto positions = frame.positions();
    for(size_t i=0; i<4; i++)
        positions[i] = vector3d(1, 2, 3);

    Topology topology;
    topology.append(Atom("A"));
    topology.append(Atom("B"));
    topology.append(Atom("C"));
    topology.append(Atom("D"));
    frame.set_topology(topology);

    {
        auto file = Trajectory("test-tmp.xyz", 'w');
        file.write(frame);
    }


    frame.resize(6);
    positions = frame.positions();
    for(size_t i=0; i<6; i++)
        positions[i] = vector3d(4, 5, 6);

    topology.append(Atom("E"));
    topology.append(Atom("F"));
    frame.set_topology(topology);

    {
        auto file = Trajectory("test-tmp.xyz", 'a');
        file.write(frame);
    }

    std::ifstream checking("test-tmp.xyz");
    std::string content((std::istreambuf_iterator<char>(checking)),
                         std::istreambuf_iterator<char>());
    checking.close();

    CHECK(content == expected_content);

    remove("test-tmp.xyz");
}
