#include <mesh.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <GL/glut.h>


Mesh::Mesh(std::string obj_path):
    obj_path_{obj_path}
{}

void Mesh::load_obj(){
    std::ifstream obj{obj_path_};
    std::string line;
    while(std::getline(obj,line)){
        std::vector<std::string> tokens;
        boost::split(tokens,line,boost::is_any_of(" "));
        if(tokens.empty())continue;
        if(tokens[0] == "v"){
            Vector3f vertex{std::stof(tokens[1]),std::stof(tokens[2]),std::stof(tokens[3])};
            vecv_.push_back(vertex);
        }
        else if(tokens[0] == "vn"){
            Vector3f normal{std::stof(tokens[1]),std::stof(tokens[2]),std::stof(tokens[3])};
            vecn_.push_back(normal);
        }
        else if(tokens[0] == "f") {
            std::vector<std::pair<int32_t,int32_t>> cur_face;
            for(int i=1;i<tokens.size();i++){
                std::string cur_side = tokens[i];
                std::vector<std::string> side_info;
                boost::split(side_info,cur_side,boost::is_any_of("/"));
                //(v,vn)
                cur_face.push_back({std::stoi(side_info[0]),std::stoi(side_info[2])});
            }
            vecf_.push_back(cur_face);
        }
    }
}

void Mesh::print_vecn(){
    for(int i=0;i<vecn_.size();i++){
        std::cout<<"vn : "<<vecn_[i][0]<<" "<<vecn_[i][1]<<" "<<vecn_[i][2]<<"\n";
    }
}

void Mesh::print_vecv(){
    for(int i=0;i<vecv_.size();i++){
        std::cout<<"v : "<<vecv_[i][0]<<" "<<vecv_[i][1]<<" "<<vecv_[i][2]<<"\n";
    }
}

void Mesh::draw_obj(){
    glBegin(GL_TRIANGLES);
    for(auto cur_face : vecf_){
        for(auto cur_side : cur_face){
            glNormal3d(vecn_[cur_side.second-1][0],vecn_[cur_side.second-1][1],
                vecn_[cur_side.second-1][2]);
            //draw vertex
            glVertex3d(vecv_[cur_side.first-1][0],vecv_[cur_side.first-1][1],
                vecv_[cur_side.first-1][2]);
        }
    }
    glEnd();
}