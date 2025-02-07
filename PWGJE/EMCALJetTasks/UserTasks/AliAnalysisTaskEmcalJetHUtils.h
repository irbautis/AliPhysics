#ifndef ALIANALYSISTASKEMCALJETHUTILS_H
#define ALIANALYSISTASKEMCALJETHUTILS_H

/**
 * @class AliAnalysisTaskEmcalJetHUtils
 * @brief Jet-hadron correlations utilities class
 *
 * Contains funtionality that is shared between the various classes. Could have been
 * a namespace except it wouldn't play nice with ROOT.
 *
 * @author Raymond Ehlers <raymond.ehlers@cern.ch>, Yale University
 * @date 23 Feb 2018
 */

#include <string>
#include <map>

#include "AliEventCuts.h"
#include "AliYAMLConfiguration.h"
class AliEmcalJet;
class AliEmcalContainer;
class AliParticleContainer;
class AliTrackContainer;
class AliClusterContainer;

namespace PWGJE {
namespace EMCALJetTasks {

class AliAnalysisTaskEmcalJetHUtils {
 public:
  /**
   * @enum ELeadingHadronBiasType_t
   * @brief Determine the jet leading hadron bias type
   */
  enum ELeadingHadronBiasType_t {
    kCharged = 0,   //!<! Charged hadron bias
    kNeutral = 1,   //!<! Neutral hadron bias
    kBoth = 2       //!<! Leading is from the leader of both
  };
  static const std::map<std::string, ELeadingHadronBiasType_t> fgkLeadingHadronBiasMap; //!<! Map from name to leading hadron bias used with the YAML config
  static double GetLeadingHadronPt(AliEmcalJet * jet, ELeadingHadronBiasType_t leadingHadronType);

  static double RelativeEPAngle(double jetAngle, double epAngle);

  static void ConfigureEventCuts(AliEventCuts & eventCuts, PWG::Tools::AliYAMLConfiguration & yamlConfig, const UInt_t offlineTriggerMask, const std::string & baseName, const std::string & taskName);
  static AliParticleContainer * CreateParticleOrTrackContainer(const std::string & collectionName);
  static void ConfigureEMCalContainersFromYAMLConfig(std::vector<std::string> baseName,
                            std::string containerName,
                            AliEmcalContainer* cont,
                            PWG::Tools::AliYAMLConfiguration& yamlConfig,
                            std::string taskName);
  static void ConfigureTrackContainersFromYAMLConfig(std::vector<std::string> baseNameWithContainer,
                            AliTrackContainer* trackCont,
                            PWG::Tools::AliYAMLConfiguration& yamlConfig,
                            std::string taskName);
  static void ConfigureClusterContainersFromYAMLConfig(std::vector<std::string> baseNameWithContainer,
                             AliClusterContainer* clusterCont,
                             PWG::Tools::AliYAMLConfiguration& yamlConfig,
                             std::string taskName);

  static double GetJetPt(const AliEmcalJet * jet, const double rho);
};

} /* namespace EMCALJetTasks */
} /* namespace PWGJE */

#endif /* AliAnalysisTaskEmcalJetHUtils.h */
