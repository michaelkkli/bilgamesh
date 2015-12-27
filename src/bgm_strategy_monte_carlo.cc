#include "bilgamesh_internal.hh"

#include <utility>
#include <valarray>
#include <vector>

template <class T>
_bgm_strategy_monte_carlo<T>::_bgm_strategy_monte_carlo (int g, int m, double km, int ngc, int nmc) :
  num_games (g), num_moves(m), kings_multiplier(km), num_games_capture(ngc), num_moves_capture(nmc)
{
}

template <class T>
_bgm_action<T>
_bgm_strategy_monte_carlo<T>::operator () (const _bgm_board<T>& board, const std::vector<_bgm_action<T>>& in)
{
  int num_actions = in.size ();
  int men, kings;
  bool black_move = board.is_black_move ();
  bool victory_encountered;
  bool black_victory;

  if (black_move) {
    board.count_black (men, kings);
  } else {
    board.count_white (men, kings);
  }

  std::valarray<double> men_count (0., num_actions);
  std::valarray<double> kings_count (0., num_actions);

  _bgm_board<T> tmp_board;
  std::vector<_bgm_action<int8_t>> vacts;
  _bgm_strategy_random<int8_t> rand_strat;

  int local_num_games;
  int local_num_moves;

  if (!in[0].is_capture ()) {
    local_num_games = num_games;
    local_num_moves = num_moves;
  } else {
    local_num_games = num_games_capture;
    local_num_moves = num_moves_capture;
  }

  for (int act = 0; act < num_actions; act++) {
    for (int game = 0; game < local_num_games; game++) {
      victory_encountered = false;

      tmp_board = board;
      tmp_board.apply (in[act]); // Play switches to other side.
      tmp_board.get_actions (vacts);

      if (vacts.empty ()) {
	victory_encountered = true;
	black_victory = !tmp_board.is_black_move ();
      } else {
	if (!in[0].is_capture ()) {
	  local_num_moves = num_moves;
	} else {
	  local_num_moves = num_moves_capture;
	}

	for (int move = 0; move < local_num_moves; move++) {
	  _bgm_action<int8_t> tmp_act = rand_strat (board, vacts);
	  if (tmp_act.is_capture ()) {
	    local_num_moves = num_moves_capture;
	  }

	  tmp_board.apply (tmp_act);

	  tmp_board.get_actions (vacts);
	  if (vacts.empty ()) {
	    victory_encountered = true;
	    black_victory = !tmp_board.is_black_move ();
	    break;
	  }
	}
      }

      if (!victory_encountered) {
	if (black_move) {
	  tmp_board.count_black (men, kings);
	  men_count[act] += men;
	  kings_count[act] += kings;

	  tmp_board.count_white (men, kings);
	  men_count[act] -= men;
	  kings_count[act] -= kings;
	} else {
	  tmp_board.count_black (men, kings);
	  men_count[act] -= men;
	  kings_count[act] -= kings;

	  tmp_board.count_white (men, kings);
	  men_count[act] += men;
	  kings_count[act] += kings;
	}
      } else {
	if (black_victory) {
	  if (black_move) {
	    tmp_board.count_black (men, kings);
	    men_count[act] += men;
	    kings_count[act] += kings;
	  } else {
	    tmp_board.count_black (men, kings);
	    men_count[act] -= men;
	    kings_count[act] -= kings;
	  }
	} else {
	  if (black_move) {
	    tmp_board.count_white (men, kings);
	    men_count[act] -= men;
	    kings_count[act] -= kings;
	  } else {
	    tmp_board.count_white (men, kings);
	    men_count[act] += men;
	    kings_count[act] += kings;
	  }
	}
      }

    }
  }

  std::valarray<double> score(0., num_actions);
  score = men_count;
  score += kings_multiplier * kings_count;

  int max_ind = 0;
  double max_val = -32.;

  std::vector<int> candidates;

  for (int i = 0; i < num_actions; i++) {
    if (score[i] > max_val) {
      max_ind = i;
      max_val = score[i];
    }
  }

  for (int i = 0; i < num_actions; i++) {
    if (score[i] >= max_val) {
      candidates.push_back (i);
    }
  }
  
  int num_cands = candidates.size ();

  if (num_cands <= 1) {
    return in[max_ind];
  }

  int cand_ind = (int)std::round ((double) std::rand()*(num_cands-1)/((double)RAND_MAX));
  return in[candidates[cand_ind]];
}

template class _bgm_strategy_monte_carlo<int8_t>;
