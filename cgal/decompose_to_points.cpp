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

  CGAL::convex_decomposition_3(sum_nef);
  printStats(sum_nef, "decomposed sum_nef");

  {
  int num_parts = 0;
  int num_unmarked = 0;
  auto ci = sum_nef.volumes_begin();
  for (; ci != sum_nef.volumes_end(); ++ci) {
    if (ci->mark()) {
      //  Polyhedron_3 P;
      //  N.convert_inner_shell_to_polyhedron(ci->shells_begin(), P);
      //  convex_parts.push_back(P);
      num_parts++;
    }
    else {
      num_unmarked++;
    }
  }

  std::cout << "Number of parts: " << num_parts << std::endl;
  std::cout << "Number of unmarked parts: " << num_unmarked << std::endl;
}
}
