#include "bilgamesh_internal.hh"
#include <tuple>
#include <utility>
#include <vector>
#include <cstdlib>

template <class T>
_bgm_board<T>::_bgm_board () : black_move (true)
{
  hboard[0] = 0x0000000000555555ULL;
  hboard[1] = 0x0000000000555555ULL;
}

template <class T>
bool
_bgm_board<T>::is_black_move () const
{
  return black_move;
}

template <class T>
bool
_bgm_board<T>::swap_turn ()
{
  black_move = !black_move;
  return black_move;
}

template <class T>
void
_bgm_board<T>::count_black (int& men, int& kings) const
{
  men = __builtin_popcountll(men_mask & hboard[0]);
  kings = __builtin_popcountll(kings_mask & hboard[0]);
}

template <class T>
void
_bgm_board<T>::count_white (int& men, int& kings) const
{
  men = __builtin_popcountll(men_mask & hboard[1]);
  kings = __builtin_popcountll(kings_mask & hboard[1]);
}

template <class T>
void
_bgm_board<T>::set (const std::vector<int>& men_black, const std::vector<int>& kings_black, const std::vector<int>& men_white, const std::vector<int>& kings_white, bool bm)
{
  hboard[0] = 0x0ULL; hboard[1] = 0x0ULL;

  for (auto i : men_black) {
    hboard[0] |= (0x1ULL << 2*(i - 1));
  }

  for (auto i : kings_black) {
    hboard[0] |= (0x2ULL << 2*(i - 1));
  }

  for (auto i : men_white) {
    hboard[1] |= (0x1ULL << 2*(i - 1));
  }

  for (auto i : kings_white) {
    hboard[1] |= (0x2ULL << 2*(i - 1));
  }

  hboard[1] = _bgm_get_reverse_bitpairs (hboard[1]);
  black_move = bm;
}

template <class T>
void
_bgm_board<T>::set_raw (uint64_t& b, uint64_t& w)
{
  hboard[0] = b; hboard[1] = w;
}

template <class T>
void
_bgm_board<T>::apply (const _bgm_action<T>& in)
{
  in.apply (hboard);

  black_move = !black_move;
}

template <class T>
void
_bgm_board<T>::get_actions (std::vector<_bgm_action<T>>& actions)
{
  int num_men, num_kings;
  uint64_t chboard, ohboard;
  std::vector<T> men_pos, kings_pos;

  actions.clear();

  if (black_move) {
    chboard = hboard[0];
    ohboard = hboard[1];
    count_black (num_men, num_kings);
  } else {
    chboard = hboard[1];
    ohboard = hboard[0];
    count_white (num_men, num_kings);
  }

  _bgm_get_occupied_positions (chboard & men_mask, men_pos, num_men);
  _bgm_get_occupied_positions (chboard & kings_mask, kings_pos, num_kings);

  std::vector<_bgm_action<T>> captures;
  std::vector<_bgm_action<T>> chains;
  uint64_t tmp_board[2];

  const bool promotion_ends_turn = false;

  for (auto pos : men_pos) {
    get_step_man_capture (chboard, ohboard, pos, captures);

    for (auto cap : captures) {
      if (cap.is_man_to_king()) {
	if (!promotion_ends_turn) {
	  chains.push_back (cap);
	} else {
	  actions.push_back (cap);
	}
      } else {
	chains.push_back (cap);
      }
    }

    while (!chains.empty()) {
      auto chain = chains.back ();
      chains.pop_back ();

      if (black_move) {
	tmp_board[0] = chboard;
	tmp_board[1] = ohboard;
	chain.apply (tmp_board);

	if (chain.is_man_to_man ()) {
	  get_step_man_capture (tmp_board[0], tmp_board[1], chain.last (), captures);
	} else {
	  get_step_king_capture (tmp_board[0], tmp_board[1], chain.last (), captures);
	}
      } else {
	tmp_board[0] = ohboard;
	tmp_board[1] = chboard;
	chain.apply (tmp_board);

	if (chain.is_man_to_man ()) {
	  get_step_man_capture (tmp_board[1], tmp_board[0], 33 - chain.last (), captures);
	} else {
	  get_step_king_capture (tmp_board[1], tmp_board[0], 33 - chain.last (), captures);
	}
      }

      if (captures.empty ()) {
	actions.push_back (chain);
      } else {
	for (auto cap : captures) {
	  if (cap.is_man_to_king()) {
	    if (!promotion_ends_turn) {
	      chains.push_back (chain.clone().join(cap));
	    } else {
	      actions.push_back (chain.clone().join(cap));
	    }
	  } else {
	    chains.push_back (chain.clone().join(cap));
	  }
	}
      }
    }
  }

  for (auto pos : kings_pos) {
    get_step_king_capture (chboard, ohboard, pos, captures);

    for (auto cap : captures) {
      chains.push_back (cap);
    }

    while (!chains.empty()) {
      auto chain = chains.back ();
      chains.pop_back ();

      if (black_move) {
	tmp_board[0] = chboard;
	tmp_board[1] = ohboard;
	chain.apply (tmp_board);

	get_step_king_capture (tmp_board[0], tmp_board[1], chain.last (), captures);
      } else {
	tmp_board[0] = ohboard;
	tmp_board[1] = chboard;
	chain.apply (tmp_board);

	get_step_king_capture (tmp_board[1], tmp_board[0], 33 - chain.last (), captures);
      }

      if (captures.empty ()) {
	actions.push_back (chain);
      } else {
	for (auto cap : captures) {
	  chains.push_back (chain.clone().join(cap));
	}
      }
    }
  }

  if (!actions.empty ()) {
    return;
  }

  // Handle moves.

  std::vector<_bgm_action<T>> tmp_actions;

  for (auto pos : men_pos) {
    get_step_man_move (chboard, ohboard, pos, tmp_actions);

    if (!tmp_actions.empty ()) {
      actions.insert (actions.end (), tmp_actions.begin(), tmp_actions.end ());
    }
  }

  for (auto pos : kings_pos ) {
    get_step_king_move (chboard, ohboard, pos, tmp_actions);

    if (!tmp_actions.empty ()) {
      actions.insert (actions.end (),tmp_actions.begin (), tmp_actions.end ());
    }
  }
}

