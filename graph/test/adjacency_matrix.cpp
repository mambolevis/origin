#include <iostream>
#include <cassert>

#include <origin/graph/adjacency_matrix.hpp>

typedef origin::directed_adjacency_matrix<int, float> g_type;
const g_type::size_type g_size = 5;

g_type r_copy(g_type g) { return g; }

int main()
{
  // Testing: 
  //  directed_adjacency_matrix(allocator_type const& alloc = allocator_type{});
  //  
  //  directed_adjacency_matrix::null()
  //  directed_adjacency_matrix::empty()
  //  directed_adjacency_matrix::order()
  //  directed_adjacency_matrix::size()
  g_type null_g;

  assert(null_g.null());
  assert(null_g.empty());
  assert(null_g.order() == 0u);
  assert(null_g.size() == 0u);

  g_type g(g_size);

  assert(!g.null());
  assert(!g.empty());
  assert(g.order() == g_size);
  assert(g.size() == g_size * g_size);

  // Testing: Add edges
  g.add_edge(g_type::vertex(1), g_type::vertex(2));

  // Testing additional constructors.

  // Testing ranges.

  {
    auto edges = g.edges();
    /*for(; edges.begin() != edges.end(); ++edges.begin())
    { }*/
  }

  return 0;
}

