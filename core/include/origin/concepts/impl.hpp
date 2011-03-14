// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_IMPL_HPP
#define ORIGIN_CONCEPTS_IMPL_HPP

#include <origin/utility/meta.hpp>
#include <origin/traits.hpp>
#include <origin/logic.hpp>
#include <origin/concepts/fwd.hpp>

namespace origin
{
  /**
   * @defgroup concepts
   */

  /**
   * @defgroup unchecked_concepts
   * An unchecked concept is one that expresses semantic requirements via one
   * or more axioms. In general, axioms cannot be statically checked. Unchecked
   * are always valid. A type that models unchecked concepts should be carefuly
   * tested against the unchecked axioms.
   *
   * In contrast, explicit concepts are always false unless specialized.
   * @see explicit_concepts
   */

  /**
   * @ingroup explicit_concepts
   * An explicit concept is one that is always false. Explicit or partial
   * specializations of the concept are used to indicate that type or set of
   * types are valid models of the concept.
   */

  /**
   * @internal
   * The concept check metafunction evaluates to true if all of the given
   * predicates are true. If not, the metafunction evaluates to false.
   */
  template<typename... Args> struct concept_check;

  // Empty argument sequences are trivially true.
  template<> struct concept_check<> : std::true_type { };

  template<typename Model>
  struct concept_check<Model>
    : bool_constant<Model::value>
  { };

  template<typename Model, typename... Args>
  struct concept_check<Model, Args...>
    : std::conditional<
        Model::value, typename concept_check<Args...>::type, std::false_type
      >::type
  { };

  // A specialization for tuples, just to help out.
  template<typename... Args>
  struct concept_check<std::tuple<Args...>>
    : concept_check<Args...>::type
  { };

  /**
   * @ingroup Concepts
   * The concept map template is used as the base class for explicit concept
   * maps. This simply renders the concept true and specifies empty concepts.
   */
  template<typename Concept>
  struct Concept_Map
    : std::true_type
  { typedef std::tuple<> requirements; };

  /**
   * @internal
   * @ingroup concepts
   * The unspecified type is a placeholder used to indicate that a template
   * parameter has been left unspecified.
   */
  struct unspecified { };

  /**
   * @ingroup concepts
   * This concept implements a same-type constraint over a set of types.
   */
  template<typename... Args>
  struct Same
  {
    Same()
    { auto p = constraints; }

    static void constraints()
    { static_assert(are_same<Args...>::value, "Types are not the same"); }

    typedef std::tuple<
      are_same<Args...>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts
   * This concept implements a same-type constraint over a set of types.
   */
  template<typename T, typename U>
  struct Convertible
  {
    Convertible()
    { auto p = constraints; }

    static void constraints()
    {
      static_assert(std::is_convertible<T, U>::value,
                    "Type is not convertible");
    }

    typedef std::tuple<
      std::is_convertible<T, U>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts
   * Determine if the type T can be constructed over the given argument types.
   */
  template<typename T, typename... Args>
  struct Constructible
  {
    Constructible()
    { auto p = constraints; }

    static void constraints(Args&&... args)
    {
      T{std::forward<Args>(args)...};
    }

    typedef std::tuple<
      is_constructible<T, Args...>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts
   * The assignable concept determines whether an object of type U can be
   * assigned to an object of type T.
   */
  template<typename T, typename U>
  struct Assignable
  {
    Assignable()
    { auto p = constraints; }

    static void constraints(T& x, U y)
    { x = y; }

    typedef std::tuple<
      has_assign<T, U>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

} // namespace origin

#endif
