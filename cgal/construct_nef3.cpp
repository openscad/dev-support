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
#include <CGAL/assertions_behaviour.h>


#include "cgal_tools.h"
#include "objects.h"

int main(int argc, char *argv[]) {

//
// Goal: Create a double-precision mesh consisting of two cubes touching along an edge,
//       and convert it to a Nef Polyhedron.
// Ultimate goal: Decompose a mesh into convex parts and hull them.
//

CGAL::set_error_handler([](const char *type, 
                           const char *expression,
                           const char *file,
                           int line,
                           const char *explanation) {

});


  // Native attempt: Create a mesh, and convert directly to a Nef polyhedron.
  std::cout << "== First attempt: Direct Nef creation == " << std::endl;
  SurfaceMesh touching_cubes_mesh = createSurfaceMesh(touching_cubes);
  writeMesh(touching_cubes_mesh, "touching_cubes.off");

  try {
    CGAL_Nef_polyhedron3 touching_cubes_nef_orig(touching_cubes_mesh);
    std::cout << "== First attempt: OK == " << std::endl;
  } catch (const CGAL::Assertion_exception& e) {
    std::cerr << "Caugh CGAL exception: " << e.what() << std::endl;
  }

  // Second attempt: Just for sanity check, this should always work: Union two cubes without
  // leaving "Nef space".
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

  std::cout << "== Third attempt: Build Mesh from valid Nef, then convert back == " << std::endl;
  SurfaceMesh sum_nef_mesh;
  CGAL::convert_nef_polyhedron_to_polygon_mesh(sum_nef, sum_nef_mesh, /*triangulate=*/true);
  writeMesh(sum_nef_mesh, "sum_nef_mesh.off");
  try {
    CGAL_Nef_polyhedron3 touching_cubes_nef_union(sum_nef_mesh);
    std::cout << "== Third attempt: OK == " << std::endl;
  } catch (const CGAL::Assertion_exception& e) {
    std::cerr << "Caugh CGAL exception: " << e.what() << std::endl;
  }

  std::cout << "== Fourth attempt: Build nef from OFF file == " << std::endl;
  CGAL_Nef_polyhedron3 touching_cubes_nef_from_off;
  std::ifstream stream("touching_cubes.off");
  const auto unhandled_facets = CGAL::OFF_to_nef_3(stream, touching_cubes_nef_from_off);
  if (unhandled_facets == 0) {
    std::cout << "== Fourth attempt: OK == " << std::endl;
  } else {
    std::cerr << "OFF_to_nef_3: " << unhandled_facets << " unhandled facets." << std::endl;
  }

}
