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
EIGEN_DEVICE_FUNC INLINE
Eigen::TensorFixedSize<dtype, Eigen::Sizes<((h - 2) >> 1), ((w - 2) >> 1), c>, Eigen::ColMajor, index_t> gauss_half(
      Eigen::TensorFixedSize<dtype, Eigen::Sizes<h, w, c>, Eigen::ColMajor, index_t> const& src) {
  static constexpr index_t hh = (h - 2) >> 1;
  // static constexpr index_t hw = (w - 2) >> 1;
  static constexpr bool is_float = std::is_floating_point<dtype>::value;
  static constexpr size_t dbits = is_float ? 0 : sizeof(dtype) << 3;
  // Would love to use ldexp but not constexpr :_(
  static constexpr dtype _w_c = is_float ? gauss::kCenter : gauss::kCenter * (1 << dbits) + 0.5;
  static constexpr dtype _w_p = is_float ? gauss::kPeriph : gauss::kPeriph * (1 << dbits) + 0.5;

  typedef Eigen::array<index_t, 3> a3;
  // static constexpr a3 stride{2, 2, 1};
  // return src.stride(stride);

  //          vvvv "drat!"
  static const auto kernel{Eigen::TensorFixedSize<dtype, Eigen::Sizes<3, 1>, Eigen::ColMajor, index_t>().setValues({{_w_p}, {_w_c}, {_w_p}})};
  static constexpr a3 patch_y{3, 1, 1};
  // static constexpr a3 patch_x{1, 3, 1};
  static constexpr a3 shape_y1{(h - 2) * w * c, 3};
  static constexpr a3 shape_y2{h - 2, w, c};
  static constexpr a3 stride_y{2, 1, 1};
  // static constexpr a3 stride_x{1, 2, 1};
  static constexpr Eigen::array<Eigen::IndexPair<index_t>, 1> prod_dim{Eigen::IndexPair<index_t>(1, 0)};
  Eigen::TensorFixedSize<dtype, Eigen::Sizes<(hh), w, c>, Eigen::ColMajor, index_t> vertical = (src
    .extract_patches(patch_y)   // <(h-2)*w*c, 3, 1, 1>
    .reshape(shape_y1)          // <(h-2)*w*c, 3>
    .contract(kernel, prod_dim) // <(h-2)*w*c, 1>
    .reshape(shape_y2)          // <h-2, w, c>
    .stride(stride_y)           // <hh, w, c>
  );
  util::write(vertical, "vertical.png");
  return {};
}



} // namespace conv

#endif // CONV_HPP_
