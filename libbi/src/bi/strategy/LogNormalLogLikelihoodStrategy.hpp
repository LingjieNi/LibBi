/**
 * @file
 *
 * @author Lawrence Murray <lawrence.murray@csiro.au>
 * $Rev$
 * $Date$
 */
#ifndef BI_STRATEGY_LOGNORMALLOGLIKELIHOODSTRATEGY_HPP
#define BI_STRATEGY_LOGNORMALLOGLIKELIHOODSTRATEGY_HPP

#include "../traits/likelihood_traits.hpp"
#include "../math/pi.hpp"
#include "../state/Coord.hpp"

namespace bi {
/**
 * @internal
 *
 * Strategy for log-normal log-likelihood calculations, used by nodes with
 * #IS_LOG_NORMAL_LIKELIHOOD trait.
 *
 * @ingroup method_strategy
 *
 * @copydoc LogLikelihoodStrategy
 *
 * Note the following optimisations based on the traits of the node:
 *
 * @li If one or both of #HAS_ZERO_MU or #HAS_UNIT_SIGMA is true, fewer
 * operations need be performed in calculating the likelihood.
 */
template<class X, class V1, class V2, class V3>
class LogNormalLogLikelihoodStrategy {
public:
  /**
   * @copydoc LogLikelihoodStrategy::ll()
   */
  static CUDA_FUNC_BOTH void ll(const Coord& cox, const V1& pax, const V2& y,
      V3& ll) {
    real mu, sigma;

    if (has_zero_mu<X>::value) {
      if (has_unit_sigma<X>::value) {
        ll = REAL(-0.5)*CUDA_POW(CUDA_LOG(y),2);
        ll -= REAL(BI_HALF_LOG_TWO_PI) + CUDA_LOG(y); // normalise
      } else {
        X::sigma(cox, pax, sigma);
        ll = REAL(-0.5)*CUDA_POW(CUDA_LOG(y),2)*CUDA_POW(sigma,-2);
        ll -= REAL(BI_HALF_LOG_TWO_PI) + CUDA_LOG(sigma) + CUDA_LOG(y); // normalise
      }
    } else {
      X::mu(cox, pax, mu);
      if (has_unit_sigma<X>::value) {
        ll = REAL(-0.5)*CUDA_POW(CUDA_LOG(y) - mu,2);
        ll -= REAL(BI_HALF_LOG_TWO_PI) + CUDA_LOG(y); // normalise
      } else {
        X::sigma(cox, pax, sigma);
        ll = REAL(-0.5)*CUDA_POW(CUDA_LOG(y) - mu,2)*CUDA_POW(sigma,-2);
        ll -= REAL(BI_HALF_LOG_TWO_PI) + CUDA_LOG(sigma) + CUDA_LOG(y); // normalise
      }
    }
  }

};

}

#endif