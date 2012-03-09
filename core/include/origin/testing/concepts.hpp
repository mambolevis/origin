// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

namespace origin
{
  // Equality comparable (specification)
  // The equality comparable test can be used to evaluate the semantics of the
  // equality comparison relation. That is, the == operator must satisfy
  // the requirements of an equivalence relation, and the != operator must
  // be the negation of the equality operator.
  //
  // In actuality, the == operator must compare for value equality, but we have
  // no way to actually test that.
  template <typename T, typename U = T>
    struct equality_comparable_semantics
    {
      static_assert(Equality_comparable<T, U>(), "");
      using C = Common_type<T, U>;

      // Randomly test the specification using values of type T varerated from
      // var1 and values of type U varerated from var2.
      template <typename Env, typename Var1, typename Var2>
        void operator()(Env& env, Var1&& var1, Var2&& var2) const
        {
          static_assert(Same<T, Result_type<Forwarded<Var1>>>(), "");
          static_assert(Same<U, Result_type<Forwarded<Var2>>>(), "");
          auto var3 = make_random<C>(var1.engine());
          (*this)(env, var1, var2, var3);
        }

        
      // Randomly test the specification using values of type T varerated
      // from var1, values of type U varerated from var2, and values of type
      // C (the common type) varerated from var3.
      template <typename Env, typename Var1, typename Var2, typename Var3>
        void operator()(Env& env, Var1&& var1, Var2&& var2, Var3&& var3) const
        {
          static_assert(Same<T, Result_type<Forwarded<Var1>>>(), "");
          static_assert(Same<U, Result_type<Forwarded<Var2>>>(), "");
          static_assert(Same<C, Result_type<Forwarded<Var3>>>(), "");

          check(env, equality_comparable_semantics<T>{}, var1);
          check(env, equality_comparable_semantics<U>{}, var2);
          check(env, equality_comparable_semantics<C>{}, var3);

          check(env, eq, var1, var2);  // a == b <=> C(a) == C(b)
          check(env, neq, var1, var2); // a != b <=> C(a) != C(b)
        }

      common_type_equivalence<origin::eq> eq;
      common_type_equivalence<origin::neq> neq;
    };
    
  // Specialization when testing for a single type.
  template <typename T>
    struct equality_comparable_semantics<T, T>
    {
      static_assert(Equality_comparable<T>(), "");
      
      template <typename Env, typename Var>
        void operator()(Env& env, Var&& var) const
        {
          static_assert(Same<T, Result_type<Forwarded<Var>>>(), "");
          check(env, equivalence, var);
          check(env, not_equal, var, var);
        }

      equivalence_relation_spec<eq> equivalence;
      logical_equivalence<neq, complement<eq>> not_equal;
    };



  // Weakly ordered semantics (specification)
  // The weak ordering specification defines the semantics for weakly ordered
  // types.
  template <typename T, typename U = T>
    struct weakly_ordered_semantics
    {
      static_assert(Weakly_ordered<T, U>(), "");
      using C = Common_type<T, U>;

      template <typename Env, typename Var1, typename Var2>
        void operator()(Env& env, Var1&& var1, Var2&& var2) const
        {
          static_assert(Same<T, Result_type<Forwarded<Var1>>>(), "");
          static_assert(Same<U, Result_type<Forwarded<Var2>>>(), "");
          auto var3 = make_random<C>(var1.engine());
          (*this)(env, var1, var2, var3);
        }
      
