#include "bilgamesh_internal.hh"
#include <utility>
#include <cstdint>

template <class T>
_bgm_action<T>::_bgm_action ()
{
  black_move = true;
  move_capture = _bgm_move;
  piece_to_piece = _bgm_man_to_man;
  start_ = 0;
  end_ = 0;
}

template <class T>
_bgm_action<T>::_bgm_action (bool b, _bgm_move_capture mc, _bgm_piece_to_piece ptp, T s, T e, const std::set<T>& cm, const std::set<T>& ck) : black_move(b), move_capture(mc), piece_to_piece(ptp), start_(s), end_(e), captured_men(cm), captured_kings(ck)
{
}

template <class T>
_bgm_action<T>::operator bool() const
{
  return 0 != start_;
}

template <class T>
bool
_bgm_action<T>::is_man_to_king () const
{
  return _bgm_man_to_king == piece_to_piece;
}

template <class T>
T
_bgm_action<T>::first () const
{
  return start_;
}

template <class T>
T
_bgm_action<T>::last () const
{
  return end_;
}

template <class T>
_bgm_action<T>
_bgm_action<T>::clone () const
{
  return _bgm_action<T>(*this);
}

template <class T>
_bgm_action<T>&
_bgm_action<T>::join (const _bgm_action<T>& in)
{
  end_ = in.end_;
  captured_men.insert (in.captured_men.begin (), in.captured_men.end ());
  captured_kings.insert (in.captured_kings.begin (), in.captured_kings.end ());

  if ((_bgm_man_to_man == piece_to_piece) & (_bgm_man_to_man != in.piece_to_piece)) {
    piece_to_piece = _bgm_man_to_king;
  }

  return *this;
}

template <class T>
void
_bgm_action<T>::apply (uint64_t* board) const
{
  uint64_t x, y;

  if (black_move) {
    x = board[0]; y = board[1];
  } else {
    x = board[1]; y = board[0];
  }

  const uint64_t start_pos = (0x3ULL << 2*(start_ - 1));
  const uint64_t not_start_pos = !start_pos;
  const uint64_t end_pos = (0x3ULL << 2*(end_ - 1));

  x = (x & not_start_pos);

  constexpr uint64_t men_mask = 0x5555555555555555;
  constexpr uint64_t kings_mask = 0xAAAAAAAAAAAAAAAA;

  if (_bgm_man_to_man == piece_to_piece) {
    x |= (end_pos & men_mask); 
  } else { // _bgm_man_to_king
    x |= (end_pos & kings_mask);
  }

  uint64_t op_men = 0x0ULL, op_kings = 0x0ULL;

  for (auto i : captured_men ) {
    op_men |= (0x01ULL << 2*(i - 1));
  }

  for (auto i : captured_kings) {
    op_kings |= (0x10ULL << 2*(i - 1));
  }

  y = (y & !op_men & !op_kings);

  if (black_move) {
    board[0] = x; board[1] = y;
  } else {
    board[0] = y; board[1] = x;
  }
}

template class _bgm_action<uint8_t>;
