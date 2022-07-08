#include "util.hpp"
#include "conv.hpp"

#include <iostream>
#include <unsupported/Eigen/CXX11/TensorSymmetry>



static constexpr kIndexType kImageHeight = 756;
static constexpr kIndexType kImageWidth = 622;
typedef Eigen::Sizes<kImageHeight, kImageWidth, 3> kImageSize;
typedef Eigen::TensorFixedSize<uint8_t, kImageSize, Eigen::RowMajor, kIndexType> ImageRowMajor;
typedef Eigen::TensorFixedSize<uint8_t, kImageSize, Eigen::ColMajor, kIndexType> ImageColMajor;



int main() {
  cv::Mat const src = cv::imread("images/alias_bricks.jpg");
  ImageRowMajor rm{util::wrap<kImageHeight, kImageWidth>(src)};
  ImageColMajor cm{util::col_major(rm)};
  util::write(cm, "images/poop.png");
  // util::write(conv::gauss_half(cm), "shit.png");
  return 0;
}
