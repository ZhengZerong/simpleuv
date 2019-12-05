import numpy as np
import cv2 as cv
import os
import glob
import math


def load_obj_data(filename):
    """load model data from .obj file"""
    v_list = []  # vertex coordinate
    vt_list = []  # vertex texture coordinate
    vc_list = []  # vertex color
    vn_list = []  # vertex normal
    f_list = []  # face vertex indices
    fn_list = []  # face normal indices
    ft_list = []  # face texture indices

    # read data
    fp = open(filename, 'r')
    lines = fp.readlines()
    fp.close()

    for line in lines:
        if len(line) < 2:
            continue
        line_data = line.strip().split(' ')
        # parse vertex cocordinate
        if line_data[0] == 'v':
            v_list.append((float(line_data[1]), float(line_data[2]), float(line_data[3])))
            if len(line_data) == 7:
                vc_list.append((float(line_data[4]), float(line_data[5]), float(line_data[6])))

        # parse vertex texture coordinate
        if line_data[0] == 'vt':
            vt_list.append((float(line_data[1]), float(line_data[2])))

        # parse vertex normal
        if line_data[0] == 'vn':
            vn_list.append((float(line_data[1]), float(line_data[2]), float(line_data[3])))

        # parse face
        if line_data[0] == 'f':
            # used for parsing face element data
            def segElementData(ele_str):
                fv = None
                ft = None
                fn = None
                eles = ele_str.strip().split('/')
                if len(eles) == 1:
                    fv = int(eles[0]) - 1
                elif len(eles) == 2:
                    fv = int(eles[0]) - 1
                    ft = int(eles[1]) - 1
                elif len(eles) == 3:
                    fv = int(eles[0]) - 1
                    fn = int(eles[2]) - 1
                    ft = None if eles[1] == '' else int(eles[1]) - 1
                return fv, ft, fn

            fv0, ft0, fn0 = segElementData(line_data[1])
            fv1, ft1, fn1 = segElementData(line_data[2])
            fv2, ft2, fn2 = segElementData(line_data[3])
            f_list.append((fv0, fv1, fv2))
            if ft0 is not None and ft1 is not None and ft2 is not None:
                ft_list.append((ft0, ft1, ft2))
            if fn0 is not None and fn1 is not None and fn2 is not None:
                fn_list.append((fn0, fn1, fn2))

    v_list = np.asarray(v_list)
    vn_list = np.asarray(vn_list)
    vt_list = np.asarray(vt_list)
    vc_list = np.asarray(vc_list)
    f_list = np.asarray(f_list)
    ft_list = np.asarray(ft_list)
    fn_list = np.asarray(fn_list)

    model = {'v': v_list, 'vt': vt_list, 'vc': vc_list, 'vn': vn_list,
             'f': f_list, 'ft': ft_list, 'fn': fn_list}
    return model


def calc_barycentric_coord(p, a, b, c):
    den = a[0]*b[1] + b[0]*c[1] + c[0]*a[1] - a[0]*c[1] - b[0]*a[1] - c[0]*b[1] + 1e-3
    num1 = p[0]*b[1] + b[0]*c[1] + c[0]*p[1] - p[0]*c[1] - b[0]*p[1] - c[0]*b[1]
    num2 = a[0]*p[1] + p[0]*c[1] + c[0]*a[1] - a[0]*c[1] - p[0]*a[1] - c[0]*p[1]
    num3 = a[0]*b[1] + b[0]*p[1] + p[0]*a[1] - a[0]*p[1] - b[0]*a[1] - p[0]*b[1]
    return [num1/den, num2/den, num3/den]


def main():
    mesh = load_obj_data('./test/test_image_5.obj')
    face_texture_coords = np.loadtxt('./test/test_image_5.tex_uv.txt')
    res = 1024
    img = np.zeros((res, res, 3), dtype=np.uint8)
    for f, ft in zip(mesh['f'], face_texture_coords):
        vc0, vc1, vc2 = mesh['vc'][f[0]], mesh['vc'][f[1]], mesh['vc'][f[2]]

        ft = [int(round(fti * res)) for fti in ft]
        u0, v0, u1, v1, u2, v2 = ft[0], ft[1], ft[2], ft[3], ft[4], ft[5]
        top = min(min(u0, u1), u2)
        bottom = max(max(u0, u1), u2)
        left = min(min(v0, v1), v2)
        right = max(max(v0, v1), v2)
        for uu in range(top, bottom):
            for vv in range(left, right):
                lambda0, lambda1, lambda2 = calc_barycentric_coord(
                    [uu, vv], [u0, v0], [u1, v1], [u2, v2])
                if lambda0 >= 0 and lambda1 >= 0 and lambda2 >=0:
                    c = vc0 * lambda0 + vc1 * lambda1 + vc2 * lambda2
                    img[uu, vv] = np.uint8(c*255)

    cv.imwrite('./test/face_texture_coords_vis.png', img[:, :, ::-1])


if __name__ == '__main__':
    main()