#include <iostream>
#include <fstream>
#include <math.h>
#include "meshdatatype.h"
#include "uvunwrapper.h"
#include "objio.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "*************************************************************************\n";
        std::cout << "*  Simple UV\n";
        std::cout << "* \n";
        std::cout << "*  Usage: ./simpleuv_gen path/to/obj/file path/to/output/file \n";
        std::cout << "*************************************************************************\n";
        return -1;
    }
    std::string fname(argv[1]);
    std::string fout_name(argv[2]);
    simpleuv::Mesh m = simpleuv::loadFromObjBinary(fname);
    simpleuv::UvUnwrapper wrapper;
    wrapper.setMesh(m);
    wrapper.unwrap();
    std::vector<simpleuv::FaceTextureCoords> face_texture_coords = wrapper.getFaceUvs();
    std::vector<simpleuv::Rect> chart_rects = wrapper.getChartRects();
    std::vector<int> chart_source_partictions = wrapper.getChartSourcePartitions();
    float s = wrapper.getTextureSize();

    std::ofstream ofile;
    ofile.open("./test/face_texture_coords.txt");
    for (int fi = 0; fi < face_texture_coords.size(); fi++)
    {
        for (int k = 0; k < 3; k++)
        {
            ofile << face_texture_coords[fi].coords[k].uv[0] << " ";
            ofile << face_texture_coords[fi].coords[k].uv[1] << " ";
        }
        ofile << std::endl;
    }
    ofile.close();

    ofile.open("./test/chart_rects.txt");
    for (int i = 0; i < chart_rects.size(); i++)
    {
        ofile << chart_rects[i].left << " ";
        ofile << chart_rects[i].top << " ";
        ofile << chart_rects[i].width << " ";
        ofile << chart_rects[i].height << " ";
        ofile << std::endl;
    }
    ofile.close();

    ofile.open("./test/chart_source_partictions.txt");
    for (int i = 0; i < chart_source_partictions.size(); i++)
    {
        ofile << chart_source_partictions[i] << std::endl;
    }
    ofile.close();          
}