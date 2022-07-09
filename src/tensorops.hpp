// No #include guards; see https://eigen.tuxfamily.org/dox/TopicCustomizing_Plugins.html

EIGEN_DEVICE_FUNC INLINE
const TensorCwiseUnaryOp<internal::bind2nd_op<scalar_promote_op>, Derived const> promote(uint8_t bits) const {
  static_assert(std::is_same<uint8_t, Scalar>::value, "For now, we can only promote uint8_t to int16_t");
  return this->unaryExpr(internal::bind2nd_op<scalar_promote_op>(bits));
}

EIGEN_DEVICE_FUNC INLINE
const TensorCwiseUnaryOp<internal::bind2nd_op<scalar_demote_op>, Derived const> demote(uint8_t bits) const {
  static_assert(std::is_same<int16_t, Scalar>::value, "For now, we can only demote int16_t to uint8_t");
  return this->unaryExpr(internal::bind2nd_op<scalar_demote_op>(bits));
}
