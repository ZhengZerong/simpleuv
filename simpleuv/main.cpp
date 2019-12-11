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
        std::cout << "*****************************************************************\n";
        std::cout << "*  Simple UV\n";
        std::cout << "* \n";
        std::cout << "*  Usage: ./simpleuv path/to/obj/file path/to/output/file \n";
        std::cout << "*                    [path/to/face/partition/file=None] ";
        std::cout << "*                    [path/to/partition/weights/file=None]";
        std::cout << "*                    [max_face_num_per_island=2000]";
        std::cout << "*                    [normal_segment_threshold=-0.2]";
        std::cout << "*                    [min_island_size=-0.2]";
        std::cout << "*****************************************************************\n";
        return -1;
    }
    std::cout << "************************************************\n";
    std::cout << argv[0] << std::endl;
    for (int i = 1; i < argc; i++)
    {
        std::cout << "    " << argv[i] << std::endl;
    }
    std::cout << "************************************************\n";

    std::string fname(argv[1]);
    std::string fout_name(argv[2]);
    simpleuv::Mesh m = simpleuv::loadFromObjBinary(fname);
    simpleuv::calcFaceNormals(m);
    simpleuv::UvUnwrapper wrapper;
    if (argc >= 4)
    {
        if (std::string(argv[3]) != std::string("None"))
        {
            std::ifstream ifile(argv[3]);
            m.facePartitions.resize(m.faces.size());
            for (int fi = 0; fi < m.faces.size(); fi++)
            {
                ifile >> m.facePartitions[fi];
            }
            ifile.close();
            std::cout << "Face partition loaded.\n";
        }
    }
    wrapper.setMesh(m);
    if (argc >= 5)
    {
        if (std::string(argv[4]) != std::string("None"))
        {
            std::ifstream ifile(argv[4]);
            std::vector<float> ws;
            while (!ifile.eof())
            {
                float w;
                ifile >> w;
                ws.push_back(w);
            }
            ifile.close();
            wrapper.setMesh(m, ws);
            std::cout << "Partition weights loaded.\n";
        }
    }
    if (argc >= 6)
        wrapper.setMaxFaceNumPerIsland(std::stoi(argv[5]));
    if (argc >= 7)
        wrapper.setSegmentThreshold(std::stof(argv[6]));
    if (argc >= 8)
        wrapper.setMinIslandSize(std::stoi(argv[7]));
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
    printf("Uv unwrapping done. \n");
}