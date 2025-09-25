#pragma once

#include <CGAL/boost/graph/convert_nef_polyhedron_to_polygon_mesh.h>
#include <CGAL/convex_decomposition_3.h>

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

void convertNefToSurfaceMesh(const CGAL_Nef_polyhedron3& nef, SurfaceMesh& mesh)
{
  constexpr bool triangulate = false;
  CGAL::convert_nef_polyhedron_to_polygon_mesh(nef, mesh, triangulate);
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

template <typename Output_kernel>
bool decompose(CGAL_Nef_polyhedron3 &nef, std::vector<std::vector<CGAL::Point_3<Output_kernel>>> &out)
{
  CGAL::convex_decomposition_3(nef);
  printStats(nef, "decomposed sum_nef");

  int num_parts = 0;
  int num_unmarked = 0;
  auto ci = nef.volumes_begin();
  for (; ci != nef.volumes_end(); ++ci) {
    if (ci->mark()) {
      // Two ways of extracting points:
      // 1. The usual Nef -> Polyhedron_3 -> points
      // 2. Direct Nef -> points using a shell explorer visitor

      CGAL::Polyhedron_3<CGAL_Kernel3> P;
      nef.convert_inner_shell_to_polyhedron(ci->shells_begin(), P);

      CGAL::Cartesian_converter<CGAL_Kernel3, Double_Kernel> conv;
      std::vector<Double_Point3> out;
      out.reserve(P.size_of_vertices());
      for (auto pi = P.vertices_begin(); pi != P.vertices_end(); ++pi) {
        out.push_back(conv(pi->point()));
      }
      std::cout << "Part " << num_parts << ": " << out.size() << " vertices" << std::endl;


      std::vector<Double_Point3> out2;
      class Add_vertices {

        std::vector<Double_Point3> &out_;
        CGAL::Cartesian_converter<CGAL_Kernel3, Double_Kernel> conv_;

      public:
        Add_vertices(std::vector<Double_Point3> &out)
          : out_(out) {}
  
        void visit(CGAL_Nef_polyhedron3::Vertex_const_handle v) {
          out_.push_back(conv_(v->point()));
        }
  
        void visit(CGAL_Nef_polyhedron3::Halffacet_const_handle) {}
        void visit(CGAL_Nef_polyhedron3::SFace_const_handle) {}
        void visit(CGAL_Nef_polyhedron3::Halfedge_const_handle) {}
        void visit(CGAL_Nef_polyhedron3::SHalfedge_const_handle) {}
        void visit(CGAL_Nef_polyhedron3::SHalfloop_const_handle) {}
      };
  
      Add_vertices A(out2);
      nef.visit_shell_objects(ci->shells_begin(), A);
      std::cout << "Part " << num_parts << ": " << out2.size() << " vertices" << std::endl;

      num_parts++;
    }
    else {
      num_unmarked++;
    }
  }

  std::cout << "Number of parts: " << num_parts << std::endl;
  std::cout << "Number of unmarked parts: " << num_unmarked << std::endl;
  return true;
}
