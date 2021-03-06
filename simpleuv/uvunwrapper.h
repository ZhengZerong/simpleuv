#ifndef SIMPLEUV_UV_UNWRAPPER_H
#define SIMPLEUV_UV_UNWRAPPER_H
#include <vector>
#include <map>
#include <simpleuv/meshdatatype.h>
#include <Eigen/Dense>
#include <tuple>

namespace simpleuv
{

class UvUnwrapper
{
public:
    /*** set mesh as well as the partition weights (controlling the uv area of each partition) ***/
    void setMesh(const Mesh &mesh, const std::vector<float> &partitionWeights = std::vector<float>());
    void setTexelSize(float texelSize);
    void setSegmentThreshold(float threshold);
    void setMaxFaceNumPerIsland(int maxnum);
    void setMinIslandSize(int islandsize);
    void unwrap();
    const std::vector<FaceTextureCoords> &getFaceUvs() const;
    const std::vector<Rect> &getChartRects() const;
    const std::vector<int> &getChartSourcePartitions() const;
    float getTextureSize() const;

private:
    void partition();
    void splitPartitionToIslands(const std::vector<size_t> &group, std::vector<std::vector<size_t>> &islands);
    void refineIslandSplitting(const std::vector<std::vector<size_t>> &islands, const std::vector<int> &islandPartitionIds,
                               std::vector<std::vector<size_t>> &islandsRefined, std::vector<int> &islandRefinedPartitionIds);
    void unwrapSingleIsland(const std::vector<size_t> &group, int sourcePartition, bool skipCheckHoles = false);
    void parametrizeSingleGroup(const std::vector<Vertex> &verticies,
                                const std::vector<Face> &faces,
                                std::map<size_t, size_t> &localToGlobalFacesMap,
                                size_t faceNumToChart,
                                int sourcePartition);
    bool fixHolesExceptTheLongestRing(const std::vector<Vertex> &verticies, std::vector<Face> &faces, size_t *remainingHoleNum = nullptr);
    void makeSeamAndCut(const std::vector<Vertex> &verticies,
                        const std::vector<Face> &faces,
                        std::map<size_t, size_t> &localToGlobalFacesMap,
                        std::vector<size_t> &firstGroup, std::vector<size_t> &secondGroup);
    void calculateSizeAndRemoveInvalidCharts();
    void packCharts();
    void finalizeUv();
    void buildEdgeToFaceMap(const std::vector<size_t> &group, std::map<std::pair<size_t, size_t>, size_t> &edgeToFaceMap);
    void buildEdgeToFaceMap(const std::vector<Face> &faces, std::map<std::pair<size_t, size_t>, size_t> &edgeToFaceMap);
    double distanceBetweenVertices(const Vertex &first, const Vertex &second);
    float areaOf3dTriangle(const Eigen::Vector3d &a, const Eigen::Vector3d &b, const Eigen::Vector3d &c);
    float areaOf2dTriangle(const Eigen::Vector2d &a, const Eigen::Vector2d &b, const Eigen::Vector2d &c);
    void triangulateRing(const std::vector<Vertex> &verticies,
                         std::vector<Face> &faces, const std::vector<size_t> &ring);
    void calculateFaceTextureBoundingBox(const std::vector<FaceTextureCoords> &faceTextureCoords,
                                         float &left, float &top, float &right, float &bottom);

    Mesh m_mesh;
    std::vector<FaceTextureCoords> m_faceUvs;
    std::map<int, std::vector<size_t>> m_partitions;
    std::vector<std::pair<std::vector<size_t>, std::vector<FaceTextureCoords>>> m_charts;
    std::vector<std::pair<float, float>> m_chartSizes;
    std::vector<std::pair<float, float>> m_scaledChartSizes;
    std::vector<Rect> m_chartRects;
    std::vector<int> m_chartSourcePartitions;
    std::vector<float> m_partitionWeights;
    bool m_segmentByNormal = true;
    float m_segmentDotProductThreshold = -0.2; //90 degrees
    uint m_maxFaceNumPerIsland = 2000;
    float m_texelSizePerUnit = 1.0;
    float m_resultTextureSize = 0;
    bool m_segmentPreferMorePieces = true;
    bool m_enableRotation = true;
    int m_minIslandSize = 16;
    static const std::vector<float> m_rotateDegrees;
};

} // namespace simpleuv

#endif
