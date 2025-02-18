# **************************************************************************
# * Copyright(c) 1998-2014, ALICE Experiment at CERN, All rights reserved. *
# *                                                                        *
# * Author: The ALICE Off-line Project.                                    *
# * Contributors are mentioned in the code where appropriate.              *
# *                                                                        *
# * Permission to use, copy, modify and distribute this software and its   *
# * documentation strictly for non-commercial purposes is hereby granted   *
# * without fee, provided that the above copyright notice appears in all   *
# * copies and that both the copyright notice and this permission notice   *
# * appear in the supporting documentation. The authors make no claims     *
# * about the suitability of this software for any purpose. It is          *
# * provided "as is" without express or implied warranty.                  *
# **************************************************************************/

#Module
set(MODULE PWGLFnuclex)
add_definitions(-D_MODULE_="${MODULE}")

# Module include folder
include_directories(${AliPhysics_SOURCE_DIR}/PWGLF/NUCLEX
                    ${AliPhysics_SOURCE_DIR}/PWGLF/NUCLEX/Utils/RecoDecay
                    ${AliPhysics_SOURCE_DIR}/PWGLF/NUCLEX/Utils/NuclexFilter
)

# Additional includes - alphabetical order except ROOT
include_directories(${ROOT_INCLUDE_DIRS}
                    ${ALIROOT_INCLUDE_DIRS}
                    ${AliPhysics_SOURCE_DIR}/OADB
                    ${AliPhysics_SOURCE_DIR}/OADB/COMMON/MULTIPLICITY
		                ${AliPhysics_SOURCE_DIR}/PWGCF/Correlations/Base
                    ${AliPhysics_SOURCE_DIR}/PWGPP/EVCHAR/FlowVectorCorrections/QnCorrections
		                ${AliPhysics_SOURCE_DIR}/PWGPP/EVCHAR/FlowVectorCorrections/QnCorrectionsInterface/
  )
set(SRCS
  Exotica/Hdibaryon/AliAnalysisTaskHdibaryonLPpi.cxx
  Exotica/LambdaN/AliAnalysisTaskLambdaNAOD.cxx
  Exotica/LambdaNN/AliAnalysisTaskLNNntuple.cxx
  Exotica/LambdaNN/AliAnalysisTaskLNNv0Bkg.cxx
  Exotica/Ps/AliAnalysisTaskPsEfficiency.cxx
  Exotica/dStar/AliAnalysisTaskdStar.cxx
  Hypernuclei/Hyp2Body/AliAnalysisTaskHelium3Pi.cxx
  Hypernuclei/Hyp2Body/AliAnalysisTaskHelium3PiAOD.cxx
  Hypernuclei/Hyp2Body/AliAnalysisTaskHelium3PiMC.cxx
  Hypernuclei/Hyp2Body/AliAnalysisTaskHypTritEventTree.cxx
  Hypernuclei/Hyp2Body/AliReducedHypTritEvent.cxx
  Hypernuclei/Hyp2Body/AliAnalysisTaskHypCrossCheck.cxx
  Hypernuclei/Hyp2Body/AliAnalysisTaskHyperTriton2He3piML.cxx
	Hypernuclei/Hyp3Body/AliAnalysisTaskFindableHypertriton3.cxx
  Hypernuclei/Hyp3Body/AliAnalysisTaskHypertriton3.cxx
  Hypernuclei/Hyp3Body/AliAnalysisTaskHypertriton3ML.cxx
  Hypernuclei/Vertexer2Body/AliVertexerHyperTriton2Body.cxx
  Hypernuclei/Vertexer3Body/AliVertexerHyperTriton3Body.cxx
  Nuclei/Absorption/AliAnalysisTaskDeuteronAbsorption.cxx
  Nuclei/AbsorptionRatios/AliAnalysisTaskLightN.cxx
  Nuclei/AbsorptionRatios/AliLightNEventHist.cxx
  Nuclei/AbsorptionRatios/AliLightNAnalysis.cxx
  Nuclei/AbsorptionRatios/AliLightNTrack.cxx
  Nuclei/AbsorptionRatios/AliLightNBasePart.cxx
  Nuclei/AbsorptionRatios/AliLightNTrackCuts.cxx
  Nuclei/AbsorptionRatios/AliLightNEvent.cxx
  Nuclei/AbsorptionRatios/AliLightNTrackHist.cxx
  Nuclei/AbsorptionRatios/AliLightNEventCuts.cxx
  Nuclei/AbsorptionRatios/AliLightNTrackMCHist.cxx
  Nuclei/DeltaMasses/AliAnalysisNucleiMass.cxx
  Nuclei/DeltaMasses/AliAnalysisNuclMult.cxx
  Nuclei/He4/AliAnalysisTaskAntiHe4.cxx
  Nuclei/He4pp/AliAnalysisHe4.cxx
  Nuclei/NucleiPbPb/AliAnalysisTaskNucleiYield.cxx
  Nuclei/NucleiPbPb/AliAnalysisTaskNucleiPIDqa.cxx
  Nuclei/NucleiPbPb/AliAnalysisTaskSignalLoss.cxx
  Nuclei/NucleiReducedTree/AliAnalysisTaskReducedTreeNuclei.cxx
  Nuclei/TritonAnalysis/AliAnalysisTaskTritonVsMultiplicity_PbPb.cxx
  Nuclei/v2/AliAnalysisTaskNucleiv2.cxx
  Nuclei/v2/AliAnalysisTaskNucleiv2SP.cxx
  Nuclei/v2/AliAnalysisTaskNucleiv2pPb.cxx
  Nuclei/v2/AliAnalysisTaskAllPtcv2.cxx
  Nuclei/NucleiKine/AliAnalysisTaskNucleiKine.cxx
  Nuclei/NucleiKine/AliAnalysisTaskNucleiKineCor.cxx
  Utils/CODEX/AliAnalysisCODEX.cxx
  Utils/CODEX/AliAnalysisCODEXtask.cxx
  Utils/NanoAOD/AliNanoFilterPID.cxx
  Utils/NanoAOD/AliNanoSkimmingPID.cxx
  Utils/ChunkFilter/AliAnalysisTaskFilterHe3.cxx
  )

