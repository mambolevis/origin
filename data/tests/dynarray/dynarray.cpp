// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <string>

#include <origin/dynarray/dynarray.hpp>

using namespace std;
using namespace origin;

// FIXME: Write real test suites

int main()
{
  {
    dynarray<int> x;
    assert(( x.empty() ));
  }

  {
    dynarray<string> x(5);
    assert(( x.size() == 5 ));
  }
}
