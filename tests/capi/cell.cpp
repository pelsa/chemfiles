#include "catch.hpp"
#include "helpers.hpp"
#include "chemfiles.h"

static bool approx_eq(double A[3][3], double B[3][3]) {
    double eps = 1e-10;
    return
        (fabs(A[0][0] - B[0][0]) < eps) && (fabs(A[0][1] - B[0][1]) < eps) && (fabs(A[0][2] - B[0][2]) < eps) &&
        (fabs(A[1][0] - B[1][0]) < eps) && (fabs(A[1][1] - B[1][1]) < eps) && (fabs(A[1][2] - B[1][2]) < eps) &&
        (fabs(A[2][0] - B[2][0]) < eps) && (fabs(A[2][1] - B[2][1]) < eps) && (fabs(A[2][2] - B[2][2]) < eps);
}

TEST_CASE("Atom", "[CAPI]") {
    SECTION("Constructors") {
        chfl_vector_t lengths = {2, 3, 4};
        CHFL_CELL* cell = chfl_cell(lengths);
        REQUIRE(cell != NULL);

        chfl_vector_t data = {0};
        CHECK_STATUS(chfl_cell_lengths(cell, data));
        CHECK(data[0] == 2);
        CHECK(data[1] == 3);
        CHECK(data[2] == 4);

        CHECK_STATUS(chfl_cell_angles(cell, data));
        CHECK(data[0] == 90);
        CHECK(data[1] == 90);
        CHECK(data[2] == 90);

        CHECK_STATUS(chfl_cell_free(cell));

        lengths[0] = 20; lengths[1] = 21; lengths[2] = 22;
        chfl_vector_t angles = {90, 100, 120};
        cell = chfl_cell_triclinic(lengths, angles);
        REQUIRE(cell != NULL);

        CHECK_STATUS(chfl_cell_lengths(cell, data));
        CHECK(data[0] == 20);
        CHECK(data[1] == 21);
        CHECK(data[2] == 22);

        CHECK_STATUS(chfl_cell_angles(cell, data));
        CHECK(data[0] == 90);
        CHECK(data[1] == 100);
        CHECK(data[2] == 120);

        CHECK_STATUS(chfl_cell_free(cell));

        // Check that a call to chfl_cell_triclinic always gives a triclinic
        // cell, even with all angles equal to 90°
        angles[0] = 90; angles[1] = 90; angles[2] = 90;
        cell = chfl_cell_triclinic(lengths, angles);
        REQUIRE(cell != NULL);

        chfl_cell_shape_t shape;
        CHECK_STATUS(chfl_cell_shape(cell, &shape));
        CHECK(shape == CHFL_CELL_TRICLINIC);

        CHECK_STATUS(chfl_cell_free(cell));
    }

    SECTION("Length") {
        chfl_vector_t lengths = {2, 3, 4};
        CHFL_CELL* cell = chfl_cell(lengths);
        REQUIRE(cell != NULL);

        chfl_vector_t data = {0};
        CHECK_STATUS(chfl_cell_lengths(cell, data));
        CHECK(data[0] == 2);
        CHECK(data[1] == 3);
        CHECK(data[2] == 4);

        lengths[0] = 10; lengths[1] = 20; lengths[2] = 30;
        CHECK_STATUS(chfl_cell_set_lengths(cell, lengths));
        CHECK_STATUS(chfl_cell_lengths(cell, data));
        CHECK(data[0] == 10);
        CHECK(data[1] == 20);
        CHECK(data[2] == 30);

        CHECK_STATUS(chfl_cell_free(cell));
    }

    SECTION("Angles") {
        chfl_vector_t lengths = {2, 3, 4};
        CHFL_CELL* cell = chfl_cell(lengths);
        REQUIRE(cell != NULL);

        chfl_vector_t data = {0};
        CHECK_STATUS(chfl_cell_angles(cell, data));
        CHECK(data[0] == 90);
        CHECK(data[1] == 90);
        CHECK(data[2] == 90);

        chfl_vector_t angles = {80, 89, 100};
        // Setting an orthorhombic cell angles is an error
        CHECK(chfl_cell_set_angles(cell, angles) != CHFL_SUCCESS);

        CHECK_STATUS(chfl_cell_set_shape(cell, CHFL_CELL_TRICLINIC));

        CHECK_STATUS(chfl_cell_set_angles(cell, angles));
        CHECK_STATUS(chfl_cell_angles(cell, data));
        CHECK(data[0] == 80);
        CHECK(data[1] == 89);
        CHECK(data[2] == 100);

        CHECK_STATUS(chfl_cell_free(cell));
    }

    SECTION("Volume") {
        chfl_vector_t lengths = {2, 3, 4};
        CHFL_CELL* cell = chfl_cell(lengths);
        REQUIRE(cell != NULL);

        double volume = 0;
        CHECK_STATUS(chfl_cell_volume(cell, &volume));
        CHECK(volume == 2 * 3 * 4);

        CHECK_STATUS(chfl_cell_free(cell));
    }

    SECTION("Matrix") {
        chfl_vector_t lengths = {10, 20, 30};
        CHFL_CELL* cell = chfl_cell(lengths);
        REQUIRE(cell != NULL);

        chfl_vector_t expected[3] = {{10, 0, 0}, {0, 20, 0}, {0, 0, 30}};
        chfl_vector_t matrix[3];
        CHECK_STATUS(chfl_cell_matrix(cell, matrix));
        CHECK(approx_eq(expected, matrix));

        CHECK_STATUS(chfl_cell_free(cell));
    }

    SECTION("Shape") {
        chfl_vector_t lengths = {2, 3, 4};
        CHFL_CELL* cell = chfl_cell(lengths);
        REQUIRE(cell != NULL);

        chfl_cell_shape_t shape;
        CHECK_STATUS(chfl_cell_shape(cell, &shape));
        CHECK(shape == CHFL_CELL_ORTHORHOMBIC);

        CHECK_STATUS(chfl_cell_set_shape(cell, CHFL_CELL_TRICLINIC));
        CHECK_STATUS(chfl_cell_shape(cell, &shape));
        CHECK(shape == CHFL_CELL_TRICLINIC);

        CHECK_STATUS(chfl_cell_set_shape(cell, CHFL_CELL_INFINITE));
        CHECK_STATUS(chfl_cell_shape(cell, &shape));
        CHECK(shape == CHFL_CELL_INFINITE);

        CHECK_STATUS(chfl_cell_free(cell));
    }
}
