/*

Read an OFF file and
Try to convert to a Nef Polyhedron.

 */

#include <iostream>
#include <fstream>
#include <vector>
#include <CGAL/Surface_mesh/Surface_mesh.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Gmpq.h>
#include <CGAL/IO/OFF.h>
#pragma push_macro("NDEBUG")
#undef NDEBUG
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#pragma pop_macro("NDEBUG")

using Kernel = CGAL::Cartesian<CGAL::Gmpq>;
using Nef_polyhedron = CGAL::Nef_polyhedron_3<Kernel>;
using Vertex = Kernel::Point_3;

int main(int argc, char *argv[])
{
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <input.off> <output.nef3>" << std::endl;
    return 1;
  }

  using SurfaceMesh = CGAL::Surface_mesh<Vertex>;
  SurfaceMesh mesh;

  std::ifstream input(argv[1]);
  if (!input) {
    std::cerr << "Cannot open file " << argv[1] << std::endl;
    return 1;
  }

  if (!CGAL::IO::read_OFF(input, mesh)) {
    std::cerr << "Error reading OFF file " << argv[1] << std::endl;
    return 1;
  }

  try {
    Nef_polyhedron nef(mesh);
    std::cout << "Successfully created Nef polyhedron from " << argv[1] << std::endl;

    std::ofstream output(argv[2]);
    if (!output) {
      std::cerr << "Cannot open file for writing " << argv[2] << std::endl;
      return 1;
    }
    output << nef;
    std::cout << "Successfully wrote Nef polyhedron to " << argv[2] << std::endl;
  } catch (const CGAL::Assertion_exception& e) {
    std::cerr << "CGAL assertion while creating Nef polyhedron: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
