#ifndef BITSHIFT_HPP_
#define BITSHIFT_HPP_

#include <Eigen/Core>



struct scalar_promote_op : Eigen::internal::binary_op_base<int16_t, uint8_t> {
  typedef uint8_t result_type;
#ifdef EIGEN_SCALAR_BINARY_OP_PLUGIN
  scalar_promote_op() {
    EIGEN_SCALAR_BINARY_OP_PLUGIN
  }
#endif
  EIGEN_DEVICE_FUNC INLINE result_type operator()(int16_t a, uint8_t bits) const { return a >> bits; }
};

// From scalar_sum_op
template <>
struct Eigen::internal::functor_traits<scalar_promote_op> {
  enum {
    Cost = (int(NumTraits<int16_t>::AddCost) + int(NumTraits<uint8_t>::AddCost)) >> 1,
    PacketAccess = false
  };
};



struct scalar_demote_op : Eigen::internal::binary_op_base<uint8_t, int16_t> {
  typedef int16_t result_type;
#ifdef EIGEN_SCALAR_BINARY_OP_PLUGIN
  scalar_demote_op() {
    EIGEN_SCALAR_BINARY_OP_PLUGIN
  }
#endif
  EIGEN_DEVICE_FUNC INLINE result_type operator()(uint8_t a, uint8_t bits) const { return a << bits; }
};

// From scalar_sum_op
template <>
struct Eigen::internal::functor_traits<scalar_demote_op> {
  enum {
    Cost = (int(NumTraits<uint8_t>::AddCost) + int(NumTraits<int16_t>::AddCost)) >> 1,
    PacketAccess = false
  };
};



#endif // BITSHIFT_HPP_
