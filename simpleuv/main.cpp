#include <iostream>
#include <fstream>
#include <math.h>
#include "meshdatatype.h"
#include "uvunwrapper.h"
#include "objio.hpp"

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cout << "*************************************************************************\n";
        std::cout << "*  Simple UV\n";
        std::cout << "* \n";
        std::cout << "*  Usage: ./simpleuv path/to/obj/file path/to/output/file \n";
        std::cout << "*                    [max_face_num_per_island] [normal_segment_threshold]";
        std::cout << "*************************************************************************\n";
        return -1;
    }
    std::string fname(argv[1]);
    std::string fout_name(argv[2]);
    simpleuv::Mesh m = simpleuv::loadFromObjBinary(fname);
    simpleuv::calcFaceNormals(m);
    simpleuv::UvUnwrapper wrapper;
    wrapper.setMesh(m);
    if (argc >= 4)
        wrapper.setMaxFaceNumPerIsland(std::stoi(argv[3]));
    if (argc >= 5)
        wrapper.setSegmentThreshold(std::stof(argv[4]));
    wrapper.unwrap();
    std::vector<simpleuv::FaceTextureCoords> face_texture_coords = wrapper.getFaceUvs();
    std::vector<simpleuv::Rect> chart_rects = wrapper.getChartRects();
    std::vector<int> chart_source_partictions = wrapper.getChartSourcePartitions();
    float s = wrapper.getTextureSize();

    std::ofstream ofile;
    ofile.open(fout_name);
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
}