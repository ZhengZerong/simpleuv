#ifndef __OBJIO_HPP__
#define __OBJIO_HPP__

#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>
#include <simpleuv/meshdatatype.h>

namespace simpleuv
{

float readFloat(std::ifstream &_ifs)
{
    char buf[32] = {0};
    int pt = 0;

    while (!_ifs.eof())
    {
        char c = 0;
        _ifs.read(&c, sizeof(char));
        if (c == ' ' || c == '\n')
            break;
        buf[pt] = c;
        pt++;
    };

    if (pt == 0)
        return 0.f;
    return std::stof(buf);
}

float readInt(std::ifstream &_ifs)
{
    char buf[32] = {0};
    int pt = 0;

    while (!_ifs.eof())
    {
        char c = 0;
        _ifs.read(&c, sizeof(char));
        if (c == ' ' || c == '\n')
            break;
        buf[pt] = c;
        pt++;
    };

    if (pt == 0)
        return 0.f;
    return std::stoi(buf);
}

std::vector<std::string> split(const std::string &s, const std::string &seperator)
{
    std::vector<std::string> result;
    typedef std::string::size_type string_size;
    string_size i = 0;

    while (i != s.size())
    {
        int flag = 0;
        while (i != s.size() && flag == 0)
        {
            flag = 1;
            for (string_size x = 0; x < seperator.size(); ++x)
                if (s[i] == seperator[x])
                {
                    ++i;
                    flag = 0;
                    break;
                }
        }

        flag = 0;
        string_size j = i;
        while (j != s.size() && flag == 0)
        {
            for (string_size x = 0; x < seperator.size(); ++x)
                if (s[j] == seperator[x])
                {
                    flag = 1;
                    break;
                }
            if (flag == 0)
                ++j;
        }
        if (i != j)
        {
            result.push_back(s.substr(i, j - i));
            i = j;
        }
    }
    std::vector<std::string> result_filtered;
    for (int i = 0; i < result.size(); i++)
    {
        if (result[i].length() > 0)
            result_filtered.push_back(result[i]);
    }
    return result_filtered;
}

Mesh loadFromObjBinary(const std::string &_filename)
{
    Mesh m;
    std::ifstream ifile(_filename, std::ifstream::binary);
    if (!ifile.is_open())
    {
        std::cout << "Failed to open " << _filename << std::endl;
        return m;
    }

    char vf, sp;
    while (!ifile.eof())
    {
        std::string line;
        std::getline(ifile, line);
        if (line.c_str()[0] == '#')
            continue;
        std::vector<std::string> linedata = split(line, std::string(" "));
        if (linedata.size() < 3)
            continue;
        if (linedata[0] == std::string("v"))
        {
            Vertex v;
            v.xyz[0] = std::stof(linedata[1]);
            v.xyz[1] = std::stof(linedata[2]);
            v.xyz[2] = std::stof(linedata[3]);
            m.vertices.push_back(v);
        }
        else if (linedata[0] == std::string("f"))
        {
            Face f;
            f.indices[0] = std::stoi(linedata[1]);
            f.indices[1] = std::stoi(linedata[2]);
            f.indices[2] = std::stoi(linedata[3]);
            m.faces.push_back(f);
        }
        else if (linedata[0] == std::string("vn"))
        {
            continue;
        }
        else if (linedata[0] == std::string("vc"))
        {
            continue;
        }
        else if (linedata[0] == std::string("vt"))
        {
            continue;
        }
    }

    std::cout << "Loaded " << m.vertices.size() << " vertices and " << m.faces.size() << " triangles. \n";
    return m;
}

void calcFaceNormals(Mesh &m)
{
    m.faceNormals.resize(m.faces.size());
    for (int fi = 0; fi < m.faces.size(); fi++)
    {
        Face f = m.faces[fi];
        Vertex v0 = m.vertices[f.indices[0]], v1 = m.vertices[f.indices[1]], v2 = m.vertices[f.indices[2]];
        Vector3 v01, v02;
        for (int i = 0; i < 3; i++)
        {
            v01.xyz[i] = v1.xyz[i] - v0.xyz[i];
            v02.xyz[i] = v2.xyz[i] - v0.xyz[i];
        }
        Vector3 n = crossProduct(v01, v02);
        float l = sqrtf(n.xyz[0] * n.xyz[0] + n.xyz[1] * n.xyz[1] + n.xyz[2] * n.xyz[2]);
        for (int i = 0; i < 3; i++)
            n.xyz[i] /= l;
        m.faceNormals[fi] = n;
    }
}

} // namespace simpleuv

#endif