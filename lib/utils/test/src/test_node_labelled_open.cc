#include "test/utils/all.h"
#include "utils/containers.h"
#include "utils/graph/adjacency_openmultidigraph.h"
#include "utils/graph/labelled/node_labelled_open.h"
#include "utils/graph/labelled/unordered_label.h"
#include "utils/graph/node.h"

#include <string>
#include <vector>

using namespace FlexFlow;

// this file test the graph/labelled/node_labelled_open.h
TEST_CASE("NodeLabelledOpenMultiDiGraph implementations") {
  NodeLabelledOpenMultiDiGraph g = NodeLabelledOpenMultiDiGraph<
      std::string>::create<AdjacencyOpenMultiDiGraph,
                           UnorderedLabelling<Node, std::string>>();

  int num_nodes = 3;
  std::vector<std::string> labels = repeat2(
      num_nodes,
      [&](int i) { return "labels_" + std::to_string(i); },
      std::string());

  std::vector<Node> n;
  for (int i = 0; i < num_nodes; i++) {
    n.push_back(g.add_node(labels[i]));
  }
  // here,the OpenMultiDiEdge should be MultiDiEdge, has node src/dst and
  // nodeport src_idx/dst_idx
  std::vector<NodePort> p = repeat(3, [&] { return g.add_node_port(); });
  std::vector<MultiDiEdge> e = {
      {n[1], p[1], n[0], p[0]},
      {n[2], p[2], n[0], p[0]},
      {n[0], p[0], n[2], p[2]},
      {n[1], p[1], n[2], p[2]}}; // this may have problem, we can fix

  for (int i = 0; i < num_nodes; i++) {
    CHECK(g.at(n[i]) == labels[i]);
  }

  CHECK(g.query_nodes(NodeQuery::all()) == without_order(n));

  for (MultiDiEdge const &edge : e) {
    g.add_edge(edge);
  }

  MultiDiEdgeQuery q = MultiDiEdgeQuery::all();

  OpenMultiDiEdgeQuery query{q};

  // CHECK(g.query_edges(q) == e);
  CHECK(transform(g.query_edges(q), [](OpenMultiDiEdge const &edge) {
          return get<MultiDiEdge>(edge);
        }) == without_order(e));

  // TODO: we should add more test use MultiDiEdgeQuery::with_src_nodes
}
