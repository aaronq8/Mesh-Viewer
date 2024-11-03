#pragma once
#include <string>
#include <vector>
#include <GL/glut.h>
#include <vecmath.h>
class Mesh {
    /*
    OBJ format
    v -> vertex x,y,z
    vn -> normal vector x,y,z
    f -> face (vertex_index/vertex_texture_index/normal_index)
    */
    public:
        void load_obj();
        Mesh(std::string);
        void print_vecv();
        void print_vecn();
        void draw_obj();
    private:
        Mesh() = delete;
        std::string obj_path_;
        std::vector<Vector3f> vecv_,vecn_;
        std::vector<std::vector<std::pair<int32_t,int32_t>>>vecf_;
};