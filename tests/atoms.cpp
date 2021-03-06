#include <catch.hpp>

#include "chemfiles.hpp"
#include "chemfiles/Atom.hpp"
using namespace chemfiles;


TEST_CASE("Use the Atom type", "[Atoms]"){
    Atom a("H");
    Atom b;
    Atom c("C1", "C");
    Atom d("name only", "");

    SECTION("Constructors") {
        CHECK(a.name() == "H");
        CHECK(a.type() == "H");
        CHECK(a.mass() == 1.008);
        CHECK(a.charge() == 0);

        CHECK(b.name() == "");
        CHECK(b.type() == "");
        CHECK(b.mass() == 0);
        CHECK(b.charge() == 0);

        CHECK(c.name() == "C1");
        CHECK(c.type() == "C");
        CHECK(c.mass() == 12.011);
        CHECK(c.charge() == 0);

        CHECK(d.name() == "name only");
        CHECK(d.type() == "");
        CHECK(d.mass() == 0);
        CHECK(d.charge() == 0);
    }

    SECTION("Set and get properties") {
        Atom atom;

        CHECK(atom.mass() == 0);
        atom.set_mass(14.789);
        CHECK(atom.mass() == 14.789);

        CHECK(atom.charge() == 0);
        atom.set_charge(-2);
        CHECK(atom.charge() == -2);

        CHECK(atom.name() == "");
        atom.set_name("HE22");
        CHECK(atom.name() == "HE22");

        CHECK(atom.type() == "");
        atom.set_type("foo");
        CHECK(atom.type() == "foo");
    }

    SECTION("Elements properties") {
        CHECK(a.atomic_number() == 1);
        CHECK(a.full_name() == "Hydrogen");
        CHECK(a.covalent_radius() == 0.37);
        CHECK(a.vdw_radius() == 1.2);

        CHECK(b.atomic_number() == -1);
        CHECK(b.full_name() == "");
        CHECK(b.covalent_radius() == -1.0);
        CHECK(b.vdw_radius() == -1.0);

        CHECK(d.atomic_number() == -1);
        CHECK(d.full_name() == "");
        CHECK(d.covalent_radius() == -1.0);
        CHECK(d.vdw_radius() == -1.0);
    }
}
