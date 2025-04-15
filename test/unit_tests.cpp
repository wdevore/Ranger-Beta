#include <iostream>

// #define CONFIG_CATCH_MAIN
void test_vector();
void test_matrix4();
void test_rectangle();
void test_affinetransform();
void test_node();
void test_spaces();
void test_transform_stack();
void test_node_manager();

int main(int argc, char *argv[])
{
    // std::cout << "argc: " << argc << std::endl;
    // std::cout << "argv0: " << argv[0] << std::endl;
    // std::cout << "argv1: " << argv[1] << std::endl;

    // char selectedTestSuite = *(argv[1]);
    std::string selectedTestSuite = std::string(argv[1]);
    std::cout << "Selected suite: " << '"' << selectedTestSuite << '"' << std::endl;

    if (selectedTestSuite == "vector_suite")
    {
        test_vector();
    }
    else if (selectedTestSuite == "matrix_suite")
    {
        test_matrix4();
    }
    else if (selectedTestSuite == "rectangle_suite")
    {
        test_rectangle();
    }
    else if (selectedTestSuite == "affinetrx_suite")
    {
        test_affinetransform();
    }
    else if (selectedTestSuite == "node_suite")
    {
        test_node();
    }
    else if (selectedTestSuite == "spaces_suite")
    {
        test_spaces();
    }
    else if (selectedTestSuite == "stack_suite")
    {
        test_transform_stack();
    }
    else if (selectedTestSuite == "node_manager_suite")
    {
        test_node_manager();
    }
    else
    {
        std::cout << "No test suite selected" << std::endl;
    }
}

// ---------------- Vector -------------------------
void create_zero_vector();
void vector_add();
void vector_length();
void vector_sub();
void vector_distance();
void vector_angle_between();
void vector_normalize();

void test_vector()
{
    std::cout << "%%%%%%%%%% Vector tests %%%%%%%%%%" << std::endl;
    create_zero_vector();
    vector_add();
    vector_length();
    vector_sub();
    vector_distance();
    vector_angle_between();
    vector_normalize();
}

// ---------------- Matrix4 -------------------------
void zero_matrix();
void identity_matrix();
void set_translate_matrix();
void get_translate_matrix();
void set_rotation_matrix();
void rotate_vector_ccw_matrix();
void rotate_vector_cw_matrix();
void translate_vector_point_matrix();
void scale_vector_matrix();

void test_matrix4()
{
    std::cout << "%%%%%%%%%% Matrix tests %%%%%%%%%%" << std::endl;
    zero_matrix();
    identity_matrix();
    set_translate_matrix();
    get_translate_matrix();
    set_rotation_matrix();
    rotate_vector_ccw_matrix();
    rotate_vector_cw_matrix();
    translate_vector_point_matrix();
    scale_vector_matrix();
}

// ---------------- Rectangle -------------------------
void point_contained_rectangle();
void point_not_contained_rectangle();
void point_inside_rectangle();
void point_inside_right_edge_rectangle();
void rectangle_intersects_rectangle();
void rectangle_not_intersects_rectangle();
void rectangle_contains_rectangle();
void rectangle_not_contains_rectangle();

void test_rectangle()
{
    std::cout << "%%%%%%%%%% Rectangle tests %%%%%%%%%%" << std::endl;
    point_contained_rectangle();
    point_not_contained_rectangle();
    point_inside_rectangle();
    point_inside_right_edge_rectangle();
    rectangle_intersects_rectangle();
    rectangle_not_intersects_rectangle();
    rectangle_contains_rectangle();
    rectangle_not_contains_rectangle();
}

// ---------------- Affinetransform -------------------------
void set_affinetransform();
void maketranslate_affinetransform();
void makeScale_affinetransform();
void scale_affinetransform();
void rotate_ccw_affinetransform();

void test_affinetransform()
{
    std::cout << "%%%%%%%%%% Affinetransform tests %%%%%%%%%%" << std::endl;
    set_affinetransform();
    maketranslate_affinetransform();
    makeScale_affinetransform();
    scale_affinetransform();
    rotate_ccw_affinetransform();
}

// ---------------- Node -------------------------
void create_node();
void append_child_node();
void find_node();

void test_node()
{
    std::cout << "%%%%%%%%%% Node tests %%%%%%%%%%" << std::endl;
    create_node();
    append_child_node();
    find_node();
}

// ---------------- Spaces -------------------------
void map_device_to_view_spaces();

void test_spaces()
{
    std::cout << "%%%%%%%%%% Spaces tests %%%%%%%%%%" << std::endl;
    map_device_to_view_spaces();
}

// ---------------- Transform stack -------------------------
void create_transform_stack();
void save_transform_stack();
void apply_transform_stack();
void restore_transform_stack();

void test_transform_stack()
{
    std::cout << "%%%%%%%%%% Transform stack tests %%%%%%%%%%" << std::endl;
    create_transform_stack();
    save_transform_stack();
    apply_transform_stack();
    restore_transform_stack();
}

// ---------------- Node manager -------------------------
void create_node_manager();

void test_node_manager()
{
    std::cout << "%%%%%%%%%% Node Manager tests %%%%%%%%%%" << std::endl;
    create_node_manager();
}
