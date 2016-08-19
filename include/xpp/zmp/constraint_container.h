/**
 @file    constraint_container.h
 @author  Alexander W. Winkler (winklera@ethz.ch)
 @date    May 30, 2016
 @brief   Provides a class to combine knowledge of individual constraints.
 */

#ifndef USER_TASK_DEPENDS_XPP_OPT_INCLUDE_XPP_ZMP_CONSTRAINT_CONTAINER_H_
#define USER_TASK_DEPENDS_XPP_OPT_INCLUDE_XPP_ZMP_CONSTRAINT_CONTAINER_H_

#include <xpp/zmp/a_constraint.h>
#include <xpp/zmp/i_observer.h>
#include <xpp/zmp/optimization_variables.h>

#include <memory>

namespace xpp {
namespace zmp {

/** @brief Knows about all constraints and gives information about them.
  *
  * For every constraint that \c ConstraintContainer knows about, it will return
  * the constraint violations and the acceptable bounds. It also maintains a
  * connection to the optimization variables, and constantly keeps up-to-date
  * values of these (observer).
  */
class ConstraintContainer : public IObserver {
public:
  typedef AConstraint::VectorXd VectorXd;
  typedef AConstraint::VecBound VecBound;
  typedef AConstraint::Jacobian Jacobian;
  typedef std::shared_ptr<AConstraint> ConstraintPtr;
  typedef std::shared_ptr<Jacobian> JacobianPtr;;

  ConstraintContainer (OptimizationVariables& subject);
  virtual ~ConstraintContainer ();

  void Update () override;
  void ClearConstraints();

  void AddConstraint (ConstraintPtr constraint);

  VectorXd EvaluateConstraints () const;
  JacobianPtr GetJacobian () const;
  VecBound GetBounds () const;

private:
  void RefreshBounds ();
  std::vector<ConstraintPtr> constraints_;
  VecBound bounds_;
  JacobianPtr jacobian_;
};

} /* namespace zmp */
} /* namespace xpp */

#endif /* USER_TASK_DEPENDS_XPP_OPT_INCLUDE_XPP_ZMP_CONSTRAINT_CONTAINER_H_ */
