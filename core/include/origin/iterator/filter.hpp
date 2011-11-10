// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_FILTER_HPP
#define ORIGIN_ITERATOR_FILTER_HPP

#include <iterator>

#include <origin/algorithm.hpp>
#include <origin/iterator/traits.hpp>

namespace origin
{
  // A filter iterator is an iterator adaptor that, when incremented, will
  // move to the next element satisfying the given predicate. Filter iterators 
  // are Forward_iterators.
  //
  // requires Input_iterator<Iter>
  // requires Predicate<Pred, ValueType<Iter>>
  //
  // invariant: this->first == this->last || pred(*this->first) == true
  //
  // TODO: Use EBO to eliminate the overhead of the predicate.
  template<typename Iter, typename Pred>
    class filter_iterator
    {
    public:
      typedef typename std::iterator_traits<Iter>::value_type value_type;
      typedef typename std::iterator_traits<Iter>::reference reference;
      typedef typename std::iterator_traits<Iter>::pointer pointer;
      typedef typename std::iterator_traits<Iter>::difference_type difference_type;
      typedef typename clamp_iterator_category<
        typename std::iterator_traits<Iter>::iterator_category, 
        std::forward_iterator_tag
      >::type iterator_category;
    
      // Initialize the filter iterator as its limit. The predicate may be 
      // omitted if Pred is Default_constructible.
      //
      // postcondition: this->base() == last.
      filter_iterator(Iter last, Pred pred = Pred{})
        : first{last}, last{last}, pred{pred}
      { }
    
      // Initialize the iterator. Note that this->base() may not be equal to
      // first after initialization.
      //
      // postcondition: this->pred() == pred
      // postcondition: this->pred(**this) == true
      filter_iterator(Iter first, Iter last, Pred pred)
        : first{first_if(first, last, pred)}, last{last}, pred{pred}
      { }
      
      // TODO: Do we need conversion constructors for const-interoperability?
      
      // Equality_comparable
      // Two filter iterators are equal if they refer to the same element.
      // The limit and predicate are not compared.
      bool operator==(filter_iterator const& x) const { return first != x.first; }
      bool operator!=(filter_iterator const& x) const { return first != x.first; }
      
      // Readable
      reference operator*() const { return *first; }
      pointer  operator->() const { return &*first; }
      
      // Incrementable
      filter_iterator& operator++() 
      { 
        first = next_if(first, last, pred); 
        return *this;
      }
      
      filter_iterator  operator++(int) 
      { 
        filter_iterator tmp{*this}; 
        operator++(); 
        return tmp; 
      }
      
      // Iterator properties
      Iter base() const { return first; }
      Pred predicate() const { return pred; }

    private:
      Iter first;
      Iter last;
      Pred pred;
    };
    
  // The filter functions need to be enabled to prevent name overload
  // ambiguities with other filter constructors.
    
  // Return an adapted filter iterator.
  template<typename Iter, typename Pred>
    inline typename std::enable_if<
      is_iterator<Iter>::value, filter_iterator<Iter, Pred>
    >::type
    filter(Iter first, Iter last, Pred pred)
    {
      return {first, last, pred};
    }
    
  // Return an adapted filter iterator that is initialized to its limit (i.e.,
  // past the end).
  template<typename Iter, typename Pred>
    inline typename std::enable_if<
      is_iterator<Iter>::value, filter_iterator<Iter, Pred>
    >::type
    filter(Iter last, Pred pred)
    {
      return {last, pred};
    }

} // namespace origin

#endif