if(ROOT_VERSION_MAJOR EQUAL 6)
	 set(SRCS
		  ${SRCS}
				  Hypernuclei/Hyp3Body/selector/AliSelectorFindableHyperTriton3Body.cxx
						  )
	  endif()

# Headers from sources
string(REPLACE ".cxx" ".h" HDRS "${SRCS}")

# Generate the dictionary
# It will create G_ARG1.cxx and G_ARG1.h / ARG1 = function first argument
get_directory_property(incdirs INCLUDE_DIRECTORIES)
generate_dictionary("${MODULE}" "${MODULE}LinkDef.h" "${HDRS}" "${incdirs}")

set(ROOT_DEPENDENCIES)
set(ALIROOT_DEPENDENCIES ANALYSISalice OADB PWGPPevcharQn PWGflowTasks EVGEN PWGLFcommonutils)

# Generate the ROOT map
# Dependecies
set(LIBDEPS ${ALIROOT_DEPENDENCIES} ${ROOT_DEPENDENCIES})
generate_rootmap("${MODULE}" "${LIBDEPS}" "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE}LinkDef.h")

# Generate a PARfile target for this library. Note the extra includes ("Tracks UserTasks")
add_target_parfile(${MODULE} "${SRCS}" "${HDRS}" "${MODULE}LinkDef.h" "${LIBDEPS}" "Tracks UserTasks")

# Create an object to be reused in case of static libraries
# Otherwise the sources will be compiled twice
add_library(${MODULE}-object OBJECT ${SRCS} G__${MODULE}.cxx)

# Add a library to the project using the object
add_library_tested(${MODULE} SHARED $<TARGET_OBJECTS:${MODULE}-object>)
target_link_libraries(${MODULE} ${ALIROOT_DEPENDENCIES} ${ROOT_DEPENDENCIES})

# Setting the correct headers for the object as gathered from the dependencies
target_include_directories(${MODULE}-object PUBLIC $<TARGET_PROPERTY:${MODULE},INCLUDE_DIRECTORIES>)
set_target_properties(${MODULE}-object PROPERTIES COMPILE_DEFINITIONS $<TARGET_PROPERTY:${MODULE},COMPILE_DEFINITIONS>)

# Public include folders that will be propagated to the dependecies
target_include_directories(${MODULE} PUBLIC ${incdirs})

set(MODULE_COMPILE_FLAGS)
set(MODULE_LINK_FLAGS)

# Additional compilation and linking flags
set(MODULE_COMPILE_FLAGS " ${MODULE_COMPILE_FLAGS}")

# System dependent: Modify the way the library is build
if(${CMAKE_SYSTEM} MATCHES Darwin)
    set(MODULE_LINK_FLAGS "-undefined dynamic_lookup ${MODULE_LINK_FLAGS}")
endif(${CMAKE_SYSTEM} MATCHES Darwin)

# Setting compilation flags for the object
set_target_properties(${MODULE}-object PROPERTIES COMPILE_FLAGS "${MODULE_COMPILE_FLAGS}")
# Setting the linking flags for the library
set_target_properties(${MODULE} PROPERTIES LINK_FLAGS "${MODULE_LINK_FLAGS}")

# Installation
install(TARGETS ${MODULE}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)

install(FILES ${HDRS} DESTINATION include)

# install macros
install(DIRECTORY . DESTINATION PWGLF/NUCLEX FILES_MATCHING PATTERN "*.C")
