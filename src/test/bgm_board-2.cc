#include "bilgamesh_internal.hh"
#include <tuple>
#include <vector>
#include <cstdlib>

using std::get;
using std::tuple;
using std::vector;

int main (int argc, char *argv[])
{
  // men_black, kings_black, men_white, kings_white, num_actions_black, num_actions_white
  typedef tuple<vector<int>, vector<int>, vector<int>, vector<int>, int, int> T;

  T tests[] =
  {
    T{ {}, {8}, {}, {25}, 4, 4 },
    T{ {4}, {}, {29}, {}, 1, 1 },
    T{ {15}, {}, {18}, {}, 1, 1 }
  };

  _bgm_board<int8_t> B;
  vector<_bgm_action<int8_t>> acts;

  for (auto i : tests) {
    B.set (get<0>(i), get<1>(i), get<2>(i), get<3>(i), true);
    B.get_actions (acts);
    if (acts.size() != get<4>(i)) {
      abort();
    }
    B.swap_turn();
    B.get_actions (acts);
    if (acts.size() != get<5>(i)) {
      abort();
    }
  }
}
