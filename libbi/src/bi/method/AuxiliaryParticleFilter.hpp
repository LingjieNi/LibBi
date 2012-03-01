/**
 * @file
 *
 * @author Lawrence Murray <lawrence.murray@csiro.au>
 * $Rev$
 * $Date$
 */
#ifndef BI_METHOD_AUXILIARYPARTICLEFILTER_HPP
#define BI_METHOD_AUXILIARYPARTICLEFILTER_HPP

#include "ParticleFilter.hpp"

namespace bi {
/**
 * Auxiliary particle filter with deterministic lookahead.
 *
 * @ingroup method
 *
 * @tparam B Model type.
 * @tparam IO1 #concept::SparseInputBuffer type.
 * @tparam IO2 #concept::SparseInputBuffer type.
 * @tparam IO3 #concept::AuxiliaryParticleFilterBuffer type.
 * @tparam CL Cache location.
 * @tparam SH Static handling.
 *
 * @section Concepts
 *
 * #concept::Filter, #concept::Markable
 */
template<class B, class IO1, class IO2, class IO3, Location CL = ON_HOST,
    StaticHandling SH = STATIC_SHARED>
class AuxiliaryParticleFilter : public ParticleFilter<B,IO1,IO2,IO3,CL,SH> {
public:
  using ParticleFilter<B,IO1,IO2,IO3,CL,SH>::resample;

  /**
   * Constructor.
   *
   * @param m Model.
   * @param rng Random number generator.
   * @param delta Time step for d- and r-nodes.
   * @param in Forcings.
   * @param obs Observations.
   * @param out Output.
   */
  AuxiliaryParticleFilter(B& m, Random& rng, const real delta = 1.0,
      IO1* in = NULL, IO2* obs = NULL, IO3* out = NULL);

  /**
   * @name High-level interface.
   *
   * An easier interface for common usage.
   */
  //@{
  /**
   * @copydoc ParticleFilter::filter()
   */
  template<Location L, class R>
  void filter(const real T, Static<L>& theta, State<L>& s, R* resam = NULL,
      const real relEss = 1.0);

  /**
   * @copydoc ParticleFilter::filter()
   */
  template<Location L, class R, class V1>
  void filter(const real T, const V1 x0, Static<L>& theta, State<L>& s,
      R* resam = NULL, const real relEss = 1.0);

  /**
   * @copydoc ParticleFilter::filter()
   */
  template<Location L, class M1, class R>
  void filter(const real T, Static<L>& theta, State<L>& s, M1& xd, M1& xc,
      M1& xr, R* resam = NULL, const real relEss = 1.0);

  /**
   * @copydoc summarise_apf()
   */
  template<class T1, class V1, class V2>
  void summarise(T1* ll, V1* lls, V2* ess);
  //@}

  /**
   * @name Low-level interface.
   *
   * Largely used by other features of the library or for finer control over
   * performance and behaviour.
   */
  //@{
  /**
   * Initialise.
   *
   * @tparam L Location.
   * @tparam V1 Vector type.
   * @tparam V2 Vector type.
   *
   * @param theta Static state.
   * @param[out] lw1s Stage 1 log-weights.
   * @param[out] lw2s Stage 2 log-weights.
   * @param[out] as Ancestry.
   */
  template<Location L, class V1, class V2>
  void init(Static<L>& theta, V1& lw1s, V1& lw2s, V2& as);

  /**
   * Resample particles.
   *
   * @tparam L Location.
   * @tparam V1 Vector type.
   * @tparam V2 Vector type.
   * @tparam R #concept::Resampler type.
   *
   * @param T Maximum end time for any lookahead.
   * @param theta Static state.
   * @param s State.
   * @param[out] lw1s Stage 1 log-weights.
   * @param[in,out] lw2s On input, current log-weights of particles, on
   * output, stage 2 log-weights.
   * @param[out] as Ancestry after resampling.
   * @param resam Resampler.
   * @param relEss Minimum ESS, as proportion of total number of particles,
   * to trigger resampling.
   *
   * @return True if resampling was performed, false otherwise.
   */
  template<Location L, class V1, class V2, class R>
  bool resample(const real T, Static<L>& theta, State<L>& s, V1& lw1s,
      V1& lw2s, V2& as, R* resam = NULL, const real relEss = 1.0);

