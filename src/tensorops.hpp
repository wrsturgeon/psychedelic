// No #include guards; see https://eigen.tuxfamily.org/dox/TopicCustomizing_Plugins.html

EIGEN_DEVICE_FUNC INLINE
Eigen::TensorCwiseUnaryOp<Eigen::internal::bind2nd_op<scalar_promote_op>, Derived const> const promote() const {
  static_assert(std::is_same<uint8_t, Scalar>::value, "For now, we can only promote uint8_t to int16_t");
  return unaryExpr(Eigen::internal::bind2nd_op<scalar_promote_op>());
}

EIGEN_DEVICE_FUNC INLINE
Eigen::TensorCwiseUnaryOp<Eigen::internal::bind2nd_op<scalar_demote_op>, Derived const> const demote() const {
  static_assert(std::is_same<int16_t, Scalar>::value, "For now, we can only demote int16_t to uint8_t");
  return unaryExpr(Eigen::internal::bind2nd_op<scalar_demote_op>());
}
