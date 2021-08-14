///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the alternating disks
// problem.
//
// As provided, this header has four functions marked with TODO comments.
// You need to write in your own implementation of these functions.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

// TODO
#include <functional>
#include <iostream>

enum disk_color
{
  DISK_DARK,
  DISK_LIGHT
};

class disk_state
{
private:
  std::vector<disk_color> _colors;

public:
  disk_state(size_t light_count)
      : _colors(light_count * 2, DISK_DARK)
  {

    assert(light_count > 0);

    for (size_t i = 1; i < _colors.size(); i += 2)
    {
      _colors[i] = DISK_LIGHT;
    }
  }

  bool operator==(const disk_state &rhs) const
  {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const
  {
    return _colors.size();
  }

  size_t light_count() const
  {
    return total_count() / 2;
  }

  size_t dark_count() const
  {
    return light_count();
  }

  bool is_index(size_t i) const
  {
    return (i < total_count());
  }

  disk_color get(size_t index) const
  {
    assert(is_index(index));
    return _colors[index];
  }

  void swap(size_t left_index)
  {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  std::string to_string() const
  {
    std::stringstream ss;
    bool first = true;
    for (auto color : _colors)
    {
      if (!first)
      {
        ss << " ";
      }

      if (color == DISK_LIGHT)
      {
        ss << "L";
      }
      else
      {
        ss << "D";
      }

      first = false;
    }
    return ss.str();
  }

  void do_all()
  {
    std::cout << total_count() << "\n"
              << light_count() << "\n"
              << dark_count() << "\n"
              << is_index(0) << "\n"
              << get(0) << "\n"
              << to_string() << "\n\n";
  }
  // Return true when this disk_state is in alternating format. That means
  // that the first disk at index 0 is light, the second disk at index 1
  // is dark, and so on for the entire row of disks.
  bool is_initialized() const
  {
    for (size_t i = 0; i < _colors.size(); i++)
    {
      if (i % 2 == 0 && get(i) == DISK_LIGHT)
        return false;
      if (i % 2 != 0 && get(i) == DISK_DARK)
        return false;
    }

    return true;
  }

  // Return true when this disk_state is fully sorted, with all dark disks
  // on the left (low indices) and all light disks on the right (high
  // indices).
  bool is_sorted() const
  {
    // TODO: Write code for this function, including rewriting the return
    // statement, and then delete these comments.
    for (size_t i = 0; i < _colors.size(); i++)
    {
      if (i < _colors.size() / 2 && get(i) != DISK_DARK)
        return false;
      if (i >= _colors.size() / 2 && get(i) != DISK_LIGHT)
        return false;
    }
    return true;
  }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks
{
private:
  disk_state _after;
  unsigned _swap_count;

public:
  sorted_disks(const disk_state &after, unsigned swap_count)
      : _after(after), _swap_count(swap_count) {}

  sorted_disks(disk_state &&after, unsigned swap_count)
      : _after(after), _swap_count(swap_count) {}

  const disk_state &after() const
  {
    return _after;
  }

  unsigned swap_count() const
  {
    return _swap_count;
  }
};

// Algorithm that sorts disks using the alternate algorithm.
sorted_disks sort_alternate(const disk_state &before)
{
  // TODO: Write code for this function, including rewriting the return
  // statement, and then delete these comments.
  int algorithm = 0;
  for (size_t i = 0; i < before.light_count(); i++) // Begin at i and iterate n/2 times.
  {
    for (size_t n = i; n < before.total_count() - 1; n++) // Begin at i and iterate n-1 times.
    {
      if (before.get(n) > before.get(n + 1)) // If the current is greater than the next. (Light is greater than Dark)
      {
        const_cast<disk_state &>(before).swap(n); // Swap and increment algorithm.
        algorithm++;
      }
    }
  }
  return sorted_disks(before, algorithm); // Return new state.
}

// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state &before)
{
  // TODO: Write code for this function, including rewriting the return
  // statement, and then delete these comments.
  int algorithm = 0;
  for (size_t i = 0; i < before.light_count(); i++) // Begin at i and iterate for n/2 times
  {
    for (size_t j = i; j < before.total_count() - 1; j++) // Begin at j = i and iterate n-1 times
    {
      if (before.get(j) > before.get(j + 1)) // If current is greater than the next. (Light > Dark)
      {
        const_cast<disk_state &>(before).swap(j); // Swap and increment algorithm.
        algorithm++;
      }
    }
    for (size_t j = before.total_count() - 1; j > 0; j--) // Start at n-1 and decrement.
    {
      if (before.get(j) < before.get(j - 1)) // If current is less than j-1. Black is less than light.
      {
        const_cast<disk_state &>(before).swap(j - 1); // Swap and increment algorithm.
        algorithm++;
      }
    }
  }
  return sorted_disks(before, algorithm);
}
