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
      {6,5,7},{6,4,5},
      {0,3,1},{0,2,3},
      {4,1,5},{4,0,1},
      {5,3,7},{5,1,3},
      {7,2,6},{7,3,2},
      {6,0,4},{6,2,0},
    
      {14,13,15},{14,12,13},
      {8,11,9},{8,10,11},
      {12,9,13},{12,8,9},
      {13,11,15},{13,9,11},
      {15,10,14},{15,11,10},
      {14,8,12},{14,10,8},
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
  if (nef.is_valid()) {
    std::cout << "Nef is valid" << std::endl;
  } else {
    std::cout << "Nef is NOT valid" << std::endl;
  }
}
