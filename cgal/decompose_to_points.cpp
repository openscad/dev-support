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

  if (!nef.is_valid()) {
    std::cerr << "Nef is not valid!" << std::endl;
    return;
  }

  SurfaceMesh out_mesh;
  convertNefToSurfaceMesh(nef, out_mesh);
  writeMesh(out_mesh, "first.off");

  auto parts = decompose(nef);
}
void processUnionTwoNefCubes() {
  auto nef = convertUnionTwoNefCubes();

  if (!nef.is_valid()) {
    std::cerr << "Nef is not valid!" << std::endl;
    return;
  }

  SurfaceMesh out_mesh;
  convertNefToSurfaceMesh(nef, out_mesh);
  writeMesh(out_mesh, "second.off");

  auto parts = decompose(nef);
}

void processMeshWithTwoCubesDistinctVertices() {
  auto nef = convertMeshWithTwoCubesDistinctVertices();

  if (!nef.is_valid()) {
    std::cerr << "Nef is not valid!" << std::endl;
    return;
  }

  SurfaceMesh out_mesh;
  convertNefToSurfaceMesh(nef, out_mesh);
  writeMesh(out_mesh, "third.off");

  auto parts = decompose(nef);
}

void processMeshWithTwoCubesMergedVertices() {
  auto nef = convertMeshWithTwoCubesMergedVertices();

  if (!nef.is_valid()) {
    std::cerr << "Nef is not valid!" << std::endl;
    return;
  }

  SurfaceMesh out_mesh;
  convertNefToSurfaceMesh(nef, out_mesh);
  writeMesh(out_mesh, "fourth.off");

  auto parts = decompose(nef);
}

int main(int argc, char *argv[]) {
  processUnionAllFaces();
  processUnionTwoNefCubes();
  processMeshWithTwoCubesDistinctVertices();
  processMeshWithTwoCubesMergedVertices();
  return 0;
}
