#pragma once

#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <type_traits>

namespace statistics {

namespace detail {

template <typename Container> auto median_in_place(Container &values) {
  using ValueType =
      std::remove_cv_t<std::remove_reference_t<decltype(*values.data())>>;

  static_assert(std::is_floating_point_v<ValueType>,
                "median() currently requires a floating-point value type");

  if (values.size() == 0) {
    throw std::invalid_argument("median() requires a non-empty container");
  }

  auto *const begin = values.data();
  auto *const end = begin + values.size();
  auto *const mid = begin + values.size() / 2;

  std::nth_element(begin, mid, end);

  // Odd number of elements
  if (values.size() % 2 != 0) {
    return *mid;
  }

  // nth_element guarantees that everything before mid is <= *mid,
  // but that range itself is not sorted.
  const auto lower = std::max_element(begin, mid);

  return std::midpoint(*lower, *mid);
}

} // namespace detail

template <typename Container> auto median(Container values) {
  return detail::median_in_place(values);
}

template <typename Container> auto absolute_deviations(Container values) {
  const auto center = median(values);
  auto *const begin = values.data();
  auto *const end = begin + values.size();
  for (auto *value = begin; value != end; ++value) {
    *value = std::abs(*value - center);
  }
  return values;
}

double normpdf(double x, double mu, double sigma) {
  return (1 / (std::sqrt(2 * std::numbers::pi * std::pow(sigma, 2)))) *
         std::exp(-std::pow((x - mu), 2) / (2 * std::pow(sigma, 2)));
}

} // namespace statistics
