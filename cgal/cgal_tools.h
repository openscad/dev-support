#pragma once

#include <CGAL/Nef_nary_union_3.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/convert_nef_polyhedron_to_polygon_mesh.h>
#include <CGAL/convex_decomposition_3.h>
#include <CGAL/convex_hull_3.h>

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

template <typename Mesh>
void writeMesh(const Mesh &mesh, const std::string &filename) {
  bool write_ok = CGAL::IO::write_OFF(filename, mesh);
  if (!write_ok) {
    std::cerr << "Error writing mesh to output" << std::endl;
    exit(1);
  }
}

void writeNef(CGAL_Nef_polyhedron3 &nef, const std::string &filename) {
  std::ofstream out(filename);
  if (!out) {
    std::cerr << "Error opening file for writing: " << filename << std::endl;
    exit(1);
  }
  out << nef;
  out.close();
}

void printStats(CGAL_Nef_polyhedron3 &nef, const std::string &name) {

  std::cout << name << ":\n";
  std::cout << "  is_simple: " << nef.is_simple() << std::endl;
  std::cout << "  is_valid: " << nef.is_valid() << std::endl;
  std::cout << "  is_bounded: " << nef.is_bounded() << std::endl;
  std::cout << "  number_of_vertices: " << nef.number_of_vertices()
            << std::endl;
  std::cout << "  number_of_volumes: " << nef.number_of_volumes() << std::endl;
}

SurfaceMesh createSurfaceMesh(const Object &obj) {
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

void convertNefToSurfaceMesh(const CGAL_Nef_polyhedron3 &nef,
                             SurfaceMesh &mesh) {
  constexpr bool triangulate = false;
  CGAL::convert_nef_polyhedron_to_polygon_mesh(nef, mesh, triangulate);
}

CGAL_Nef_polyhedron3 unionMeshFacesToNef(const SurfaceMesh &mesh) {
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
  return nef_union;
}

CGAL_Nef_polyhedron3 convertSurfaceMeshToNef(const SurfaceMesh &mesh) {
  // Note: This may cause a CGAL exception if the input mesh is
  // self-intersecting: If a very thin part of an object collapses into one
  // floating point coordinate, but the vertices are still distinct, it's
  // considered a self-intersection. This is valid in e.g. Manifold, but invalid
  // in SurfaceMesh -> Nef.
  // If this happens, we fall back to unioning all faces, which is slower but more
  // robust.
  try {
    CGAL_Nef_polyhedron3 touching_cubes_nef(mesh);
    writeNef(touching_cubes_nef, "third.nef3");
    printStats(touching_cubes_nef, "third");
    return touching_cubes_nef;
  } catch (const CGAL::Assertion_exception &e) {
    std::cerr
        << "Warning: CGAL error in CGAL_Nef_polyhedron3(): Attempting union..."
        << std::endl;
    return unionMeshFacesToNef(mesh);
  }
}

std::vector<std::vector<Double_Point3>>
decompose(CGAL_Nef_polyhedron3 &nef, bool use_shell_exploration = false) {
  CGAL::convex_decomposition_3(nef);
  printStats(nef, "decomposed sum_nef");

  std::vector<std::vector<Double_Point3>> parts;
  int num_parts = 0;
  int num_unmarked = 0;
  auto ci = nef.volumes_begin();
  for (; ci != nef.volumes_end(); ++ci) {
    if (ci->mark()) {
      // Two ways of extracting points:
      // 1. The usual Nef -> Polyhedron_3 -> points
      // 2. Direct Nef -> points using a shell explorer visitor

      if (!use_shell_exploration) {
        // Method 1: Nef -> Polyhedron_3 -> points
        CGAL::Polyhedron_3<CGAL_Kernel3> P;
        nef.convert_inner_shell_to_polyhedron(ci->shells_begin(), P);

        CGAL::Cartesian_converter<CGAL_Kernel3, Double_Kernel> conv;
        auto &out = parts.emplace_back();
        out.reserve(P.size_of_vertices());
        for (auto pi = P.vertices_begin(); pi != P.vertices_end(); ++pi) {
          out.push_back(conv(pi->point()));
        }
        std::cout << "Part " << num_parts << ": " << out.size() << " vertices"
                  << std::endl;
      } else {
        auto &out = parts.emplace_back();
        class Add_vertices {

          std::vector<Double_Point3> &out_;
          CGAL::Cartesian_converter<CGAL_Kernel3, Double_Kernel> conv_;

        public:
          Add_vertices(std::vector<Double_Point3> &out) : out_(out) {}

          void visit(CGAL_Nef_polyhedron3::Vertex_const_handle v) {
            out_.push_back(conv_(v->point()));
          }

          void visit(CGAL_Nef_polyhedron3::Halffacet_const_handle) {}
          void visit(CGAL_Nef_polyhedron3::SFace_const_handle) {}
          void visit(CGAL_Nef_polyhedron3::Halfedge_const_handle) {}
          void visit(CGAL_Nef_polyhedron3::SHalfedge_const_handle) {}
          void visit(CGAL_Nef_polyhedron3::SHalfloop_const_handle) {}
        };

        Add_vertices A(out);
        nef.visit_shell_objects(ci->shells_begin(), A);
        std::cout << "Part " << num_parts << ": " << out.size() << " vertices "
                  << std::endl;
      }
      num_parts++;
    } else {
      num_unmarked++;
    }
  }

  std::cout << "Number of parts: " << num_parts << std::endl;
  std::cout << "Number of unmarked parts: " << num_unmarked << std::endl;
  return parts;
}

std::vector<CGAL::Surface_mesh<Double_Point3>>
hull_parts(std::vector<std::vector<Double_Point3>> &parts) {
  std::vector<CGAL::Surface_mesh<Double_Point3>> meshes;
  for (auto &part : parts) {
    auto &mesh = meshes.emplace_back();
    CGAL::convex_hull_3(part.begin(), part.end(), mesh);
  }
  return meshes;
}
