// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <origin/graph/concepts.hpp>

namespace origin
{
  // ------------------------------------------------------------------------ //
  //                                                             [graph.generic]
  //                          Generic Graph Interface
  //
  // The generic graph interface is a set of types and operations defined
  // commonly for various graph data structures.

  template<typename G>
    using Vertex = typename G::vertex;

  template<typename G>
    using Edge = typename G::edge;


  // Retursn the source vertex of an edge in g.
  template<typename G>
    inline Vertex<G>
    source(const G& g, Edge<G> e) { return g.source(e); }

  // Returns the target vertex of an edge in g.
  template<typename G>
    inline Vertex<G>
    target(const G& g, Edge<G> e) { return g.target(e); }



  // ------------------------------------------------------------------------ //
  //                                                                [graph.pred]
  //                          Common Graph Predicates
  //
  // The following function objects are useful in a number of graph operations.
  //
  //    has_target<G>
  //    has_source<G>
  //

  // Returns true when an edge is is the same as some target vertex.
  template<typename G>
    struct has_target
    {
      has_target(const G& g, Vertex<G> v)
        : g(g), v(v)
      { }

      inline bool
      operator()(Edge<G> e) const { return target(g, e) == v; }

      const G&  g;
      Vertex<G> v;
    };

  // Returns true when an edge is is the same as some source vertex.
  template<typename G>
    struct has_source
    {
      has_source(const G& g, Vertex<G> v)
        : g(g), v(v)
      { }

      inline bool
      operator()(Edge<G> e) const { return source(g, e) == v; }

      const G&  g;
      Vertex<G> v;
    };


  // Returns true when an edge is is the same as some source vertex.
  template<typename G>
    struct has_ends
    {
      has_ends(const G& g, Vertex<G> u, Vertex<G> v)
        : g(g), u(u), v(v)
      { }

      inline bool
      operator()(Edge<G> e) const 
      {
        Vertex<G> a = source(g, e);
        Vertex<G> b = target(g, e);
        if (Directed_graph<G>())
          return (u == a && v == b);
        else
          return (u == a && v == b) || (u == b && v == a);
      }

      const G&  g;
      Vertex<G> u;
      Vertex<G> v;
    };


} // namespace origin


#endif
