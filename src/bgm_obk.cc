#include "bilgamesh_internal.hh"
#include <iterator>

void
_bgm_obk::insert_accepted (uint64_t other, std::string move, uint64_t current)
{
  other_to_accepted.insert (std::make_pair(other, move));
  accepted_to_current.insert (std::make_pair(move, current));
}

void
_bgm_obk::insert_rejected (uint64_t other, std::string move, uint64_t current)
{
  other_to_rejected.insert (std::make_pair(other, move));
  rejected_to_current.insert (std::make_pair(move, current));
}

void
_bgm_obk::find (uint64_t other, uint64_t current, std::vector<std::string>& accepted, std::vector<std::string>* rejected) const
{
  accepted.clear ();
  auto r = other_to_accepted.equal_range (other);
  // Basic implementation assumes non-repeats.
  if (1 < std::distance (r.first, r.second)) {
    accepted.push_back ((r.first)->second);
  }

  if (rejected) {
    auto r = other_to_rejected.equal_range (other);
    if (1 < std::distance (r.first, r.second)) {
      rejected->push_back ((r.first)->second);
    }
  }
}

