/*
 * Create a corner-case object (two cubes touching along an edge).
 * Try to convert to a Nef Polyhedron via Surface_mesh
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
  auto nef_union = convertUnionAllFaces();
  SurfaceMesh out_mesh;
  convertNefToSurfaceMesh(nef_union, out_mesh);
  writeMesh(out_mesh, "first.off");
}

void processUnionTwoNefCubes() {
  auto second_nef = convertUnionTwoNefCubes();
  SurfaceMesh out_mesh;
  convertNefToSurfaceMesh(second_nef, out_mesh);
  writeMesh(out_mesh, "second.off");
}

void processMeshWithTwoCubesDistinctVertices() {
  auto touching_cubes_nef = convertMeshWithTwoCubesDistinctVertices();
  SurfaceMesh out_mesh;
  convertNefToSurfaceMesh(touching_cubes_nef, out_mesh);
  writeMesh(out_mesh, "third.off");
}

void processMeshWithTwoCubesMergedVertices() {
  auto touching_cubes_nef = convertMeshWithTwoCubesMergedVertices();
  SurfaceMesh out_mesh;
  convertNefToSurfaceMesh(touching_cubes_nef, out_mesh);
  writeMesh(out_mesh, "fourth.off");
}

int main(int argc, char *argv[]) {
  processUnionAllFaces();
  processUnionTwoNefCubes();
  processMeshWithTwoCubesDistinctVertices();
  processMeshWithTwoCubesMergedVertices();
  return 0;
}