  /**
   * Resample particles.
   *
   * @tparam L Location.
   * @tparam V1 Vector type.
   * @tparam V2 Vector type.
   * @tparam R #concept::Resampler type.
   *
   * @param T Maximum end time for any lookahead.
   * @param theta Static state.
   * @param s State.
   * @param a Conditioned ancestor of first particle.
   * @param[out] lw1s Stage 1 log-weights.
   * @param[in,out] lw2s On input, current log-weights of particles, on
   * output, stage 2 log-weights.
   * @param[out] as Ancestry after resampling.
   * @param resam Resampler.
   * @param relEss Minimum ESS, as proportion of total number of particles,
   * to trigger resampling.
   *
   * @return True if resampling was performed, false otherwise.
   */
  template<Location L, class V1, class V2, class R>
  bool resample(const real T, Static<L>& theta, State<L>& s, const int a,
      V1& lw1s, V1& lw2s, V2& as, R* resam = NULL, const real relEss = 1.0);

  /**
   * Output.
   *
   * @tparam L Location.
   * @tparam V1 Vector type.
   * @tparam V2 Vector type.
   *
   * @param k Time index.
   * @param theta Static state.
   * @param s State.
   * @param r 1 if resampling was performed before moving to this time, 0
   * otherwise.
   * @param lw1s Stage 1 log-weights.
   * @param lw2s Stage 2 log-weights.
   * @param Ancestry.
   */
  template<Location L, class V1, class V2>
  void output(const int k, const Static<L>& theta, const State<L>& s,
      const int r, const V1& lw1s, const V1& lw2s, const V2& as);

  /**
   * Flush output caches to file.
   */
  void flush();
  //@}

protected:
  /**
   * Perform lookahead.
   *
   * @param T Maximum end time for lookahead.
   * @param theta Static state.
   * @param s State.
   * @param[in,out] lw1s On input, current log-weights of particles, on
   * output, stage 1 log-weights.
   */
  template<Location L, class V1>
  void lookahead(const real T, Static<L>& theta, State<L>& s, V1& lw1s);

  /**
   * Cache for stage 1 log-weights.
   */
  Cache2D<real> stage1LogWeightsCache;

  /* net sizes, for convenience */
  static const int ND = net_size<B,typename B::DTypeList>::value;
  static const int NC = net_size<B,typename B::CTypeList>::value;
  static const int NR = net_size<B,typename B::RTypeList>::value;
  static const int NP = net_size<B,typename B::PTypeList>::value;
};

/**
 * Factory for creating AuxiliaryParticleFilter objects.
 *
 * @ingroup method
 *
 * @tparam CL Cache location.
 *
 * @see AuxiliaryParticleFilter
 */
template<Location CL = ON_HOST, StaticHandling SH = STATIC_SHARED>
struct AuxiliaryParticleFilterFactory {
  /**
   * Create disturbance particle filter.
   *
   * @return AuxiliaryParticleFilter object. Caller has ownership.
   *
   * @see AuxiliaryParticleFilter::AuxiliaryParticleFilter()
   */
  template<class B, class IO1, class IO2, class IO3>
  static AuxiliaryParticleFilter<B,IO1,IO2,IO3,CL,SH>* create(B& m,
      Random& rng, const real delta = 1.0, IO1* in = NULL, IO2* obs = NULL,
      IO3* out = NULL) {
    return new AuxiliaryParticleFilter<B,IO1,IO2,IO3,CL,SH>(m, rng, delta,
        in, obs, out);
  }
};

}

#include "../math/primitive.hpp"
#include "../math/functor.hpp"

template<class B, class IO1, class IO2, class IO3, bi::Location CL,
    bi::StaticHandling SH>
bi::AuxiliaryParticleFilter<B,IO1,IO2,IO3,CL,SH>::AuxiliaryParticleFilter(
    B& m, Random& rng, const real delta, IO1* in, IO2* obs, IO3* out) :
    ParticleFilter<B,IO1,IO2,IO3,CL,SH>(m, rng, delta, in, obs, out) {
  //
}

template<class B, class IO1, class IO2, class IO3, bi::Location CL,
    bi::StaticHandling SH>
