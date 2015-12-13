#include "bilgamesh_internal.hh"

#include <utility>
#include <valarray>
#include <vector>

template <class T>
_bgm_strategy_monte_carlo<T>::_bgm_strategy_monte_carlo (int g, int m, double km) :
  num_games (g), num_moves(m), kings_multiplier(km)
{
}

template <class T>
_bgm_action<T>
_bgm_strategy_monte_carlo<T>::operator () (const _bgm_board<T>& board, const std::vector<_bgm_action<T>>& in)
{
  int num_actions = in.size ();
  int men, kings;
  bool black_move = board.is_black_move ();

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

  for (int act = 0; act < num_actions; act++) {
    for (int game = 0; game < num_games; game++) {
      tmp_board = board;
      tmp_board.apply (in[act]);

      for (int move = 0; move < num_moves; move++) {
	tmp_board.get_actions (vacts);
	if (vacts.empty ()) {
	  break;
	}
	tmp_board.apply (rand_strat (board, vacts));
      }

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
    }
  }

  std::valarray<double> score(0., num_actions);
  score = men_count;
  score += kings_multiplier * kings_count;

  int max_ind = 0;
  double max_val = -32.;

  std::vector<std::pair<int, double>> candidates;

  for (int i = 0; i < num_actions; i++) {
    if (score[i] > max_val) {
      max_ind = i;
      max_val = score[i];
    }
  }

  for (int i = 0; i < num_actions; i++) {
    if (score[i] >= max_val) {
      candidates.push_back (std::make_pair (i, score[i]));
    }
  }
  
  int num_cands = candidates.size ();

  if (num_cands <= 1) {
    return in[max_ind];
  }

  return in[(int)std::round ((double) (std::rand()*(num_cands - 1)/RAND_MAX))];
}

template class _bgm_strategy_monte_carlo<int8_t>;
