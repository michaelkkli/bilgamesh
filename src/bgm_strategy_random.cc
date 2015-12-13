#include "bilgamesh_internal.hh"

#include <cmath>
#include <cstdlib>

template <class T>
_bgm_action<T>
_bgm_strategy_random<T>::operator () (const _bgm_board<T>& board, const std::vector<_bgm_action<T>>& in)
{
  size_t n = in.size ();

  if (0 == n) {
    return _bgm_action<T> ();
  }

  int tmp = (int)std::round((double)(n-1)*rand()/RAND_MAX);
  
  return in[tmp];
}

template class _bgm_strategy_random<int8_t>;
