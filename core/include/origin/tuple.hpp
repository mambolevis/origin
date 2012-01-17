// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TUPLE_HPP
#define ORIGIN_TUPLE_HPP

#include <tuple>

#include <origin/concepts.hpp>

namespace origin
{
  template<typename T, std::size_t N>
    using Tuple_element = typename std::tuple_element<N, T>::type;
  
  
  // Tuple for each
  // The tuple_for_each operation applies a Function f to each element of the
  // given tuple.
  
  // Recursively apply F to the ith (and increasing) element of the the
  // given tuple.
  template<std::size_t I, std::size_t N, typename F>
    struct do_tuple_for_each
    {
      template<typename T>
        static void apply(F f, T& x)
        {
          static_assert(Function<F, Tuple_element<T, I>&>(), "");
          f(std::get<I>(x));
          do_tuple_for_each<I + 1, N, F>::apply(f, x);
        }
    };

  // Base case for the for_each.
  template<std::size_t N, typename F>
    struct do_tuple_for_each<N, N, F>
    {
      template<typename T>
        static void apply(F f, T& x) { }
    };

  // Apply the function f to each element of the given tuple. The order of
  // applicaiton is from left to right (i.e,. [0, sizeof...(Args]) rather
  // than in reverse.
  template<typename F, typename... Args>
    inline void tuple_for_each(std::tuple<Args...>& x, F f)
    { 
      do_tuple_for_each<0, sizeof...(Args), F>::apply(f, x); 
    }

  // A const version of the function above.
  template<typename F, typename... Args>
    inline void tuple_for_each(std::tuple<Args...> const& x, F f)
    { 
      do_tuple_for_each<0, sizeof...(Args), F>::apply(f, x); 
    }

    
  
  // Tuple invoke
  // This framework expands a tuple as arguments to a given function.

  // The expander template is responsible for recursively constructing the
  // sequence of arguments that will ultimately be passed to the function
  // call. 
  //
  // Note that the actual result type is pushed down into the function. This 
  // prevents us from having to reconstruct it from the arguments in every 
  // recursion.
  template<size_t I, size_t N, typename Result>
  struct tuple_expand_args
  {
    template<typename F, typename T, typename... Args>
      static Result apply(F f, T& x, Args&&... args)
      {
        return tuple_expand_args<I + 1, N, Result>::apply(
          f, x, std::forward<Args>(args)..., std::get<I>(x)
        );
    }

    template<typename F, typename T, typename... Args>
      static Result apply(F f, T const& x, Args&&... args)
      {
        return tuple_expand_args<I + 1, N, Result>::apply(
          f, x, std::forward<Args>(args)..., std::get<I>(x)
        );
      }
  };

  // Base case. This ultimately calls the target function.
  template<size_t N, typename Result>
  struct tuple_expand_args<N, N, Result>
  {
    template<typename F, typename T, typename... Args>
      static Result apply(F f, T& x, Args&&... args)
      { 
        return f(std::forward<Args>(args)...); 
      };

    template<typename F, typename T, typename... Args>
      static Result apply(F f, T const& x, Args&&... args)
      { 
        return f(std::forward<Args>(args)...); 
      };
  };

  // The tuple invoke function invokes the polymorphic forwarding function f
  // over the arguments expanded from the given tuple. A "polymorphic
  // forwarding function object" is a function object that defines a variadic
  // template function call operator that forwards the expanded arguments to
  // the target function call.

  template<typename F, typename... Args>
  inline Result_type<F, Args...> tuple_invoke(F f, std::tuple<Args...>& x)
  {
    using Result = Result_type<F, Args...>;
    return tuple_expand_args<0, sizeof...(Args), Result>::apply(f, x);
  }
  
  // A const version of the function above.
  template<typename F, typename... Args>
  inline Result_type<F, Args...> tuple_invoke(F f, std::tuple<Args...> const& x)
  {
    using Result = Result_type<F, Args...>;
    return tuple_expand_args<0, sizeof...(Args), Result>::apply(f, x);
  }

} // namespace origin

#endif
