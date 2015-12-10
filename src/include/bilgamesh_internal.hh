#ifndef BILGAMESH_INTERNAL_HH
#define BILGAMESH_INTERNAL_HH
/*
 * Guard macros placed before pragma once to allow multiple-include
 * optimization if pragma once is unsupported.
 * https://gcc.gnu.org/onlinedocs/cppinternals/Guard-Macros.html
 */
#pragma once

#include <set>

enum _bgm_move_capture { _bgm_move, _bgm_capture };
enum _bgm_piece_to_piece { _bgm_man_to_man, _bgm_man_to_king, _bgm_king_to_king };

template <class T>
class _bgm_action {
  public:
    _bgm_action();
    operator bool() const;
  private:
    bool black_move;
    _bgm_move_capture move_capture;
    _bgm_piece_to_piece piece_to_piece;
    T start;
    T end;
    std::set<T> captured_men;
    std::set<T> captured_kings;
};

#endif // BILGAMESH_INTERNAL_HH
