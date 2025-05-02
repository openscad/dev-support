#include <array>
#include <vector>

#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Surface_mesh/Surface_mesh.h>

using NT3 = CGAL::Gmpq;
using CGAL_Kernel3 = CGAL::Cartesian<NT3>;
using CGAL_Vertex = CGAL::Point_3<CGAL_Kernel3>;

struct Object {
  std::vector<std::array<double, 3>> vertices;
  std::vector<std::array<uint32_t, 3>> indices;
};

int main(int argc, char *argv[]) {
  const Object touching_cubes = {
    .vertices = {
      {0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0},
      {0, 0, 1}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1},
  
      {1, 1, 0}, {2, 1, 0}, {1, 2, 0}, {2, 2, 0},
      {1, 1, 1}, {2, 1, 1}, {1, 2, 1}, {2, 2, 1},
    },
    .indices = {
      {6,7,5},{6,5,4},
      {0,1,3},{0,3,2},
      {4,5,1},{4,1,0},
      {5,7,3},{5,3,1},
      {7,6,2},{7,2,3},
      {6,4,0},{6,0,2},
    
      {14,15,13},{14,13,12},
      {8,9,11},{8,11,10},
      {12,13,9},{12,9,8},
      {13,15,11},{13,11,9},
      {15,14,10},{15,10,11},
      {14,12,8},{14,8,10},
    }  
  };
    
using SurfaceMesh = CGAL::Surface_mesh<CGAL_Vertex>;
SurfaceMesh mesh;
  for (const auto &v : touching_cubes.vertices) {
    mesh.add_vertex({v[0], v[1], v[2]});
  }
  for (const auto &f : touching_cubes.indices) {
    mesh.add_face(SurfaceMesh::Vertex_index(f[0]),
                  SurfaceMesh::Vertex_index(f[1]), 
                  SurfaceMesh::Vertex_index(f[2]));
  }

  CGAL::Nef_polyhedron_3<CGAL_Kernel3> nef(mesh);
}