      template <typename Env, typename Var1, typename Var2, typename Var3>
        void operator()(Env& env, Var1&& var1, Var2&& var2, Var3&& var3) const
        {
          static_assert(Same<T, Result_type<Forwarded<Var1>>>(), "");
          static_assert(Same<U, Result_type<Forwarded<Var2>>>(), "");
          static_assert(Same<C, Result_type<Forwarded<Var3>>>(), "");
          
          check(env, weakly_ordered_semantics<T>{}, var1);
          check(env, weakly_ordered_semantics<U>{}, var2);
          check(env, weakly_ordered_semantics<C>{}, var3);
          
          check(env, lt, var1, var2);  // a < b <=> C(a) < C(b)
          check(env, gt, var1, var2);  // a > b <=> C(a) > C(b)
          check(env, leq, var1, var2); // a <= b <=> C(a) <= C(b)
          check(env, geq, var1, var2); // b >= a <=> C(a) >= C(b)
        }
        
      common_type_equivalence<origin::lt> lt;
      common_type_equivalence<origin::gt> gt;
      common_type_equivalence<origin::leq> leq;
      common_type_equivalence<origin::geq> geq;
    };
    
  // Specialization for the unary type.
  template <typename T>
    struct weakly_ordered_semantics<T, T>
    {
      static_assert(Weakly_ordered<T>(), "");

      template <typename Env, typename Var>
        void operator()(Env& env, Var&& var) const
        {
          static_assert(Same<T, Result_type<Forwarded<Var>>>(), "");
          check(env, lt, var);
          check(env, gt, var, var);
          check(env, leq, var, var);
          check(env, geq, var, var);
        }
        
      strict_weak_order_spec<origin::lt> lt;
      logical_equivalence<origin::gt, converse<origin::lt>> gt;
      logical_equivalence<origin::leq, complement_of_converse<origin::lt>> leq;
      logical_equivalence<origin::geq, complement<origin::lt>> geq;
    };



  // Totally ordered (specification)
  // The total ordering specification defines the semantics for totally ordered
  // types. When used with heterovareous types, the semantics are defined in
  // terms of the common type.
  template <typename T, typename U = T>
    struct totally_ordered_semantics
    {
      static_assert(Totally_ordered<T, U>(), "");
      using C = Common_type<T, U>;

      template <typename Env, typename Var1, typename Var2>
        void operator()(Env& env, Var1&& var1, Var2&& var2) const
        {
          static_assert(Same<T, Result_type<Forwarded<Var1>>>(), "");
          static_assert(Same<U, Result_type<Forwarded<Var2>>>(), "");
          auto var3 = make_random<C>(var1.engine());
          (*this)(env, var1, var2, var3);
        }
      
      template <typename Env, typename Var1, typename Var2, typename Var3>
        void operator()(Env& env, Var1&& var1, Var2&& var2, Var3&& var3) const
        {
          static_assert(Same<T, Result_type<Forwarded<Var1>>>(), "");
          static_assert(Same<U, Result_type<Forwarded<Var2>>>(), "");
          static_assert(Same<C, Result_type<Forwarded<Var3>>>(), "");
          
          check(env, totally_ordered_semantics<T>{}, var1);
          check(env, totally_ordered_semantics<U>{}, var2);
          check(env, totally_ordered_semantics<C>{}, var3);

          check(env, weak, var1, var2, var3);
          check(env, equal, var1, var2);
        }
        
      weakly_ordered_semantics<T, U> weak;
      logical_equivalence<eq, symmetric_complement<lt>> equal;
    };
    
  // Specialization for the unary type.
  template <typename T>
    struct totally_ordered_semantics<T, T>
    {
      static_assert(Totally_ordered<T>(), "");

      template <typename Env, typename Var>
        void operator()(Env& env, Var&& var) const
        {
          static_assert(Same<T, Result_type<Forwarded<Var>>>(), "");
          check(env, lt, var);
          check(env, gt, var, var);
          check(env, leq, var, var);
          check(env, geq, var, var);
        }
        
      strict_total_order_spec<origin::lt> lt;
      logical_equivalence<origin::gt, converse<origin::lt>> gt;
      logical_equivalence<origin::leq, complement_of_converse<origin::lt>> leq;
      logical_equivalence<origin::geq, complement<origin::lt>> geq;
    };
    
    

