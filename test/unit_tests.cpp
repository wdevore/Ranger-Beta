#include <iostream>

// #define CONFIG_CATCH_MAIN
void test_vector();
void test_matrix4();

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

void test_matrix4()
{
    zero_matrix();
    identity_matrix();
    set_translate_matrix();
    get_translate_matrix();
}