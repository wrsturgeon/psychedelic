#ifndef BITSHIFT_HPP_
#define BITSHIFT_HPP_

#include <unsupported/Eigen/CXX11/TensorSymmetry>



struct scalar_rshift_op : Eigen::internal::binary_op_base<int16_t, uint8_t> {
  typedef uint8_t result_type;
#ifdef EIGEN_SCALAR_BINARY_OP_PLUGIN
  scalar_rshift_op() {
    EIGEN_SCALAR_BINARY_OP_PLUGIN
  }
#endif
  EIGEN_DEVICE_FUNC INLINE result_type operator() (const int16_t& a, uint8_t b) const { return a >> b; }
  // template<typename Packet>
  // EIGEN_DEVICE_FUNC INLINE Packet packetOp(const Packet& a, const Packet& b) const
  // { return internal::padd(a,b); }
  // template<typename Packet>
  // EIGEN_DEVICE_FUNC INLINE dtype predux(const Packet& a) const
  // { return internal::predux(a); }
};

// From scalar_sum_op
template <>
struct Eigen::internal::functor_traits<scalar_rshift_op> {
  enum {
    Cost = (int(NumTraits<int16_t>::AddCost) + int(NumTraits<uint8_t>::AddCost)) >> 1,
    PacketAccess = false
  };
};



// EIGEN_DEVICE_FUNC
// INLINE const TensorCwiseUnaryOp<internal::bind2nd_op<internal::scalar_sum_op<Scalar,Scalar>>, const Derived>
// operator+ (Scalar rhs) const {
//   return unaryExpr(internal::bind2nd_op<internal::scalar_sum_op<Scalar,Scalar>>(rhs));
// }

template <typename Derived>
EIGEN_DEVICE_FUNC INLINE
Eigen::TensorCwiseUnaryOp<Eigen::internal::bind2nd_op<scalar_rshift_op>, Derived const> const operator>>(Eigen::TensorBase<Derived> const& a, uint8_t b) {
  assert(b == 8);
  return a.unaryExpr(Eigen::internal::bind2nd_op<scalar_rshift_op>(8));
}



#endif // BITSHIFT_HPP_