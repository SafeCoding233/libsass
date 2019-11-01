#ifndef SASS_AST_SUPPORTS_H
#define SASS_AST_SUPPORTS_H

// sass.hpp must go before all system headers to get the
// __EXTENSIONS__ fix on Solaris.
#include "sass.hpp"

#include <set>
#include <deque>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <typeinfo>
#include <algorithm>
#include "sass/base.h"
#include "ast_fwd_decl.hpp"

#include "util.hpp"
#include "units.hpp"
#include "context.hpp"
#include "position.hpp"
#include "constants.hpp"
#include "operation.hpp"
#include "position.hpp"
#include "inspect.hpp"
#include "source_map.hpp"
#include "environment.hpp"
#include "error_handling.hpp"
#include "ast_def_macros.hpp"
#include "ast_fwd_decl.hpp"
#include "source_map.hpp"
#include "fn_utils.hpp"

#include "sass.h"

namespace Sass {

  ////////////////////
  // `@supports` rule.
  ////////////////////
  class SupportsRule : public ParentStatement {
    ADD_PROPERTY(Supports_Condition_Obj, condition)
  public:
    SupportsRule(SourceSpan pstate, Supports_Condition_Obj condition, Block_Obj block = {});
    bool bubbles() override;
    ATTACH_AST_OPERATIONS(SupportsRule)
    ATTACH_CRTP_PERFORM_METHODS()
  };

  //////////////////////////////////////////////////////
  // The abstract superclass of all Supports conditions.
  //////////////////////////////////////////////////////
  class Supports_Condition : public Expression {
  public:
    Supports_Condition(SourceSpan pstate);
    virtual bool needs_parens(Supports_Condition_Obj cond) const { return false; }
    ATTACH_AST_OPERATIONS(Supports_Condition)
    ATTACH_CRTP_PERFORM_METHODS()
  };

  ////////////////////////////////////////////////////////////
  // An operator condition (e.g. `CONDITION1 and CONDITION2`).
  ////////////////////////////////////////////////////////////
  class Supports_Operator : public Supports_Condition {
  public:
    enum Operand { AND, OR };
  private:
    ADD_PROPERTY(Supports_Condition_Obj, left);
    ADD_PROPERTY(Supports_Condition_Obj, right);
    ADD_PROPERTY(Operand, operand);
  public:
    Supports_Operator(SourceSpan pstate, Supports_Condition_Obj l, Supports_Condition_Obj r, Operand o);
    virtual bool needs_parens(Supports_Condition_Obj cond) const override;
    ATTACH_AST_OPERATIONS(Supports_Operator)
    ATTACH_CRTP_PERFORM_METHODS()
  };

  //////////////////////////////////////////
  // A negation condition (`not CONDITION`).
  //////////////////////////////////////////
  class Supports_Negation : public Supports_Condition {
  private:
    ADD_PROPERTY(Supports_Condition_Obj, condition);
  public:
    Supports_Negation(SourceSpan pstate, Supports_Condition_Obj c);
    virtual bool needs_parens(Supports_Condition_Obj cond) const override;
    ATTACH_AST_OPERATIONS(Supports_Negation)
    ATTACH_CRTP_PERFORM_METHODS()
  };

  /////////////////////////////////////////////////////
  // A declaration condition (e.g. `(feature: value)`).
  /////////////////////////////////////////////////////
  class Supports_Declaration : public Supports_Condition {
  private:
    ADD_PROPERTY(ExpressionObj, feature);
    ADD_PROPERTY(ExpressionObj, value);
  public:
    Supports_Declaration(SourceSpan pstate, ExpressionObj f, ExpressionObj v);
    virtual bool needs_parens(Supports_Condition_Obj cond) const override;
    ATTACH_AST_OPERATIONS(Supports_Declaration)
    ATTACH_CRTP_PERFORM_METHODS()
  };

  ///////////////////////////////////////////////
  // An interpolation condition (e.g. `#{$var}`).
  ///////////////////////////////////////////////
  class Supports_Interpolation : public Supports_Condition {
  private:
    ADD_PROPERTY(ExpressionObj, value);
  public:
    Supports_Interpolation(SourceSpan pstate, ExpressionObj v);
    virtual bool needs_parens(Supports_Condition_Obj cond) const override;
    ATTACH_AST_OPERATIONS(Supports_Interpolation)
    ATTACH_CRTP_PERFORM_METHODS()
  };

}

#endif
