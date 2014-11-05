/**
 * @file
 *
 * @author Lawrence Murray <lawrence.murray@csirexpr.au>
 * $Rev$
 * $Date$
 */
#ifndef BI_PROGRAM_FUNCTIONOVERLOAD_HPP
#define BI_PROGRAM_FUNCTIONOVERLOAD_HPP

#include "Named.hpp"
#include "Parenthesised.hpp"
#include "Braced.hpp"
#include "Scoped.hpp"

namespace biprog {
/**
 * FunctionOverload.
 *
 * @ingroup program
 */
class FunctionOverload: public virtual Named,
    public virtual Parenthesised,
    public virtual Braced,
    public virtual Scoped,
    public boost::enable_shared_from_this<FunctionOverload> {
public:
  /**
   * Constructor.
   */
  FunctionOverload(const char* name, boost::shared_ptr<Expression> parens,
      boost::shared_ptr<Expression> braces, boost::shared_ptr<Scope> scope);

  /**
   * Destructor.
   */
  virtual ~FunctionOverload();

  /*
   * Operators.
   */
  virtual bool operator<(const Expression& o) const;
  virtual bool operator<=(const Expression& o) const;
  virtual bool operator>(const Expression& o) const;
  virtual bool operator>=(const Expression& o) const;
  virtual bool operator==(const Expression& o) const;
  virtual bool operator!=(const Expression& o) const;

protected:
  /**
   * Output.
   */
  virtual void output(std::ostream& out) const;
};
}

inline biprog::FunctionOverload::FunctionOverload(const char* name,
    boost::shared_ptr<Expression> parens,
    boost::shared_ptr<Expression> braces, boost::shared_ptr<Scope> scope) :
    Named(name), Parenthesised(parens), Braced(braces), Scoped(scope) {
  //
}

inline biprog::FunctionOverload::~FunctionOverload() {
  //
}

#endif