template <class T>
void
_bgm_board<T>::get_step_man_capture (uint64_t chboard, uint64_t ohboard, int pos, std::vector<_bgm_action<T>>& out)
{
  uint64_t other = _bgm_get_reverse_bitpairs(ohboard);
  uint64_t overlap = (chboard | other);
  uint64_t position_only = (0x3ULL << 2*(pos - 1));
  uint64_t not_position = ~position_only;
  uint64_t landing_pos, capture_pos;
  int8_t clear_offset, occupied_offset, capture_offset;

  std::vector<T> men_captured, kings_captured;

  // Overlap-clear; action mask; fourfive, threefour other-occupied.
  std::tuple<int8_t, uint64_t, int8_t, int8_t> action_spec[] =
    {
      std::tuple<int8_t, uint64_t, int8_t, int8_t>{ 7, uint64_t(WL_capture_mask), 4, 3 },
      std::tuple<int8_t, uint64_t, int8_t, int8_t>{ 9, uint64_t(WH_capture_mask), 5, 4 }
    };

  out.clear();

  for (auto t : action_spec) {
    if (position_only & std::get<1>(t)) {
      if (position_only & fourfive_mask) {
	capture_offset = std::get<2>(t);
      } else { // threefour
	capture_offset = std::get<3>(t);
      }
      clear_offset = std::get<0>(t);

      landing_pos = (0x3ULL << 2*(pos - 1 + clear_offset));
      capture_pos = (0x3ULL << 2*(pos - 1 + capture_offset));

      if (!(overlap & landing_pos) && (other & capture_pos)) {
	if (other & (0x1ULL << 2*(pos - 1 + capture_offset))) {
	  men_captured = {(T)(pos + capture_offset)};
	  kings_captured = {};
	} else { // king captured
	  men_captured = {};
	  kings_captured = {(T)(pos + capture_offset)};
	}

	if (!(landing_pos & man_to_king_mask)) {
	  out.push_back (_bgm_action<T>(black_move, _bgm_capture, _bgm_man_to_man, pos, pos + clear_offset, men_captured, kings_captured));
	} else {
	  out.push_back (_bgm_action<T>(black_move, _bgm_capture, _bgm_man_to_king, pos, pos + clear_offset, men_captured, kings_captured));
	}
      }
    }
  }
}

template <class T>
void
_bgm_board<T>::get_step_man_move (uint64_t chboard, uint64_t ohboard, int pos, std::vector<_bgm_action<T>>& out)
{
  uint64_t other = _bgm_get_reverse_bitpairs(ohboard);
  uint64_t overlap = (chboard | other);
  uint64_t position_only = (0x3ULL << 2*(pos - 1));
  uint64_t not_position = ~position_only;
  uint64_t landing_pos;
  int8_t clear_offset;

  // Overlap-clear; action mask.
  std::tuple<int8_t, int8_t, uint64_t> action_spec[] =
    {
      std::tuple<int8_t, int8_t, uint64_t>{ 4, 3, uint64_t(WL_move_mask) },
      std::tuple<int8_t, int8_t, uint64_t>{ 5, 4, uint64_t(WH_move_mask) }
    };

  out.clear();

  for (auto t : action_spec) {
    if (position_only & std::get<2>(t)) {
      if (position_only & fourfive_mask) {
	clear_offset = std::get<0>(t);
      } else { // threefour
	clear_offset = std::get<1>(t);
      }

      landing_pos = (0x3ULL << 2*((int)pos - 1 + (int)clear_offset));

      if (0x0ULL == (overlap & landing_pos)) {
	if (0x0ULL == (landing_pos & man_to_king_mask)) {
	  out.push_back (_bgm_action<T>(black_move, _bgm_move, _bgm_man_to_man, pos, pos + clear_offset));
	} else {
	  out.push_back (_bgm_action<T>(black_move, _bgm_move, _bgm_man_to_king, pos, pos + clear_offset));
	}
      }
    }
  }
}