template<bi::Location L, class R>
void bi::AuxiliaryParticleFilter<B,IO1,IO2,IO3,CL,SH>::filter(const real T,
    Static<L>& theta, State<L>& s, R* resam, const real relEss) {
  /* pre-conditions */
  assert (T > this->getTime());
  assert (relEss >= 0.0 && relEss <= 1.0);

  const int P = s.size();
  int n = 0, r = 0;
  typename locatable_temp_vector<L,real>::type lw1s(P), lw2s(P);
  typename locatable_temp_vector<L,int>::type as(P);

  init(theta, lw1s, lw2s, as);
  while (this->getTime() < T) {
    r = resample(T, theta, s, lw1s, lw2s, as, resam, relEss);
    predict(T, theta, s);
    correct(s, lw2s);
    output(n, theta, s, r, lw1s, lw2s, as);
    ++n;
  }
  synchronize();
  term(theta);
}

template<class B, class IO1, class IO2, class IO3, bi::Location CL,
    bi::StaticHandling SH>
template<bi::Location L, class R, class V1>
void bi::AuxiliaryParticleFilter<B,IO1,IO2,IO3,CL,SH>::filter(const real T,
    const V1 x0, Static<L>& theta, State<L>& s, R* resam,
    const real relEss) {
  set_rows(s.get(D_NODE), subrange(x0, 0, ND));
  set_rows(s.get(C_NODE), subrange(x0, ND, NC));
  set_rows(theta.get(P_NODE), subrange(x0, ND + NC, NP));

  filter(T, theta, s, resam, relEss);
}

template<class B, class IO1, class IO2, class IO3, bi::Location CL,
    bi::StaticHandling SH>
template<bi::Location L, class M1, class R>
void bi::AuxiliaryParticleFilter<B,IO1,IO2,IO3,CL,SH>::filter(const real T,
    Static<L>& theta, State<L>& s, M1& xd, M1& xc, M1& xr, R* resam,
    const real relEss) {
  /* pre-condition */
  assert (T > this->state.t);
  assert (relEss >= 0.0 && relEss <= 1.0);
  assert (this->out != NULL);

  int n = 0, r = 0, a = 0;
  BOOST_AUTO(lw1s, host_temp_vector<real>(s.size()));
  BOOST_AUTO(lw2s, host_temp_vector<real>(s.size()));
  BOOST_AUTO(as, host_temp_vector<int>(s.size()));

  init(theta, *lw1s, *lw2s, *as);
  while (this->state.t < T) {
    r = resample(T, theta, s, a, *lw1s, *lw2s, *as, resam, relEss);
    predict(T, theta, s);

    /* overwrite first particle with conditioned particle */
    row(s.get(D_NODE), 0) = column(xd, n);
    row(s.get(C_NODE), 0) = column(xc, n);
    row(s.get(R_NODE), 0) = column(xr, n);

    correct(s, *lw2s);
    output(n, theta, s, r, *lw1s, *lw2s, *as);
    ++n;
  }
  synchronize();
  term(theta);

  delete lw1s;
  delete lw2s;
  delete as;
}

template<class B, class IO1, class IO2, class IO3, bi::Location CL,
    bi::StaticHandling SH>
template<class T1, class V1, class V2>
void bi::AuxiliaryParticleFilter<B,IO1,IO2,IO3,CL,SH>::summarise(T1* ll,
    V1* lls, V2* ess) {
  summarise_apf(stage1LogWeightsCache, this->logWeightsCache, ll, lls, ess);
}

template<class B, class IO1, class IO2, class IO3, bi::Location CL,
    bi::StaticHandling SH>
template<bi::Location L, class V1, class V2>
void bi::AuxiliaryParticleFilter<B,IO1,IO2,IO3,CL,SH>::init(Static<L>& theta,
    V1& lw1s, V1& lw2s, V2& as) {
  /* pre-condition */
  assert (lw2s.size() == as.size());

  ParticleFilter<B,IO1,IO2,IO3,CL,SH>::init(theta, lw1s, as);

  bi::fill(lw2s.begin(), lw2s.end(), 0.0);
}

template<class B, class IO1, class IO2, class IO3, bi::Location CL,
    bi::StaticHandling SH>
template<bi::Location L, class V1, class V2, class R>
bool bi::AuxiliaryParticleFilter<B,IO1,IO2,IO3,CL,SH>::resample(const real T,
    Static<L>& theta, State<L>& s, V1& lw1s, V1& lw2s, V2& as, R* resam,
    const real relEss) {
  /* pre-condition */
  assert (lw1s.size() == lw2s.size());

  bool r = false;
  this->normalise(lw2s);
  if (this->oyUpdater.hasNext()) {
    const real to = this->oyUpdater.getNextTime();
    lw1s = lw2s;
    if (resam != NULL && to > this->state.t) {
      this->lookahead(T, theta, s, lw1s);
      if (relEss >= 1.0 || ess(lw1s) <= s.size()*relEss) {
        resam->resample(lw1s, lw2s, as, theta, s);
        r = true;
      } else {
        lw1s = lw2s;
        bi::sequence(as.begin(), as.end(), 0);
      }

      /* post-condition */
      assert (this->sim.getTime() == this->getTime());
    }
  }
  return r;
}

