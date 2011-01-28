// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ADJACENCY_MATRIX_DIRECTED_HPP
#define ORIGIN_GRAPH_ADJACENCY_MATRIX_DIRECTED_HPP

#include <origin/graph/adjacency_matrix/impl.hpp>

#include <vector>

#include <origin/utility/empty.hpp>
#include <origin/range/iterator_range.hpp>
#include <origin/optional.hpp>

namespace origin
{

  template<typename Vertex_T = empty,
           typename Edge_T = empty,
           typename Alloc = std::allocator<Vertex_T>>
  class directed_adjacency_matrix
  {
    typedef directed_adjacency_matrix<Vertex_T, Edge_T, Alloc> this_type;
    typedef Vertex_T vertex_type;
    typedef Edge_T edge_type;
  public:
    typedef Vertex_T vertex_value_type;
    typedef optional<Edge_T> edge_value_type;
    typedef Alloc allocator_type;
    typedef std::vector<vertex_value_type, Alloc> vertex_list;
    typedef std::vector<edge_value_type, Alloc> edge_matrix;

    typedef typename vertex_list::size_type size_type;
    typedef typename vertex_list::difference_type difference_type;

    // Vertex and Edge data types
    typedef adj_mtx_impl_::handles::vertex_t vertex;
    typedef const adj_mtx_impl_::handles::vertex_t const_vertex;
    typedef adj_mtx_impl_::handles::edge_t edge;
    typedef const adj_mtx_impl_::handles::edge_t const_edge;

    // Iterator types
    typedef typename vertex_list::iterator vertex_iterator;
    typedef typename vertex_list::const_iterator const_vertex_iterator;
    typedef int edge_iterator;
    typedef const int const_edge_iterator;
    typedef float in_edge_iterator;
    typedef const float const_in_edge_iterator;
    typedef edge_iterator out_edge_iterator;
    typedef const_edge_iterator const_out_edge_iterator;
    //typedef int adjacent_edge_iterator;

    // Range types
    typedef iterator_range<vertex_iterator> vertex_range;
    typedef iterator_range<const_vertex_iterator> const_vertex_range;
    typedef iterator_range<edge_iterator> edge_range;
    typedef iterator_range<const_edge_iterator> const_edge_range;
    typedef iterator_range<in_edge_iterator> in_edge_range;
    typedef iterator_range<const_in_edge_iterator> const_in_edge_range;
    typedef iterator_range<out_edge_iterator> out_edge_range;
    typedef iterator_range<const_out_edge_iterator> const_out_edge_range;

    /** @name Construction, Assignment and Destruction */
    //@{
    // Construction and destruction.
    directed_adjacency_matrix(allocator_type const& alloc = allocator_type{});
    directed_adjacency_matrix(size_type n,
                              allocator_type const& alloc = allocator_type{});

    // Copy semantics
    directed_adjacency_matrix(directed_adjacency_matrix const&);
    directed_adjacency_matrix& operator=(directed_adjacency_matrix const&);

    // Move Semantics
    directed_adjacency_matrix(directed_adjacency_matrix&&);
    directed_adjacency_matrix& operator=(directed_adjacency_matrix&&);
    //@}

    /** @name Data Structure Properties */
    //@{
    allocator_type get_allocator() const;
    constexpr size_type max_order() const;
    constexpr size_type max_size() const;
    //@}

    /** @name Graph Properties */
    //@{
    bool null() const;
    size_type order() const;
    bool empty() const;
    size_type size() const;
    //@}

    /** Data Accessors */
    //@{
    vertex_value_type& operator[](vertex v);
    vertex_value_type const& operator[](const_vertex v) const;
    edge_value_type& operator[](edge e);
    edge_value_type const& operator[](const_edge e) const;
    //@}

    /** @name Data Structure Operations */
    //@{
    void swap(directed_adjacency_matrix&);
    void clear();   // Clear edges? Vertices are fixed.
    //@}

    /** @name Vertex Properties and Operations */
    //@{
    vertex add_vertex(vertex_value_type);
    void remove_vertex(vertex v);
    size_type out_degree(const_vertex v) const;
    size_type in_degree(const_vertex v) const;
    size_type degree(const_vertex v) const;
    //@}

    /** @name Edge Properties and Operations */
    //@{
    edge add_edge(vertex u, vertex v);
    void remove_edge(edge e);
    void remove_edges(vertex u, vertex v);
    void remove_edges()
    edge get_edge(vertex u, vertex v);
    const_edge get_edge(const_vertex u, const_vertex v) const;
    vertex source(edge e);
    const_vertex source(const_edge e) const;
    vertex target(edge e);
    const_vertex target(edge e);
    //@}

    /** @name Ranges */
    //@{
    vertex_range vertices();
    const_vertex_range vertices() const;
    edge_range edges();
    const_vertex_range edges() const;
    out_edge_range out_edges(vertex_iterator);
    const_out_edge_range out_edges(const_vertex_iterator) const;

    in_edge_range in_edges(vertex_iterator);
    const_in_edge_range in_edges(const_vertex_iterator) const;
    //@}

  private:
    vertex_list vertices_;
    edge_matrix edges_;
  };

  /** @Internal Member Definitions */
  //@{
  template<typename V, typename E, typename A>
  directed_adjacency_matrix::directed_adjacency_matrix
  (allocator_type const& alloc = allocator_type{})
  { }

  template<typename V, typename E, typename A>
  directed_adjacency_matrix::directed_adjacency_matrix
  (size_type n, allocator_type const& alloc = allocator_type{})
  { }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix::get_allocator() const -> allocator
  { return allocator_type{}; }

  // Since the order of the graph is fixed, return size/capacity of verticies.
  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix::max_order() const -> size_type
  { return vertices_.capacity(); }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix::max_size() const -> size_type
  { return edges_.max_size(); }

  template<typename V, typename E, typename A>
  bool directed_adjacency_matrix::null() const
  { return vertices_.empty(); }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix::order() const -> size_type
  { return vertices_.size(); }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix::empty() const -> size_type
  { return edges_.empty(); }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix::size() const -> size_type
  { return edges_.size(); }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix::operator[](vertex v) -> vertex_value_type&
  { }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix::const& operator[](const_vertex v) const
    -> vertex_value_type const&
  { }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix::operator[](edge e) -> edge_value_type&
  { }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix::const& operator[](const_edge e) const
    -> edge_value_type const&
  { }
  //@}

} // namesapce origin

#endif
