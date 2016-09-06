/*!
 * \file   zmp_runner.h
 * \author Alexander Winkler
 * \date   Oct 4, 2014
 * \brief  SL Task that executes a walking gait given a arbitrary sequence and
 *         position of footholds.
 *
 *         It uses the dynamic locomotion library \c xpp to find an optimal and
 *         dynamicaly stable body trajectory and executes this trjaectory using
 *         inverse dynamics coupled with a virtual model controller.
 */

#ifndef IIT_ZMP_RUNNER_H_
#define IIT_ZMP_RUNNER_H_

#include <xpp_controller/controller.h>
#include "virtual_model-inl.h"
#include "walking_controller_state.h"
#include <xpp/hyq/hyq_spliner.h>

#include <xpp_opt/OptimizedParametersNlp.h>
#include <xpp_opt/RequiredInfoNlp.h>
#include <iit/robots/hyq/declarations.h>
#include <iit/robots/hyq/inertia_properties.h>
#include <iit/robots/hyq/jsim.h>
#include <iit/robots/hyq/transforms.h>

#include <Eigen/src/Core/Matrix.h>
#include <ros/publisher.h>
#include <ros/subscriber.h>
#include <vector>

namespace xpp {
namespace exe {


class WalkingController : public Controller {
public:
  typedef Eigen::Vector3d Vector3d;
  typedef iit::HyQ::JointState JointState;
  typedef xpp::hyq::HyqState HyqState;
  typedef xpp::hyq::HyqSpliner HyqSpliner;
  typedef xpp::hyq::Foothold Foothold;
  typedef std::vector<xpp::hyq::Foothold> VecFoothold;
  typedef xpp::hyq::VirtualModel VirtualModel;
  template<typename T> using LegDataMap = xpp::hyq::LegDataMap<T>;
  typedef xpp::hyq::LegID LegID;
  typedef xpp::utils::Point3d State;
  typedef xpp::utils::Orientation Orientation;
  using VecSpline = HyqSpliner::VecZmpSpline;
  // ROS stuff
  typedef xpp_opt::RequiredInfoNlp ReqInfoMsg;
  typedef xpp_opt::OptimizedParametersNlp OptimizedParametersMsg;

  explicit WalkingController();
  virtual ~WalkingController();

  void SetState(WalkingControllerState::State state);
  // fsm callable functions
  void PublishCurrentState();
  void IntegrateOptimizedTrajectory();
  void ExecuteLoop();
  void EstimateCurrPose();
  bool SwitchToNewTrajectory();
  bool IsTimeToSendOutState() const;
  void PublishOptimizationStartState(); // sends out command to start NLP optimization



private:
//  void AddVarForLogging();

  void GetReadyHook() override;
  bool RunHook() override;

  WalkingControllerState::State current_state_;
  WalkingControllerState::StatesMap states_map_;

  void OptParamsCallback(const OptimizedParametersMsg& msg);
  ::ros::Publisher current_info_pub_;
  ::ros::Subscriber opt_params_sub_;

//  /** Estimates where the robot will be when optimization is complete in order
//    * to start optimization from there.
//    *
//    * @param required_time
//    * @return
//    */
//  State GetStartStateForOptimization(/*const double required_time*/) const;

  bool reoptimize_before_finish_;
  bool first_run_after_integrating_opt_trajectory_;

  VecSpline opt_splines_;
  VecFoothold opt_footholds_;
  bool optimal_trajectory_updated;

  HyqSpliner spliner_;  //for normal body, ori, and feet traj.
  HyqState P_des_;
  HyqState P_curr_;

  // some hacky stuff
  State prev_state_;
  double t_stance_initial_;
  double t_swing_;
  double robot_height_;
  double max_cpu_time_;

  hyq::SplineNode switch_node_;
  double kOptTimeReq_;


  void SmoothTorquesAtContactChange(JointState& uff);
  LegDataMap<bool> prev_swingleg_;
  bool ffsplining_;
  double ffspliner_timer_;
  double ffspline_duration_;
  JointState uff_prev_;
  Eigen::Vector3d b_r_geomtocog; // tranform from geometric body center to center of gravity

  bool use_virtual_model_;
  VirtualModel vm_;
  iit::HyQ::ForceTransforms force_transforms_;
  iit::HyQ::dyn::InertiaProperties inertia_properties_;
  iit::HyQ::dyn::JSIM jsim_; // Joint-Space Inertia Matrix


  Eigen::Vector3d TransformBaseToProjectedFrame(const Eigen::Vector3d& B_r_btox,
                                                const xpp::utils::Pose& P_base_BtoP) const;
};


} // namespace exe
} // namespace iit

#endif /* ZMP_ZMP_RUNNER */
