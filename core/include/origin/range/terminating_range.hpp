// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_TERMINATE_RANGE_HPP
#define ORIGIN_RANGE_TERMINATE_RANGE_HPP

#include <origin/range.hpp>

namespace origin
{
  // Terminating iterator
  template <typename I, typename P>
    class terminating_iterator
    {
      std::tuple<I, I, P> data;
    public:
      using value_type = Value_type<I>;
      using reference = Iterator_reference<I>;
      using pointer = Iterator_pointer<I>;
      using difference_type = Distance_type<I>;
      using iterator_category = std::input_iterator_tag;


      terminating_iterator(I first, I last, P pred)
        : data {check(first, last, pred), last, pred}
      { }

      // Equality comparable
      // Two terminating iterators are equal when their first iterators are
      // equal. They are only compared by position, not the generated range.
      bool operator==(const terminating_iterator& x) const 
      { 
        return first() == x.first(); 
      }
      
      bool operator!=(const terminating_iterator& x) const 
      { 
        return first() != x.first();
      }

      // Readable
      auto operator*() const -> Dereference_result<I> { return *first(); }

      // Incrementable
      terminating_iterator& operator++() 
      { 
        next(); return *this; 
      }

    private:
      // Advance to the next iterator
      void next()
      {
        if (first() != last())
          first() = check(o_next(first()), last(), pred());
      }

      // Return last if pred(*first) is true or first if it is not. If
      // first == last, then return last, obviously.
      static I check(I first, I last, P pred) 
      { 
        if (first != last)
          return pred(*first) ? last : first; 
        else
          return last;
      }

      // Returns the first (current) iterator
      I&       first()       { return std::get<0>(data); }
      const I& first() const { return std::get<0>(data); }

      // Returns the last iterator
      const I& last() const  { return std::get<1>(data); }
      
      // Returns the predicate function
      const P& pred() const  { return std::get<2>(data); }
    };



  // Terminating range
  // A terminate stops whenever a specified predicate is true.
  //
  // FIXME: Does the terminating function actually need to be a regular
  // function? Does it even need to take an iterator as an argument? It could
  // simply be called to observe some non-local state.
  template <typename I, typename P>
    class terminating_range
    {
      static_assert(Input_iterator<I>(), "");
      static_assert(Predicate<P, Value_type<I>>(), "");
    public:
      using value_type = Value_type<I>;
      using iterator = terminating_iterator<I, P>;
      
      terminating_range(I first, I last, P pred)
        : data {first, last, pred}
      { }
      
      // Returns the transformation function
      P terminating_pred() const { return pred(); }
      
      // Range operations
      iterator begin() const { return {first(), last(), pred()}; }
      iterator end() const   { return {last(), last(), pred()}; }
      
    private:
      const I& first() const { return std::get<0>(data); }
      const I& last() const  { return std::get<1>(data); }
      const P& pred() const  { return std::get<2>(data); }

    private:
      std::tuple<I, I, P> data;
    };
    
    
  // Terminated (constructor)
  // The terminated function constructs a range...
  //
  //    terminated(first, last, pred)
  //    terminated(first, last, value)
  //    terminated(range, pred)
  //    terminated(range, value)
  //
  // When constructed over a value (instead of a predicate), the range
  // terminates when that an element of the underlying range compares equal
  // to the given value.
  //
  // Note that the comparison can be generalized using the relational 
  // constructors in the functional library (e.g., lt(value) constructs a
  // viable predicate that will compare x < value for each element x in the
  // underlying range.

  template <typename I, typename P>
    inline auto terminated(I first, I last, P pred)
      -> Requires<Predicate<P, Value_type<I>>(), terminating_range<I, P>>
    {
      return {first, last, pred};
    }
    
  template <typename I, typename T>
    inline auto terminated(I first, I last, const T& value)
      -> Requires<Equality_comparable<Value_type<I>, T>(), 
                  terminating_range<I, Equal_to_value<T>>>
    {
      return {first, last, eq(value)};
    }
    
  template <typename R, typename P>
    inline auto terminated(R&& range, P pred)
      -> Requires<
           Predicate<P, Value_type<Forwarded<R>>>(), 
           terminating_range<Iterator_type<Remove_reference<R>>, P>>
    {
      return {o_begin(range), o_end(range), pred};
    }
    

  template <typename R, typename T>
    inline auto terminated(R&& range, const T& value)
      -> Requires<
           Equality_comparable<Value_type<Forwarded<R>>, T>(), 
           terminating_range<Iterator_type<Remove_reference<R>>, Equal_to_value<T>>>
    {
      return {o_begin(range), o_end(range), eq(value)};
    }
   

} // namespace origin

#endif
