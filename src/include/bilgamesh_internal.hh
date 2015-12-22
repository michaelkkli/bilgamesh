#ifndef BILGAMESH_INTERNAL_HH
#define BILGAMESH_INTERNAL_HH
/*
 * Guard macros placed before pragma once to allow multiple-include
 * optimization if pragma once is unsupported.
 * https://gcc.gnu.org/onlinedocs/cppinternals/Guard-Macros.html
 */
#pragma once

#include <iostream>
#include <random>
#include <set>
#include <utility>
#include <vector>
#include <cstdint>

enum _bgm_move_capture { _bgm_move, _bgm_capture };
enum _bgm_piece_to_piece { _bgm_man_to_man, _bgm_man_to_king, _bgm_king_to_king };

template <class T>
class _bgm_action {
  public:
    _bgm_action ();
    _bgm_action (bool, _bgm_move_capture, _bgm_piece_to_piece, T start, T end, const std::vector<T>& = {}, const std::vector<T>& = {});
    operator bool () const;
    bool is_man_to_king () const;
    bool is_capture () const;
    T first () const;
    T last () const;
    _bgm_action<T> clone () const;
    _bgm_action<T>& join (const _bgm_action<T>&);
    void apply (uint64_t* board) const;
    void get_all_hop_positions (std::vector<int>&) const;
  private:
    bool black_move;
    _bgm_move_capture move_capture;
    _bgm_piece_to_piece piece_to_piece;
    T start_;
    T end_;
    std::vector<T> captured_men;
    std::vector<T> captured_kings;
    std::vector<T> intermediate_hops;
};

template <class T>
std::ostream&
operator << (std::ostream&, const _bgm_action<T>&);

template <class T>
class _bgm_board {
  public:
    _bgm_board ();
    bool is_black_move () const;
    bool swap_turn ();
    void count_black (int& men, int& kings) const;
    void count_white (int& men, int& kings) const;
    void set_raw (uint64_t&, uint64_t&);
    void set (const std::vector<int>& men_black, const std::vector<int>& kings_black, const std::vector<int>& men_white, const std::vector<int>& kings_white, bool bm);
    void apply (const _bgm_action<T>&);
    void get_actions (std::vector<_bgm_action<T>>&);
  private:
    void get_step_man_move (uint64_t black_board, uint64_t white_board, int pos, std::vector<_bgm_action<T>>&);
    void get_step_king_move (uint64_t black_board, uint64_t white_board, int pos, std::vector<_bgm_action<T>>&);
    void get_step_man_capture (uint64_t black_board, uint64_t white_board, int pos, std::vector<_bgm_action<T>>&);
    void get_step_king_capture (uint64_t black_board, uint64_t white_board, int pos, std::vector<_bgm_action<T>>&);
    uint64_t hboard[2];
    bool black_move;
    static constexpr uint64_t men_mask = 0x5555555555555555;
    static constexpr uint64_t kings_mask = 0xAAAAAAAAAAAAAAAA;
    static constexpr uint64_t WL_capture_mask = 0x0000FCFCFCFCFCFC;
    static constexpr uint64_t WH_capture_mask = 0x00003F3F3F3F3F3F;
    static constexpr uint64_t BL_capture_mask = 0xFCFCFCFCFCFC0000;
    static constexpr uint64_t BH_capture_mask = 0x3F3F3F3F3F3F0000;
    static constexpr uint64_t WL_move_mask = 0x00FFFCFFFCFFFCFF;
    static constexpr uint64_t WH_move_mask = 0x003FFF3FFF3FFF3F;
    static constexpr uint64_t BL_move_mask = 0xFCFFFCFFFCFFFC00;
    static constexpr uint64_t BH_move_mask = 0xFF3FFF3FFF3FFF00;
    static constexpr uint64_t man_to_king_mask = 0xFF00000000000000;
    static constexpr uint64_t fourfive_mask = 0x00FF00FF00FF00FF;
};

template <class T>
class _bgm_strategy_random {
  public:
    _bgm_action<T> operator () (const _bgm_board<T>&, const std::vector<_bgm_action<T>>&);
};

template <class T>
class _bgm_strategy_monte_carlo {
  public:
    _bgm_strategy_monte_carlo (int games = 100, int moves = 100, double king_multiplier = 1.5);
    _bgm_action<T> operator () (const _bgm_board<T>&, const std::vector<_bgm_action<T>>&);
  private:
    int num_games;
    int num_moves;
    double kings_multiplier;
};

void
_bgm_get_occupied_positions (uint64_t, std::vector<int8_t>&, int max);

uint64_t
_bgm_get_reverse_bitpairs (uint64_t);

template <class T>
void
_bgm_string_to_board (T s, uint64_t& bhboard, uint64_t& whboard);

void
_bgm_position_to_row_col (int, int&, int&);

#endif // BILGAMESH_INTERNAL_HH
