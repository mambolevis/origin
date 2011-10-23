// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>
#include <iostream>

#include <origin/utility/typestr.hpp>
#include <origin/range.hpp>

using namespace std;
using namespace origin;

template<typename R>
  void print(R const& r)
  {
    for(auto x : r)
      cout << x << " ";
    cout << "\n";
  }

int main()
{
  vector<int> v = {1, 2, 3, 4, 5};
  auto f = v.begin();
  auto l = f + 3;
  for(auto r : combinations(v, 3)) {
    for(auto i = f; i != l; ++i)
      cout << *i << ' ';
    cout << '\n';
  }
}
