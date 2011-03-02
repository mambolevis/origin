// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_LABEL_HPP
#define ORIGIN_GRAPH_LABEL_HPP

/**
 * @defgroup graph_label
 * Types and traits related to the definition and use of labels. A label is
 * a function maps vertices and edges to a value type, supporting both reading
 * and writing.
 */

namespace origin
{

  /**
   * @internal
   * A placeholder type that is used to indicate that the use of an internal
   * label.
   */
  template<typename Graph, typename Value>
  struct internal_label { };
  
  /**
   * The vertex label class wraps a label and provides a (empty) constructor 
   * over a graph. This is primarily used to support interoperability with
   * builtin labels (see the specialization below).
   */
  template<typename Graph, typename Label>
  struct vertex_label
  {
    // FIXME: This should be the same as decay<T>, I think. Basically, we're
    // just getting the value type with cv-qualifiers removed. Also note that
    // we're explicitly generating the result type over the non-const vertex
    // type. The const and non-const result types should only vary by a const
    // and maybe a reference, both of which are removed here.
    typedef typename std::remove_reference<
      typename std::remove_const<
        typename std::result_of<Label(typename Graph::vertex)>::type
      >::type
    >::type value_type;
  
    vertex_label(Label l)
      : label(l)
    { }
    
    // FIXME: Try to use decltype. May not be easy since I can't access the
    // label as a member
    template<typename Vertex>
    typename std::result_of<Label(Vertex)>::type 
    operator()(Vertex v) const
    { return label(v); }
    
    Label label;
  };
  
  // Specialization over internal labels. This actually owns the data that
  // the label references.
  template<typename Graph, typename Value>
  struct vertex_label<Graph, internal_label<Graph, Value>>
  {
    typedef typename vertex_type<Graph>::type vertex;
    typedef Value value_type;
    typedef Value& reference;
    typedef Value const& const_reference;
    
    vertex_label(Graph const& g)
      : label(g.order())
    { }
    
    reference operator()(vertex v)
    { return label[v]; }
    
    const_reference operator()(vertex v) const
    { return label[v]; }
    
    // FIXME: Select an optional mapping type based on the graph kind. 
    std::unordered_map<vertex, value_type> label;
  };


} // namespace origin

#endif
