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
#include "objects.h"

void convertUnionAllFaces() {
  std::cout << "== First attempt: Build nef by unioning all faces == "
            << std::endl;
  SurfaceMesh touching_cubes_mesh = createSurfaceMesh(touching_cubes);
  writeMesh(touching_cubes_mesh, "first_touching_cubes.off");
  const auto &mesh = touching_cubes_mesh;
  CGAL::Nef_nary_union_3<CGAL_Nef_polyhedron3> nary_union;
  int discarded_facets = 0;
  for (const auto face : mesh.faces()) {
    std::vector<CGAL::Point_3<CGAL_Kernel3>> vertices;
    for (auto vd : CGAL::vertices_around_face(mesh.halfedge(face), mesh)) {
      vertices.push_back(mesh.point(vd));
    }

    bool is_nef = false;
    if (vertices.size() >= 1) {
      CGAL_Nef_polyhedron3 nef(vertices.begin(), vertices.end());
      if (!nef.is_empty()) {
        nary_union.add_polyhedron(nef);
        is_nef = true;
      }
    }
    if (!is_nef) {
      discarded_facets++;
    }
  }
  if (discarded_facets > 0) {
    std::cerr << "Discarded " << discarded_facets << " facets." << std::endl;
  }
  CGAL_Nef_polyhedron3 nef_union = nary_union.get_union();
  CGAL::Mark_bounded_volumes<CGAL_Nef_polyhedron3> mbv(true);
  nef_union.delegate(mbv);
  writeNef(nef_union, "first.nef3");
  printStats(nef_union, "first");
  SurfaceMesh out_mesh;
  convertNefToSurfaceMesh(nef_union, out_mesh);
  writeMesh(out_mesh, "first.off");
}

void convertUnionTwoNefCubes() {
  std::cout << "== Second attempt: Build Nef from two cubes == " << std::endl;
  SurfaceMesh first_cube_mesh = createSurfaceMesh(first_cube);
  SurfaceMesh second_cube_mesh = createSurfaceMesh(second_cube);
  writeMesh(first_cube_mesh, "first_cube1.off");
  writeMesh(second_cube_mesh, "first_cube2.off");
  CGAL_Nef_polyhedron3 first_cube_nef(first_cube_mesh);
  CGAL_Nef_polyhedron3 second_cube_nef(second_cube_mesh);

  CGAL_Nef_polyhedron3 sum_nef;
  sum_nef += first_cube_nef;
  sum_nef += second_cube_nef;
  writeNef(sum_nef, "second.nef3");
  printStats(sum_nef, "second");
  SurfaceMesh out_mesh;
  convertNefToSurfaceMesh(sum_nef, out_mesh);
  writeMesh(out_mesh, "second.off");
}

void convertMeshWithTwoCubesDistinctVertices() {
  std::cout << "== Third attempt: Build Nef from a mesh with two cubes "
               "(distinct vertices) == "
            << std::endl;
  SurfaceMesh touching_cubes_mesh = createSurfaceMesh(touching_cubes);
  writeMesh(touching_cubes_mesh, "third_touching_cubes.off");
  CGAL_Nef_polyhedron3 touching_cubes_nef(touching_cubes_mesh);
  writeNef(touching_cubes_nef, "third.nef3");
  printStats(touching_cubes_nef, "third");
  SurfaceMesh out_mesh;
  convertNefToSurfaceMesh(touching_cubes_nef, out_mesh);
  writeMesh(out_mesh, "third.off");
}

void convertMeshWithTwoCubesMergedVertices() {
  std::cout << "== Fourth attempt: Build Nef from a mesh with two cubes "
               "(merged vertices) == "
            << std::endl;
  SurfaceMesh touching_cubes_mesh = createSurfaceMesh(touching_cubes_14);
  writeMesh(touching_cubes_mesh, "fourth_touching_cubes.off");
  CGAL_Nef_polyhedron3 touching_cubes_nef(touching_cubes_mesh);
  writeNef(touching_cubes_nef, "fourth.nef3");
  printStats(touching_cubes_nef, "fourth");
  SurfaceMesh out_mesh;
  convertNefToSurfaceMesh(touching_cubes_nef, out_mesh);
  writeMesh(out_mesh, "fourth.off");
}

int main(int argc, char *argv[]) {
  convertUnionAllFaces();
  convertUnionTwoNefCubes();
  convertMeshWithTwoCubesDistinctVertices();
  convertMeshWithTwoCubesMergedVertices();
  return 0;
}