  // Move semantics (specification)
  //
  // TODO: Implement tests for move semantics.
  template <typename T>
    struct move_semantics
    {
      template <typename Env, typename Var>
        void operator()(Env&&, Var&&) { }
    };


    
  // Copy construction preservation (property)
  // The result of a copy construction is a new value that is equal to the
  // unmodified original.
  template <typename T>
    struct copy_construction_preservation
    {
      static_assert(Equality_comparable<T>(), "");

      bool operator()(const T& a) const
      {
        static_assert(Copy_constructible<T>(), "");
        return T{a} == a;
      }
    };
    
    
  
  // Copy assignment preservation (property)
  // The result of copy-assigning a value is that the assigned variable is
  // equal to the unmodified original.
  template <typename T>
    struct copy_assignment_preservation
    {
      static_assert(Equality_comparable<T>(), "");
      
      bool operator()(const T& a)
      {
        static_assert(Copy_assignable<T>(), "");
        T b = a;
        return b == a;
      }
    };

    
    
  // Copy semantics (specification)
  // A type T implements copy semantics, if for all values of T a copy
  // compares equal to its original.
  template <typename T>
    struct copy_semantics
    {
      template <typename Env, typename Var>
        void operator()(Env&& env, Var& var)
        {
          check(env, move, var);
          check(env, construct, var);
          check(env, assign, var);
        }

      move_semantics<T> move;
      copy_construction_preservation<T> construct;
      copy_assignment_preservation<T> assign;
    };


  
  // Default value (property)
  // The default value property is true when T has a unique default value, that
  // is initialized by expression T{}.
  template <typename T>
    struct default_value_property
    {
      bool operator()() const
      {
        return T{} == T{};
      }
    };



  // Default semantics (specification)
  // The default initialization semantics require a default initialized type
  // to have a unique default value.
  template <typename T>
    struct default_semantics
    {
      template <typename Env>
        void operator()(Env&& env) const
        {
          check(env, init);
        }
        
      default_value_property<T> init;
    };
    
    

  // Regular semantics (specification)
  // A regular type is required to implement default initializtaion, copy,
  // and 
  template <typename T>
    struct regular_semantics
    {
      template <typename Env, typename Var>
        void operator()(Env&& env, Var&& var) const
        {
          check(env, def);
          check(env, copy, var);
          check(env, equal, var);
        }
      
      default_semantics<T> def;
      copy_semantics<T> copy;
      equality_comparable_semantics<T> equal;
    };
    


  // Function semantics (specification)
  // A function is only required to be copy constructible. 

  // Note that we can't  randomly varerate functions (a strange though), the 
  // tested function must be assigned during the initialization of the
  // specification.
  template <typename F>
    struct function_semantics
    {
      function_semantics(F f = {}) : fn(f) { }

      template <typename Env>
        void operator()(Env& env) const
        {
            check(env, copy, fn);
        }

      F fn;
      copy_construction_preservation<F> copy;
    };



  // Equality preserving (property)
  // A function is equality preserving if equal inputs yield equal outputs,
  // for all posssible inputs to the function, including the implicit program
  // state. In other words, given the same arguments, the function always
  // gives the same results.
  template <typename F>
    struct equality_preserving
    {
      equality_preserving(F f = {}) : fn(f) { }

      template <typename... Args>
        auto operator()(Args&&... args) const
          -> Requires<Function<F, Args...>(), bool> 
        {
          return fn(args...) == fn(args...);
        }

      F fn;
    };



  // Regular function semantics (specification)
  // A regular function is equality preserving: equal inputs yield equal
  // outputs.
  template <typename F>
    struct regular_function_semantics
    {
      regular_function_semantics(F f = {}) : regular(f) { }

      template <typename Env, typename... Vars>
        void operator()(Env& env, Vars&&... vars) const
        {
          check(env, regular, std::forward<Vars>(vars)...);
        }

      equality_preserving<F> regular;
    };

} // namespace origin
