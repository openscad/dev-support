/*

Create a corner-case object (two cubes touching along an edge).
Try to convert to a Nef Polyhedron.

 */

#include <array>
#include <vector>
#include <CGAL/Surface_mesh/Surface_mesh.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using Epeck = CGAL::Exact_predicates_exact_constructions_kernel;
using Nef_polyhedron = CGAL::Nef_polyhedron_3<Epeck>;
using EpeckVertex = Epeck::Point_3;
using DoubleVertex = std::array<double, 3>;

const std::vector<DoubleVertex> vertices = {
    {0, 0, 0},
    {1, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
    {0, 0, 1},
    {1, 0, 1},
    {0, 1, 1},
    {1, 1, 1},

    {1, 1, 0},
    {2, 1, 0},
    {1, 2, 0},
    {2, 2, 0},
    {1, 1, 1},
    {2, 1, 1},
    {1, 2, 1},
    {2, 2, 1},

};

const std::vector<std::array<uint32_t,3>> indices = {
  {6,7,5},{6,5,4},
  {0,1,3},{0,3,2},
  {4,5,1},{4,1,0},
  {5,7,3},{5,3,1},
  {7,6,2},{7,2,3},
  {6,4,0},{6,0,2},
  {14,15,13},{14,13,7},
  {3,9,11},{3,11,10},
  {7,13,9},{7,9,3},
  {13,15,11},{13,11,9},
  {15,14,10},{15,10,11},
  {14,7,3},{14,3,10},
};

int main(int argc, char *argv[])
{
  using SurfaceMesh = CGAL::Surface_mesh<EpeckVertex>;

  SurfaceMesh mesh;

  for (const auto& v : vertices) {
    mesh.add_vertex(EpeckVertex(v[0], v[1], v[2]));
  }
  for (const auto& f : indices) {
    mesh.add_face(SurfaceMesh::Vertex_index(f[0]),
                  SurfaceMesh::Vertex_index(f[1]),
                  SurfaceMesh::Vertex_index(f[2]));
  }

  Nef_polyhedron nef(mesh);

}
