// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RELATION_HPP
#define ORIGIN_RELATION_HPP

#include <functional>

namespace origin
{
  template<typename R, typename T, typename U> struct cRelation;
  
  // FIXME: Optimize with EBO?
  /**
   * The incomparable operation is defined in terms of a strict ordering
   * comparison on T, less by default. The operation returns true if, for
   * objects x and y, it is neither the case that x < y, nor y < x. 
   * 
   * Note that if the  template parameter T is a total order, then this is 
   * equivalent to the equal_to function.
   *
   * @tparam T      A type
   * @tparam Comp   A Strict Partial Order on T
   */
  template<typename T, typename Comp>
  class incomparable
    : cRelation<Comp, T, T>
  {
    // aStrict_Partial_Order<Comp, T>
  public:
    incomparable(Comp c)
      : comp{c}
    { }
    
    bool operator()(T const& x, T const& y) const
    {
      return !comp(x, y) && !comp(y, x);
    }
    
  private:
    Comp comp;
  };

} // namespace origin

#include <origin/concepts/relation.hpp>

#endif
