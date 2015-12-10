#include "bilgamesh_internal.hh"
#include <cstdint>
#include <cstdlib>

int main (int argc, char *argv[])
{
  _bgm_action<uint8_t> a;

  if (a) {
    abort();
  }
}