template <class T>
void
_bgm_board<T>::get_step_king_move (uint64_t chboard, uint64_t ohboard, int pos, std::vector<_bgm_action<T>>& out)
{
  uint64_t other = _bgm_get_reverse_bitpairs(ohboard);
  uint64_t overlap = (chboard | other);
  uint64_t position_only = (0x3ULL << 2*(pos - 1));
  uint64_t not_position = !position_only;
  uint64_t landing_pos;
  int8_t clear_offset;

  // Overlap-clear; action mask.
  std::tuple<int8_t, int8_t, uint64_t> action_spec[] =
    {
      std::tuple<int8_t, int8_t, uint64_t>{ 4, 3, uint64_t(WL_move_mask) },
      std::tuple<int8_t, int8_t, uint64_t>{ 5, 4, uint64_t(WH_move_mask) },
      std::tuple<int8_t, int8_t, uint64_t>{ -4, -5, uint64_t(BL_move_mask) },
      std::tuple<int8_t, int8_t, uint64_t>{ -3, -4, uint64_t(BH_move_mask) }
    };

  out.clear();

  for (auto t : action_spec) {
    if (position_only & std::get<2>(t)) {
      if (position_only & fourfive_mask) {
	clear_offset = std::get<0>(t);
      } else { // threefour
	clear_offset = std::get<1>(t);
      }

      landing_pos = (0x3ULL << 2*(pos - 1 + clear_offset));

      if (!(overlap & landing_pos)) {
	out.push_back (_bgm_action<T>(black_move, _bgm_move, _bgm_king_to_king, pos, pos + clear_offset));
      }
    }
  }
}

template <class T>
void
_bgm_board<T>::get_step_king_capture (uint64_t chboard, uint64_t ohboard, int pos, std::vector<_bgm_action<T>>& out)
{
  uint64_t other = _bgm_get_reverse_bitpairs(ohboard);
  uint64_t overlap = (chboard | other);
  uint64_t position_only = (0x3ULL << 2*(pos - 1));
  uint64_t not_position = ~position_only;
  uint64_t landing_pos, capture_pos;
  int8_t clear_offset, occupied_offset, capture_offset;

  std::vector<T> men_captured, kings_captured;

  // Overlap-clear; action mask; fourfive, threefour other-occupied.
  std::tuple<int8_t, uint64_t, int8_t, int8_t> action_spec[] =
    {
      std::tuple<int8_t, uint64_t, int8_t, int8_t>{ 7, uint64_t(WL_capture_mask), 4, 3 },
      std::tuple<int8_t, uint64_t, int8_t, int8_t>{ 9, uint64_t(WH_capture_mask), 5, 4 },
      std::tuple<int8_t, uint64_t, int8_t, int8_t>{ -9, uint64_t(BL_capture_mask), -4, -5 },
      std::tuple<int8_t, uint64_t, int8_t, int8_t>{ -7, uint64_t(BH_capture_mask), -3, -4 }
    };

  out.clear();

  for (auto t : action_spec) {
    if (position_only & std::get<1>(t)) {
      if (position_only & fourfive_mask) {
	capture_offset = std::get<2>(t);
      } else { // threefour
	capture_offset = std::get<3>(t);
      }
      clear_offset = std::get<0>(t);

      landing_pos = (0x3ULL << 2*(pos - 1 + clear_offset));
      capture_pos = (0x3ULL << 2*(pos - 1 + capture_offset));

      if (!(overlap & landing_pos) && (other & capture_pos)) {
	if (other & (0x1ULL << 2*(pos - 1 + capture_offset))) {
	  men_captured = {(T)(pos + capture_offset)};
	  kings_captured = {};
	} else { // king captured
	  men_captured = {};
	  kings_captured = {(T)(pos + capture_offset)};
	}

	out.push_back (_bgm_action<T>(black_move, _bgm_capture, _bgm_king_to_king, pos, pos + clear_offset, men_captured, kings_captured));
      }
    }
  }
}

template class _bgm_board<int8_t>;
