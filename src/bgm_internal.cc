#include "bilgamesh_internal.hh"
#include <cstdint>

template <class T>
_bgm_action<T>::_bgm_action ()
{
  black_move = true;
  move_capture = _bgm_move;
  piece_to_piece = _bgm_man_to_man;
  start = 0;
  end = 0;
}

template <class T>
_bgm_action<T>::operator bool() const
{
  return 0 != start;
}

template class _bgm_action<uint8_t>;