template<class B, class IO1, class IO2, class IO3, bi::Location CL,
    bi::StaticHandling SH>
template<bi::Location L, class V1, class V2, class R>
bool bi::AuxiliaryParticleFilter<B,IO1,IO2,IO3,CL,SH>::resample(const real T,
    Static<L>& theta, State<L>& s, const int a, V1& lw1s, V1& lw2s, V2& as,
    R* resam, const real relEss) {
  /* pre-condition */
  assert (lw1s.size() == lw2s.size());
  assert (a >= 0 && a < lw1s.size());

  bool r = false;
  this->normalise(lw2s);
  if (this->oyUpdater.hasNext()) {
    const real to = this->oyUpdater.getNextTime();
    lw1s = lw2s;
    if (resam != NULL && to > this->state.t) {
      this->lookahead(T, theta, s, lw1s);
      if (relEss >= 1.0 || ess(lw1s) <= s.size()*relEss) {
        resam->resample(a, lw1s, lw2s, as, theta, s);
        r = true;
      } else {
        lw1s = lw2s;
        bi::sequence(as.begin(), as.end(), 0);
      }

      /* post-condition */
      assert (this->sim.getTime() == this->getTime());
    }
  }
  return r;
}

template<class B, class IO1, class IO2, class IO3, bi::Location CL,
    bi::StaticHandling SH>
template<bi::Location L, class V1, class V2>
void bi::AuxiliaryParticleFilter<B,IO1,IO2,IO3,CL,SH>::output(const int k,
    const Static<L>& theta, const State<L>& s, const int r, const V1& lw1s,
    const V1& lw2s, const V2& as) {
  ParticleFilter<B,IO1,IO2,IO3,CL,SH>::output(k, theta, s, r, lw2s, as);

  if (this->haveOut) {
    stage1LogWeightsCache.put(k, lw1s);
  }
}

template<class B, class IO1, class IO2, class IO3, bi::Location CL,
    bi::StaticHandling SH>
void bi::AuxiliaryParticleFilter<B,IO1,IO2,IO3,CL,SH>::flush() {
  ParticleFilter<B,IO1,IO2,IO3,CL,SH>::flush();

  int p;
  if (this->haveOut) {
    assert (stage1LogWeightsCache.isValid());
    for (p = 0; p < stage1LogWeightsCache.size(); ++p) {
      this->out->writeStage1LogWeights(p, stage1LogWeightsCache.get(p));
    }
    stage1LogWeightsCache.clean();
  }
}

template<class B, class IO1, class IO2, class IO3, bi::Location CL,
    bi::StaticHandling SH>
template<bi::Location L, class V1>
void bi::AuxiliaryParticleFilter<B,IO1,IO2,IO3,CL,SH>::lookahead(
    const real T, Static<L>& theta, State<L>& s, V1& lw1s) {
  typedef typename locatable_temp_vector<L,real>::type temp_vector_type;
  typedef typename locatable_temp_matrix<L,real>::type temp_matrix_type;

  if (this->oyUpdater.hasNext()) {
    const real to = this->oyUpdater.getNextTime();
    if (to <= T) {
      temp_matrix_type X(s.size(), ND + NC + NR);
      real delta = this->sim.getDelta();
      int nupdates = lt_steps(to, delta) - lt_steps(this->state.t, delta);

      /* store current state */
      columns(X, 0, ND) = s.get(D_NODE);
      columns(X, ND, NC) = s.get(C_NODE);
      columns(X, ND + NC, NR) = s.get(R_NODE);
      this->mark();

      /* auxiliary simulation forward */
      BOOST_AUTO(&U, this->rUpdater.buf());
      U.resize(s.size(), NR*nupdates);
      U.clear();
      this->rUpdater.setNext(nupdates);
      this->predict(to, theta, s);
      this->correct(s, lw1s);

      /* restore previous state */
      s.get(D_NODE) = columns(X, 0, ND);
      s.get(C_NODE) = columns(X, ND, NC);
      s.get(R_NODE) = columns(X, ND + NC, NR);
      this->restore();
    }
  }
}

#endif