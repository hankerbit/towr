/**
 @file    spline_junction_equation.h
 @author  Alexander W. Winkler (winklera@ethz.ch)
 @date    May 26, 2016
 @brief   Brief description
 */

#ifndef USER_TASK_DEPENDS_XPP_OPT_INCLUDE_XPP_ZMP_SPLINE_JUNCTION_EQUATION_H_
#define USER_TASK_DEPENDS_XPP_OPT_INCLUDE_XPP_ZMP_SPLINE_JUNCTION_EQUATION_H_

#include <xpp/zmp/i_linear_equation_builder.h>
#include <xpp/zmp/continuous_spline_container.h>

namespace xpp {
namespace zmp {

class SplineJunctionEquation : public ILinearEquationBuilder {
public:
  SplineJunctionEquation (const ContinuousSplineContainer&);
  virtual ~SplineJunctionEquation () { }

  MatVec BuildLinearEquation() const override;

private:
  const ContinuousSplineContainer splines_;
};

} /* namespace zmp */
} /* namespace xpp */

#endif /* USER_TASK_DEPENDS_XPP_OPT_INCLUDE_XPP_ZMP_SPLINE_JUNCTION_EQUATION_H_ */
