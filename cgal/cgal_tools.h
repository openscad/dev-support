#pragma once

using NT3 = CGAL::Gmpq;
using CGAL_Kernel3 = CGAL::Cartesian<NT3>;
using CGAL_Nef_polyhedron3 = CGAL::Nef_polyhedron_3<CGAL_Kernel3>;
using CGAL_Vertex = CGAL::Point_3<CGAL_Kernel3>;
using DoubleVertex = std::array<double, 3>;


using Double_Kernel = CGAL::Simple_cartesian<double>;
using Double_Point3 = CGAL::Point_3<Double_Kernel>;

struct Object {
  std::vector<DoubleVertex> vertices;
  std::vector<std::array<uint32_t, 3>> indices;
};

using SurfaceMesh = CGAL::Surface_mesh<CGAL_Vertex>;
    
SurfaceMesh createSurfaceMesh(const Object& obj) {
  SurfaceMesh mesh;

  for (const auto &v : obj.vertices) {
    mesh.add_vertex({v[0], v[1], v[2]});
  }
  for (const auto &f : obj.indices) {
    mesh.add_face(SurfaceMesh::Vertex_index(f[0]),
                  SurfaceMesh::Vertex_index(f[1]),
                  SurfaceMesh::Vertex_index(f[2]));
  }
  return mesh;
}

void writeMesh(const SurfaceMesh& mesh, const std::string& filename) {
  bool write_ok = CGAL::IO::write_OFF(filename, mesh);
  if (!write_ok) {
    std::cerr << "Error writing mesh to output" << std::endl;
    exit(1);
  }
}

void writeNef(CGAL_Nef_polyhedron3& nef, const std::string& filename) {
  std::ofstream out(filename);
  if (!out) {
    std::cerr << "Error opening file for writing: " << filename << std::endl;
    exit(1);
  }
  out << nef;
  out.close();
}

void printStats(CGAL_Nef_polyhedron3& nef, const std::string& name) {

  std::cout << name << ":\n";
  std::cout << "  is_simple: " << nef.is_simple() << std::endl;
  std::cout << "  is_valid: " << nef.is_valid() << std::endl;
  std::cout << "  is_bounded: " << nef.is_bounded() << std::endl;
  std::cout << "  number_of_vertices: " << nef.number_of_vertices()
            << std::endl;
  std::cout << "  number_of_volumes: " << nef.number_of_volumes() << std::endl;

}


