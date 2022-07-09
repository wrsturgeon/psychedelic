#ifndef CONV_HPP_
#define CONV_HPP_

#include <stdint.h>
#include <math.h>
#include <iostream>

#include <unsupported/Eigen/CXX11/TensorSymmetry>

#include "util.hpp"
#include "bitshift.hpp"

// Stupid fucking sqrt & erfc aren't constexpr >:(((
#include "gauss_kernel.hpp"

namespace conv {



template <typename index_t, index_t h, index_t w, index_t c>
EIGEN_DEVICE_FUNC INLINE
Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<((h - 2) >> 1), ((w - 2) >> 1), c>, Eigen::ColMajor, index_t> gauss_half(
      Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<h, w, c>, Eigen::ColMajor, index_t>& src) {
  static constexpr bool is_float = std::is_floating_point<uint8_t>::value;
  static constexpr size_t dbits = is_float ? 0 : sizeof(uint8_t) << 3;
  // Would love to use ldexp but not constexpr :_(
  static constexpr int16_t _w_p = is_float ? gauss::kPeriph : gauss::kPeriph * (1 << dbits) + 0.5;
  static constexpr int16_t _w_c = is_float ? gauss::kCenter : gauss::kCenter * (1 << dbits) + 0.5;
  // static constexpr int16_t _w_c = is_float ? gauss::kCenter : (1L << dbits) - (2 * _w_p); // Necessarily adds up to e.g. 256

  // // THIS WORKS
  // static constexpr Eigen::array<index_t, 3> stride{2, 2, 1};
  // static constexpr Eigen::array<index_t, 3> start{1, 1, 0};
  // static constexpr Eigen::array<index_t, 3> girth{h - 2, w - 2, c};
  // return src.slice(start, girth).stride(stride);

  typedef Eigen::TensorFixedSize<int16_t, Eigen::Sizes<3>, Eigen::ColMajor, index_t> kernel_t;
  static const kernel_t kernel{kernel_t().setValues({_w_p, _w_c, _w_p})};
  // typedef Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<1>, Eigen::ColMajor, index_t> axis_t;
  // static const axis_t   vertical{axis_t().setConstant(0)};
  // static const axis_t horizontal{axis_t().setConstant(0)};
  static constexpr Eigen::array<index_t, 3> patch_y{3, 1, 1};
  // static constexpr Eigen::array<index_t, 3> patch_x{1, 3, 1};
  static constexpr Eigen::array<index_t, 2> shape_y1{3, (h - 2) * w * c};
  static constexpr Eigen::array<index_t, 3> shape_y2{h - 2, w, c};
  // static constexpr Eigen::array<index_t, 3> stride_y{2, 1, 1};
  // static constexpr Eigen::array<index_t, 3> stride_x{1, 2, 1};
  static constexpr Eigen::array<Eigen::IndexPair<index_t>, 1> trans{{Eigen::IndexPair<index_t>(0, 0)}};
  Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<h - 2, w, c>, Eigen::ColMajor, index_t> vertical{(src
    .extract_patches(patch_y)   // 3,1,1,(h-2)wc
    .reshape(shape_y1)          // 3,    (h-2)wc
    .template cast<int16_t>()
    .contract(kernel, trans)    // 1,    (h-2)wc
    >> dbits)
    .template cast<uint8_t>()
    .reshape(shape_y2)          // h-2,w,c
  // //   .stride(stride_y)           // <hh, w, c>
  };
  // Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<h - 2, w, c>, Eigen::ColMajor, index_t> squeeze_v{src.convolve(kernel, vertical)};
  // Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<3, 1, 1,  (h - 2)        * w * c>, Eigen::ColMajor, index_t> _b{src.extract_patches(patch_y)}; // <3, 1, 1, (h-2)wc>
  // Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<3,        (h - 2)        * w * c>, Eigen::ColMajor, index_t> _c{_b.reshape(shape_y1)};         // <3, (h-2)wc>
  // Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<1,        (h - 2)        * w * c>, Eigen::ColMajor, index_t> _d{kernel.contract(_c, mmul)};    // <1, (h-2)wc>
  // Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<           h - 2,          w,  c>, Eigen::ColMajor, index_t> _e{_d.reshape(shape_y2)};         // <h-2, w, c>
  // Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<         ((h - 2) >> 1),   w,  c>, Eigen::ColMajor, index_t> _f{_e.stride(stride_y)};          // <hh, w, c>
  // Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<         ((h - 2) >> 1),   w,  c>, Eigen::ColMajor, index_t> vertical{_f};
  util::write(vertical, "vertical.png");
  // std::cout << src.template cast<int16_t>();
  std::cout << vertical.template cast<int16_t>();
  // std::cout << squeeze_v.template cast<int16_t>();
  return {};
}



} // namespace conv

#endif // CONV_HPP_
