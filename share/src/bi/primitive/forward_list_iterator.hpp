/**
 * @file
 *
 * @author Lawrence Murray <lawrence.murray@csiro.au>
 * $Rev$
 * $Date$
 */
#ifndef BI_PRIMITIVE_FORWARDLISTCONSTITERATOR_HPP
#define BI_PRIMITIVE_FORWARDLISTCONSTITERATOR_HPP

#include "forward_list_const_iterator.hpp"

namespace bi {
template<class T> class forward_list;
template<class T> class forward_list_const_iterator;

/**
 * Iterator for forward_list.
 *
 * @tparam T Value type.
 */
template<class T>
class forward_list_iterator : public forward_list_const_iterator<T> {
  friend class forward_list<T>;
  friend class forward_list_const_iterator<T>;
public:
  forward_list_iterator();
  explicit forward_list_iterator(boost::shared_ptr<forward_list_node<T> >& node);
  ~forward_list_iterator();
  T& operator*();
  T* operator->();
};
}

template<class T>
bi::forward_list_iterator<T>::forward_list_iterator() {
  //
}

template<class T>
bi::forward_list_iterator<T>::forward_list_iterator(
    boost::shared_ptr<forward_list_node<T> >& node) :
    forward_list_const_iterator<T>(node) {
  //
}

template<class T>
inline T& bi::forward_list_iterator<T>::operator*() {
  return this->node->value;
}

template<class T>
inline T* bi::forward_list_iterator<T>::operator->() {
  return &this->node->value;
}

#endif
