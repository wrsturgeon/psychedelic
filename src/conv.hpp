#ifndef CONV_HPP_
#define CONV_HPP_

#include <stdint.h>
#include <math.h>
#include <unsupported/Eigen/CXX11/TensorSymmetry>

#include "util.hpp"

// Stupid fucking sqrt & erfc aren't constexpr >:(((
#include "gauss_kernel.hpp"

namespace conv {



template <typename dtype, typename index_t, index_t h, index_t w, index_t c>
EIGEN_DEVICE_FUNC EIGEN_ALWAYS_INLINE
Eigen::TensorFixedSize<dtype, Eigen::Sizes<(h >> 1), (w >> 1), c>, Eigen::ColMajor, index_t> gauss_half(
      Eigen::TensorFixedSize<dtype, Eigen::Sizes<h, w, c>, Eigen::ColMajor, index_t> const& src) {
  static constexpr index_t hh = h >> 1;
  // static constexpr index_t hw = w >> 1;
  static constexpr size_t dbits = sizeof(dtype) << 3;
  static constexpr dtype _w_c = gauss::kCenter * (1 << dbits) + 0.5;
  static constexpr dtype _w_p = gauss::kCenter * (1 << dbits) + 0.5;

  // Vertical convolution
  Eigen::TensorFixedSize<dtype, Eigen::Sizes<hh, w, c>, Eigen::ColMajor, index_t> tmp;
  for (index_t y = 0; y < hh; ++y) {
    for (index_t x = 0; x < w; ++x) {
      for (index_t z = 0; z < c; ++z) {
        tmp(y, x, z) = (
              _w_p * src((y << 1) - 1, x, z) +
              _w_c * src((y << 1)    , x, z) +
              _w_p * src((y << 1) | 1, x, z)
        ) >> dbits;
      }
    }
  }

  return tmp;
}



} // namespace conv

#endif // CONV_HPP_
