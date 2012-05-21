// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

// FIXMEs
//  - add vertex and edge map types
//  - create a make_undirected_adjacency_vector function
//  - add_edges convienience function
//  - remove STL algorithm dependency (use origin)
//  - put in concept support
//  - make iterators random access
//  - add utility operations like degree

#ifndef ORIGIN_GRAPH_ADJACECNY_VECTOR_UNDIRECTED_HPP
#define ORIGIN_GRAPH_ADJACECNY_VECTOR_UNDIRECTED_HPP

#include <tuple>
#include <vector>
#include <algorithm>

#include <origin/graph/vertex.hpp>
#include <origin/graph/edge.hpp>
#include <origin/iterator.hpp>

//#include <origin/graph/adjacency_vector/impl.hpp>

namespace origin {

/*============================================================================*/
  // Common uav definitions

  namespace {
  // This is where vertex type and edge type should go.
  //  using edge_vec = std::vector

  } // namespace

  namespace uav {

  } // namespace uav



/*============================================================================*/
  // Iterators

  namespace uav {

    // Undirected adjacency vector edge iterator
    class edge_iterator {
      using size_type     = std::size_t;
      using internal_edge = std::tuple<size_type,size_type>;
      using edge_vec_iter = std::vector<internal_edge>::const_iterator;
      using edge          = undirected_edge_handle<size_type>;
    public:
      using value_type        = edge;
      using reference         = edge;
      using pointer           = edge;
      using difference_type   = std::ptrdiff_t;
      using iterator_category = std::bidirectional_iterator_tag;

      // Initializers
      edge_iterator(edge_vec_iter first, size_type n = 0)
        : first_(first), i_(n)
      { }

      edge_iterator(edge_iterator const& ei)
        : first_(ei.first_), i_(ei.i_)
      { }

      edge_iterator(edge_iterator && ei)
        : first_(ei.first_), i_(ei.i_)
      { }

      edge_iterator& operator= (edge_iterator ei)
      { first_ = ei.first_; i_ = ei.i_; return *this; }

      // Readable
      reference operator* () const { return make_edge(); }
      //pointer operator-> () const;

      // Equality comparable
      bool operator== (edge_iterator ei) const { return i_ == ei.i_; }
      bool operator!= (edge_iterator ei) const { return i_ != ei.i_; }

      // Totally ordered
      bool operator<(edge_iterator ei) const { return i_ < ei.i_; }
      bool operator>(edge_iterator ei) const { return i_ > ei.i_; }
      bool operator<=(edge_iterator ei) const { return i_ <= ei.i_; }
      bool operator>=(edge_iterator ei) const { return i_ >= ei.i_; }

      // Incrementable
      edge_iterator& operator++ () { ++i_; return *this; }
      edge_iterator operator++ (int) {
        edge_iterator t(*this);
        ++i_;
        return t;
      }

      // Decrementable
      edge_iterator& operator-- () { --i_; return *this; }
      edge_iterator operator-- (int) {
        edge_iterator t(*this);
        --i_;
        return t;
      }

    private:
      edge_vec_iter first_;
      size_type i_;

      // Helpers
      // wraps the iterator position into an edge
      edge make_edge() const
      { return edge(i_, std::get<0>(*first_), std::get<1>(*first_)); }
    };

  } // namespace uav



/*============================================================================*/
  // Local Helper functions
  namespace {

    // Predicate to find a vertex with an edge
    struct incident_to_edge
    {
      incident_to_edge(std::size_t n)
        : edge_index(n)
      { }

      bool operator() (std::tuple<std::size_t,std::size_t> v)
      { return std::get<1>(v) == edge_index; }

      std::size_t edge_index;
    };
  } // namespace detail




/*============================================================================*/
  // undirected adjacency vector implementation
  //template<typename V, typename E>
    class undirected_adjacency_vector
    {
      using size_type       = std::size_t;
      using internal_vertex = std::tuple<size_type,size_type>; // (target,edge)
      using internal_edge   = std::tuple<size_type,size_type>; // (src,target)
      using adj_vec         = std::vector<internal_vertex>;
      using vertex_vec      = std::vector<adj_vec>;
      using edge_vec        = std::vector<internal_edge>;

