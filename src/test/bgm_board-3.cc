#include "bilgamesh_internal.hh"
#include <cstdlib>

int main (int argc, char *argv[])
{
  _bgm_board<int8_t> a;
  a.set ({1, 2, 3, 4, 5, 7, 10, 11, 12, 14, 16}, {},
    {6, 21, 22, 23, 24, 26, 27, 28, 29, 30, 31, 32}, {}, true);
  //a.set ({2, 10}, {}, {6}, {}, true);

  std::vector<_bgm_action<int8_t>> actions;
  a.get_actions(actions);

  if (1!=actions.size()) {
    abort();
  }
}
