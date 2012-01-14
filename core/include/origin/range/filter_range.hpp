// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_FILTER_RANGE_HPP
#define ORIGIN_RANGE_FILTER_RANGE_HPP

#include <origin/iterator/filter_iterator.hpp>
#include <origin/range/traits.hpp>

namespace origin
{

  // A filter range is a bounded range adaptor that describes a subset of
  // elements satisfying a given predicate. In other words, iterating over a
  // filter range visits all elements x in r such that r.predicate()(x) is
  // true.
  //
  // Note that R may be const. 
  template<typename R, typename Pred>
    class filter_range
    {
    public:
      using value_type = Value_type<R>;
      using iterator = filter_iterator<Iterator_type<R>, Pred>;
      
      // Construct a filter range over the underlying range. The predicate
      // may be omitted if Pred is Default_constructible.
      filter_range(R& r, Pred pred = Pred{})
        : data{r, pred}
      { }

      // Returns the underlying range.
      R const& base() const { return range(); }
      
      // Returns the predicate function.
      Pred predicate() const { return pred(); }
      
      // Begin and end.
      iterator begin() const { return {std::begin(range()), std::end(range()), pred()}; }
      iterator end() const   { return {std::end(range()), pred()}; }
      
    private:
      R&       range()       { return std::get<0>(data); }
      const R& range() const { return std::get<0>(data); }
      
      const Pred& pred() const { return std::get<1>(data); }
    
    private:
      std::tuple<R&, Pred> data;
    };
    
  // Return an adapted filter over the given range.
  template<typename R, typename Pred>
    inline filter_range<R, Pred> filter(R& range, Pred pred)
    {
      return {range, pred};
    }

  // A constant version of the function above.
  template<typename R, typename Pred>
    inline filter_range<R const, Pred> filter(R const& range, Pred pred)
    {
      return {range, pred};
    }

} // namespace origin

#endif
