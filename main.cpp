#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <array>
#include <iostream>
#include "glm/gtx/string_cast.hpp"

using namespace glm;

struct Plane {
    vec3 normal;
    float d;

    void set(vec4 equation) {
        this->normal = vec3(equation.x, equation.y, equation.z);
        this->d = equation.w;
    }

    float distance(vec3 point) const {
        return (normal.x * point.x) + (normal.y * point.y) + (normal.z * point.z) + d;
    };
};

float width = 900;
float height = 600;
float aspect = width / height;
float fov = 90.0f;
float far_dist = 100;
float near_dist = 0.1;
mat4 view_mat = glm::inverse(mat4(1.0f));
mat4 projection_mat = glm::perspective(fov, aspect, near_dist, far_dist);
mat4 m = projection_mat * view_mat;
std::array<Plane, 6> planes;
std::array<std::string, 6> planes_names = {"left", "right", "bottom", "top", "near", "far"};

void printPlanesData() {
    for (int i = 0; i < planes.size(); ++i) {

        std::cout << '\n' << (std::to_string(i)) << " " << planes_names[i] << std::endl;

        std::cout << ("equation:" + std::to_string(planes[i].normal.x) + "a+" + std::to_string(planes[i].normal.y) +
                      "b+" +
                      std::to_string(planes[i].normal.z) + "c=" + std::to_string(planes[i].d)) << std::endl;

        std::cout << ("normal:(" + std::to_string(planes[i].normal.x) + "," + std::to_string(planes[i].normal.y) + "," +
                      std::to_string(planes[i].normal.z) + ")") << std::endl;

        std::cout << ("d:" + std::to_string(planes[i].d)) << std::endl;
    }
}

void testPointInsideFrustum(vec3 point) {
    std::cout << "\n(" + std::to_string(point.x) + "," + std::to_string(point.y) + "," +
                 std::to_string(point.z) + "):" << std::endl;
    bool inside = true;
    for (int i = 0; i < planes.size(); i++) {
        float d = planes[i].distance(point);
        std::cout << ("distance " + planes_names[i] + ":" + std::to_string(d)) << std::endl;
        if (0 > d) {
            inside = false;
        }
    }
    std::cout << std::string(inside ? "inside" : "outside") << std::endl;
}

std::string get_matrix_sting(mat4 mat) {
    //transpose because glm matrix are column major and printing a vector print a line
    glm::transpose(mat);
    return std::string(glm::to_string(mat[0]) + '\n' + glm::to_string(mat[1]) + '\n' + glm::to_string(mat[2]) + '\n' +
                       glm::to_string(mat[3]));
}

int main() {
    std::cout << "aspect:" << std::to_string(aspect) << std::endl << std::endl;
    std::cout << "near distance:" << std::to_string(near_dist) << std::endl << std::endl;
    std::cout << "far distance:" << std::to_string(far_dist) << std::endl << std::endl;
    std::cout << "fov:" << std::to_string(fov) << std::endl << std::endl;
    std::cout << "projection matrix:\n" << get_matrix_sting(projection_mat) << std::endl << std::endl;
    std::cout << "view matrix:\n" << get_matrix_sting(view_mat) << std::endl << std::endl;
    std::cout << "projection*view:\n" << get_matrix_sting(projection_mat * view_mat) << std::endl << std::endl;
    planes[0].set((vec4(m[0][3] + m[0][0], m[1][3] + m[1][0], m[2][3] + m[2][0], m[3][3] + m[3][0])));//left
    planes[1].set((vec4(m[0][3] - m[0][0], m[1][3] - m[1][0], m[2][3] - m[2][0], m[3][3] - m[3][0])));//right
    planes[3].set((vec4(m[0][3] + m[0][1], m[1][3] + m[1][1], m[2][3] + m[2][1], m[3][3] + m[3][1])));//bottom
    planes[2].set((vec4(m[0][3] - m[0][1], m[1][3] - m[1][1], m[2][3] - m[2][1], m[3][3] - m[3][1])));//top
    planes[4].set((vec4(m[0][3] + m[0][2], m[1][3] + m[1][2], m[2][3] + m[2][2], m[3][3] + m[3][2])));//near
    planes[5].set((vec4(m[0][3] - m[0][2], m[1][3] - m[1][2], m[2][3] - m[2][2], m[3][3] - m[3][2])));//far
    printPlanesData();


    //directly on near plane
    testPointInsideFrustum(vec3(0, 0, -0.1));
    //directly on far plane
    testPointInsideFrustum(vec3(0, 0, -100));
    //in
    testPointInsideFrustum(vec3(0, 0, -0.2));
    testPointInsideFrustum(vec3(0, 0, -99));
    //out
    testPointInsideFrustum(vec3(0, 0, -101));
    testPointInsideFrustum(vec3(0, 0, 0));

    return 0;
}