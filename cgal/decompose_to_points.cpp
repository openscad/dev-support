/*

Create a corner-case object (two cubes touching along an edge).
Try to convert to a Nef Polyhedron.

 */

#include <array>
#include <fstream>
#include <vector>


#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Surface_mesh/Surface_mesh.h>
#include <CGAL/convex_decomposition_3.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include <CGAL/boost/graph/IO/OFF.h>
#include <CGAL/boost/graph/convert_nef_polyhedron_to_polygon_mesh.h>
#include <CGAL/OFF_to_nef_3.h>

#include "cgal_tools.h"
#include "objects.h"

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

int main(int argc, char *argv[]) {

  std::cout << "== Second attempt: Build Nef from two cubes == " << std::endl;
  SurfaceMesh first_cube_mesh = createSurfaceMesh(first_cube);
  SurfaceMesh second_cube_mesh = createSurfaceMesh(second_cube);
  writeMesh(first_cube_mesh, "first_cube.off");
  writeMesh(second_cube_mesh, "second_cube.off");
  CGAL_Nef_polyhedron3 first_cube_nef(first_cube_mesh);
  CGAL_Nef_polyhedron3 second_cube_nef(second_cube_mesh);

  CGAL_Nef_polyhedron3 sum_nef;
  sum_nef += first_cube_nef;
  sum_nef += second_cube_nef;
  writeNef(sum_nef, "sum_nef.nef3");
  std::cout << "== Second attempt: OK == " << std::endl;

  std::vector<std::vector<CGAL::Point_3<Double_Kernel>>> parts;
  bool decompose_ok = decompose(sum_nef, parts);
  if (!decompose_ok) {
    std::cerr << "Decomposition failed." << std::endl;
    return 1;
  }
}
