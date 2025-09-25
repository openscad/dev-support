/*

Create a corner-case object (two cubes touching along an edge).
Try to convert to a Nef Polyhedron.

 */

#include <array>
#include <fstream>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/OFF_to_nef_3.h>
#include <CGAL/Surface_mesh/Surface_mesh.h>
#include <CGAL/boost/graph/IO/OFF.h>
#include <CGAL/boost/graph/convert_nef_polyhedron_to_polygon_mesh.h>

#include "cgal_tools.h"
#include "convert.h"

void processUnionAllFaces() {
  auto nef = convertUnionAllFaces();

  SurfaceMesh out_mesh;
  convertNefToSurfaceMesh(nef, out_mesh);
  writeMesh(out_mesh, "first.off");

  std::vector<std::vector<CGAL::Point_3<Double_Kernel>>> parts;
  bool decompose_ok = decompose(nef, parts);
  if (!decompose_ok) {
    std::cerr << "Decomposition failed." << std::endl;
  }
}
void processUnionTwoNefCubes() {
  auto nef = convertUnionTwoNefCubes();

  SurfaceMesh out_mesh;
  convertNefToSurfaceMesh(nef, out_mesh);
  writeMesh(out_mesh, "second.off");

  std::vector<std::vector<CGAL::Point_3<Double_Kernel>>> parts;
  bool decompose_ok = decompose(nef, parts);
  if (!decompose_ok) {
    std::cerr << "Decomposition failed." << std::endl;
  }
}

void processMeshWithTwoCubesDistinctVertices() {
  auto nef = convertMeshWithTwoCubesDistinctVertices();

  SurfaceMesh out_mesh;
  convertNefToSurfaceMesh(nef, out_mesh);
  writeMesh(out_mesh, "third.off");

  std::vector<std::vector<CGAL::Point_3<Double_Kernel>>> parts;
  bool decompose_ok = decompose(nef, parts);
  if (!decompose_ok) {
    std::cerr << "Decomposition failed." << std::endl;
  }
}

void processMeshWithTwoCubesMergedVertices() {
  auto nef = convertMeshWithTwoCubesMergedVertices();

  SurfaceMesh out_mesh;
  convertNefToSurfaceMesh(nef, out_mesh);
  writeMesh(out_mesh, "fourth.off");

  std::vector<std::vector<CGAL::Point_3<Double_Kernel>>> parts;
  bool decompose_ok = decompose(nef, parts);
  if (!decompose_ok) {
    std::cerr << "Decomposition failed." << std::endl;
  }
}

int main(int argc, char *argv[]) {
  processUnionAllFaces();
  processUnionTwoNefCubes();
  processMeshWithTwoCubesDistinctVertices();
  processMeshWithTwoCubesMergedVertices();
  return 0;
}