    public:
      // Types
      using vertex = vertex_handle<size_type>;
      using edge   = undirected_edge_handle<size_type>;

      using vertex_range        = iterator_range<vertex_iterator<size_type>>;
      using edge_range          = iterator_range<uav::edge_iterator>;
      using incident_edge_range = iterator_range<uav::edge_iterator>;


      // Initializers
      undirected_adjacency_vector (size_type n = 0) : neighbors_(n), edges_() {}


      // Graph Metrics
      bool empty()      const { return edges_.empty(); }
      size_type size()  const { return edges_.size(); }
      bool null()       const { return neighbors_.empty(); }
      size_type order() const { return neighbors_.size(); }


      // Structural Mutators
      vertex add_vertex();
      edge add_edge(vertex u, vertex v);
      void remove_edge(edge e);
      //void remove_edges(vertex u, vertex u);


      // Ranges
      vertex_range vertices() const;
      edge_range edges() const;
      incident_edge_range incident_edges(vertex v) const;

    private:
      vertex_vec neighbors_;
      edge_vec edges_;

      // Helper functions
      // Removes a vertex adjacent to vertex v incedent to edge e
      void remove_adjacent_vertex_by_edge(vertex v, edge e);
    };



  auto undirected_adjacency_vector::add_vertex() -> vertex
  {
    neighbors_.push_back(adj_vec());
    return vertex(order() - 1);
  }

  auto undirected_adjacency_vector::add_edge(vertex u, vertex v) -> edge
  {
    // add an edge to the edge list
    edges_.push_back(std::make_tuple(u.value(), v.value()));
    // add v to u's adjacent stuff
    neighbors_[u.value()].push_back(std::make_tuple(v.value(), size() - 1));
    // add u to v's adjacent stuff, unless it is a loop
    if (v.value() != u.value())
      neighbors_[v.value()].push_back(std::make_tuple(u.value(), size() - 1));

    // return edge
    return edge(size() - 1, u.value(), v.value());
  }

  void undirected_adjacency_vector::remove_edge(edge e)
  {
    // remove from edges
    edges_.erase(edges_.begin() + e.edge.value());
    // remove adjacent stuff
    remove_adjacent_vertex_by_edge(e.source, e);
    if (e.source.value() != e.target.value()) // if e is not a loop
      remove_adjacent_vertex_by_edge(e.target, e);
  }

  void
  undirected_adjacency_vector::remove_adjacent_vertex_by_edge(vertex v, edge e)
  {
    // find vertex
    size_type e_index = e.edge.value();
    auto pos = std::find_if(
      neighbors_[v.value()].begin(),
      neighbors_[v.value()].end(),
      incident_to_edge(e_index));
    // remove edge
    neighbors_[v.value()].erase(pos);
  }

  auto undirected_adjacency_vector::vertices() const -> vertex_range
  {
    return vertex_range(
      vertex_iterator<size_type>(0),
      vertex_iterator<size_type>(order())
    );
  }

  auto undirected_adjacency_vector::edges() const -> edge_range
  {
    return edge_range(
      uav::edge_iterator(edges_.begin(), 0),
      uav::edge_iterator(edges_.begin(), size())
    );
  }

  auto undirected_adjacency_vector::incident_edges(vertex v) const
    -> incident_edge_range
  {
    return incident_edge_range(
      uav::edge_iterator(
        neighbors_[v.value()].begin(), 0),
      uav::edge_iterator(
        neighbors_[v.value()].begin(), neighbors_[v.value()].size())
    );
  }




/*============================================================================*/

} // namespace origin

#endif // ORIGIN_GRAPH_ADJACECNY_VECTOR_UNDIRECTED_HPP