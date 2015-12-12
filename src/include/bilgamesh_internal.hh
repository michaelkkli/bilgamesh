#ifndef BILGAMESH_INTERNAL_HH
#define BILGAMESH_INTERNAL_HH
/*
 * Guard macros placed before pragma once to allow multiple-include
 * optimization if pragma once is unsupported.
 * https://gcc.gnu.org/onlinedocs/cppinternals/Guard-Macros.html
 */
#pragma once

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
    _bgm_action (bool, _bgm_move_capture, _bgm_piece_to_piece, T start, T end, const std::set<T>& = {}, const std::set<T>& = {});
    operator bool () const;
    bool is_man_to_king () const;
    T first () const;
    T last () const;
    _bgm_action<T> clone () const;
    _bgm_action<T>& join (const _bgm_action<T>&);
    void apply (uint64_t* board) const;
  private:
    bool black_move;
    _bgm_move_capture move_capture;
    _bgm_piece_to_piece piece_to_piece;
    T start_;
    T end_;
    std::set<T> captured_men;
    std::set<T> captured_kings;
};

template <class T>
class _bgm_board {
  public:
    _bgm_board ();
    void count_black (int& men, int& kings) const;
    void count_white (int& men, int& kings) const;
    void get_actions (std::vector<_bgm_action<T>>&);
//    void get_step_man_move (int pos, std::set<_bgm_action<T>>&);
//    void get_step_man_capture (int pos, std::set<_bgm_action<T>>&);
//    void get_step_king_move (int pos, std::set<_bgm_action<T>>&);
//    void get_step_king_capture (int pos, std::set<_bgm_action<T>>&);
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

void
_bgm_get_occupied_positions (uint64_t, std::set<uint8_t>&, int max);

uint64_t
_bgm_get_reverse_bitpairs (uint64_t);

#endif // BILGAMESH_INTERNAL_HH
