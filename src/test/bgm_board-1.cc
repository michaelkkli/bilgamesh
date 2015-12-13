#include "bilgamesh_internal.hh"
#include <cstdlib>

int main (int argc, char *argv[])
{
  _bgm_board<int8_t> a;
  int men, kings;

  a.count_black (men, kings);
  if ( (12!=men) || (0!=kings) ) {
    abort();
  }

  a.count_white (men, kings);
  if ( (12!=men) || (0!=kings) ) {
    abort();
  }

  std::vector<_bgm_action<int8_t>> actions;
  a.get_actions(actions);

  if (7!=actions.size()) {
    abort();
  }
}
