// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include "matrix_alloc_base.hpp"

namespace origin
{
    template<typename T, std::size_t Dimension, typename Alloc>
    struct matrix_impl_base
        :public matrix_alloc_base<T, Alloc>
    {
        
    };
    
} // end origin