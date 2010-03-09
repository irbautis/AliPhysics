/*************************************************************************
* Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. *
*                                                                        *
* Author: The ALICE Off-line Project.                                    *
* Contributors are mentioned in the code where appropriate.              *
*                                                                        *
* Permission to use, copy, modify and distribute this software and its   *
* documentation strictly for non-commercial purposes is hereby granted   *
* without fee, provided that the above copyright notice appears in all   *
* copies and that both the copyright notice and this permission notice   *
* appear in the supporting documentation. The authors make no claims     *
* about the suitability of this software for any purpose. It is          *
* provided "as is" without express or implied warranty.                  *
**************************************************************************/

//#####################################################
//#                                                   # 
//#              Class AliDielectronMC                #
//#       Cut Class for Jpsi->e+e- analysis           #
//#                                                   #
//#   by WooJin J. Park, GSI / W.J.Park@gsi.de        #
//#                                                   #
//#####################################################

#include <AliAnalysisManager.h>
#include <AliAODHandler.h>
#include <AliESDInputHandler.h>
#include <AliMCEventHandler.h>
#include <AliMCEvent.h>
#include <AliMCParticle.h>
#include <AliAODMCParticle.h>
#include <AliStack.h>
#include <AliESDEvent.h>
#include <AliESDtrack.h>
#include <AliLog.h>

#include "AliDielectronMC.h"

AliDielectronMC* AliDielectronMC::fgInstance=0x0;

//____________________________________________________________
AliDielectronMC* AliDielectronMC::Instance()
{
  //
  // return pointer to singleton implementation
  //
  if (fgInstance) return fgInstance;
  
  AnalysisType type=kUNSET;
  if (AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler()->IsA()==AliESDInputHandler::Class()) type=kESD;
  else if (AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler()->IsA()==AliAODHandler::Class()) type=kAOD;
  
  fgInstance=new AliDielectronMC(type);
  
  return fgInstance;
}

//____________________________________________________________
AliDielectronMC::AliDielectronMC(AnalysisType type):
  fMCEvent(0x0),
  fStack(0x0),
  fAnaType(type)
{
  //
  // default constructor
  //
}


//____________________________________________________________
AliDielectronMC::~AliDielectronMC()
{
  //
  // default destructor
  //
  
}

//____________________________________________________________
void AliDielectronMC::Initialize()
{
  //
  // initialize MC class
  //
  if (!ConnectMCEvent()) AliError("Initialization of MC object failed!");
}

//____________________________________________________________
Int_t AliDielectronMC::GetNMCTracks()
{
  //
  //  return the number of generated tracks from MC event
  //
  if (!fMCEvent){ AliError("No fMCEvent"); return 0; }
  return fMCEvent->GetNumberOfTracks();
}

//____________________________________________________________
Int_t AliDielectronMC::GetNMCTracksFromStack()
{
  //
  //  return the number of generated tracks from stack
  //
  if (!fStack){ AliError("No fStack"); return -999; }
  return fStack->GetNtrack();
}

//____________________________________________________________
AliVParticle* AliDielectronMC::GetMCTrackFromMCEvent(Int_t _itrk)
{
  //
  // return MC track directly from MC event
  //
  if (!fMCEvent){ AliError("No fMCEvent"); return NULL;}
  AliVParticle * track = fMCEvent->GetTrack(_itrk); //  tracks from MC event
  return track;
}

//____________________________________________________________
Bool_t AliDielectronMC::ConnectMCEvent()
{
  //
  // connect stack object from the mc handler
  //
  AliMCEventHandler* mcHandler = dynamic_cast<AliMCEventHandler*> (AliAnalysisManager::GetAnalysisManager()->GetMCtruthEventHandler());
  if (!mcHandler){ AliError("Could not retrive MC event handler!"); return kFALSE; }
  
  AliMCEvent* mcEvent = mcHandler->MCEvent();
  if (!mcEvent){ AliError("Could not retrieve MC event!"); return kFALSE; }
  fMCEvent = mcEvent;
  
  if (!UpdateStack()) return kFALSE;
  return kTRUE;
}

//____________________________________________________________
Bool_t AliDielectronMC::UpdateStack()
{
  //
  // update stack with new event
  //
  if (!fMCEvent){ AliError("No fMCEvent"); return kFALSE;}
  AliStack* stack = fMCEvent->Stack();
  if (!stack){ AliError("Could not retrive stack!"); return kFALSE; }
  fStack = stack;
  return kTRUE;
}

//____________________________________________________________
AliMCParticle* AliDielectronMC::GetMCTrack(AliESDtrack* _track)
{
  //
  // return MC track
  //
  Int_t label = TMath::Abs(_track->GetLabel());
  AliMCParticle *mctrack = dynamic_cast<AliMCParticle *>(fMCEvent->GetTrack(label));
  return mctrack;
}

//____________________________________________________________
TParticle* AliDielectronMC::GetMCTrackFromStack(AliESDtrack* _track)
{
  //
  // return MC track from stack
  //
  Int_t label = TMath::Abs(_track->GetLabel());
  if (!fStack) AliWarning("fStack is not available. Update stack first.");
  TParticle* mcpart = fStack->Particle(label);
  if (!mcpart) return NULL;
  return mcpart;
}

//____________________________________________________________
AliMCParticle* AliDielectronMC::GetMCTrackMother(AliESDtrack* _track)
{
  //
  // return MC track mother
  //
  AliMCParticle* mcpart = GetMCTrack(_track);
  if (!mcpart) return NULL;
printf("mcpart->GetMother() : %d\n",mcpart->GetMother());
  AliMCParticle* mcmother = dynamic_cast<AliMCParticle *>(fMCEvent->GetTrack(mcpart->GetMother()));
  if (!mcmother) return NULL;
  return mcmother;
}

//____________________________________________________________
TParticle* AliDielectronMC::GetMCTrackMotherFromStack(AliESDtrack* _track)
{
  //
  // return MC track mother from stack
  //
  TParticle* mcpart = GetMCTrackFromStack(_track);
  if ( !mcpart || mcpart->GetFirstMother()<=0 ) return NULL; 
  TParticle* mcmother = fStack->Particle(mcpart->GetFirstMother());
  if (!mcmother) return NULL;
  return mcmother;
}

//____________________________________________________________
Int_t AliDielectronMC::GetMCPID(AliESDtrack* _track)
{
  //
  // return PDG code of the track from the MC truth info
  //
  AliMCParticle* mcpart = GetMCTrack(_track);
  if (!mcpart) return -999;
  return mcpart->PdgCode();
}

//____________________________________________________________
Int_t AliDielectronMC::GetMCPIDFromStack(AliESDtrack* _track)
{
  // 
  // return MC PDG code from stack
  //
  TParticle* mcpart = GetMCTrackFromStack(_track);
  if (!mcpart) return -999;
  return mcpart->GetPdgCode();
}

//____________________________________________________________
Int_t AliDielectronMC::GetMotherPDG(AliESDtrack* _track)
{
  //
  // return PDG code of the mother track from the MC truth info
  //
  AliMCParticle* mcmother = GetMCTrackMother(_track);
  if (!mcmother) return -999;
  return mcmother->PdgCode();
}

//____________________________________________________________
Int_t AliDielectronMC::GetMotherPDGFromStack(AliESDtrack* _track)
{
  //
  // return PDG code of the mother track from stack
  //
  TParticle* mcmother = GetMCTrackMotherFromStack(_track);
  if (!mcmother) return -999;
  return mcmother->GetPdgCode();
}

//____________________________________________________________
Int_t AliDielectronMC::GetMCProcess(AliESDtrack* _track)
{
  //
  // return process number of the track
  //
  AliMCParticle* mcpart = GetMCTrack(_track);
  if (!mcpart) return -999;
  return 0;
}

//____________________________________________________________
Int_t AliDielectronMC::GetMCProcessFromStack(AliESDtrack* _track)
{
  //
  // return process number of the track
  //
  TParticle* mcpart = GetMCTrackFromStack(_track);
  if (!mcpart) return -999;
  return mcpart->GetUniqueID();
}

//____________________________________________________________
Int_t AliDielectronMC::GetMCProcessMother(AliESDtrack* _track)
{
  //
  // return process number of the mother of the track
  //
  AliMCParticle* mcmother = GetMCTrackMother(_track);
  if (!mcmother) return -999;
  return 0;
}

//____________________________________________________________
Int_t AliDielectronMC::GetMCProcessMotherFromStack(AliESDtrack* _track)
{
  //
  // return process number of the mother of the track
  //
  TParticle* mcmother = GetMCTrackMotherFromStack(_track);
  if (!mcmother) return -999;
  return mcmother->GetUniqueID();
}

//____________________________________________________________
Bool_t AliDielectronMC::IsMCMotherToEE(const AliVParticle *particle, Int_t pdgMother)
{
  //
  // Check if the Mother 'particle' is of type pdgMother and decays to e+e-
  //
  
  if (!fMCEvent) return kFALSE;
  
  if (particle->IsA()==AliMCParticle::Class()){
    return IsMCMotherToEEesd(static_cast<const AliMCParticle*>(particle),pdgMother);
  } else if (particle->IsA()==AliAODMCParticle::Class()){
    return IsMCMotherToEEaod(static_cast<const AliAODMCParticle*>(particle),pdgMother);
  } else {
    AliError("Unknown particle type");
  }
  return kFALSE;
  
}

//____________________________________________________________
Bool_t AliDielectronMC::IsMCMotherToEEesd(const AliMCParticle *particle, Int_t pdgMother)
{
  //
  // Check if the Mother 'particle' is of type pdgMother and decays to e+e-
  // ESD case
  //
  
  //check pdg code
  if (particle->PdgCode()!=pdgMother) return kFALSE;
  Int_t ifirst = particle->GetFirstDaughter();
  Int_t ilast  = particle->GetLastDaughter();
  
  //check number of daughters
  if ((ilast-ifirst)!=1) return kFALSE;
  AliMCParticle *firstD=static_cast<AliMCParticle*>(GetMCTrackFromMCEvent(ifirst));
  AliMCParticle *secondD=static_cast<AliMCParticle*>(GetMCTrackFromMCEvent(ilast));
  
  if (firstD->Charge()>0){
    if (firstD->PdgCode()!=-11) return kFALSE;
    if (secondD->PdgCode()!=11) return kFALSE;
  }else{
    if (firstD->PdgCode()!=11) return kFALSE;
    if (secondD->PdgCode()!=-11) return kFALSE;
  }
  
  return kTRUE;
}

//____________________________________________________________
Bool_t AliDielectronMC::IsMCMotherToEEaod(const AliAODMCParticle *particle, Int_t pdgMother)
{
  //
  // Check if the Mother 'particle' is of type pdgMother and decays to e+e-
  // AOD case
  //
  if (particle->GetPdgCode()!=pdgMother) return kFALSE;
  if (particle->GetNDaughters()!=2) return kFALSE;
  
  Int_t ifirst = particle->GetDaughter(0);
  Int_t ilast  = particle->GetDaughter(1);
  
  //check number of daughters
  if ((ilast-ifirst)!=1) return kFALSE;
  AliAODMCParticle *firstD=static_cast<AliAODMCParticle*>(GetMCTrackFromMCEvent(ifirst));
  AliAODMCParticle *secondD=static_cast<AliAODMCParticle*>(GetMCTrackFromMCEvent(ilast));
  
  if (firstD->Charge()>0){
    if (firstD->GetPdgCode()!=11) return kFALSE;
    if (secondD->GetPdgCode()!=-11) return kFALSE;
  }else{
    if (firstD->GetPdgCode()!=-11) return kFALSE;
    if (secondD->GetPdgCode()!=11) return kFALSE;
  }
  return kTRUE;
}

//____________________________________________________________
Bool_t AliDielectronMC::IsMotherPdg(const AliVParticle *particle1, const AliVParticle *particle2, Int_t pdgMother)
{
  //
  // test if mother of particle 1 and 2 has pdgCode pdgMother and is the same;
  //
  if (!fMCEvent) return kFALSE;
  
  if (fAnaType==kESD) return IsMotherPdgESD(particle1, particle2, pdgMother);
  else if (fAnaType==kAOD) return IsMotherPdgAOD(particle1, particle2, pdgMother);
  
  return kFALSE;
}

//____________________________________________________________
Bool_t AliDielectronMC::IsMotherPdgESD(const AliVParticle *particle1, const AliVParticle *particle2, Int_t pdgMother)
{
  //
  // test if mother of particle 1 and 2 has pdgCode pdgMother and is the same;
  // ESD case
  //
  
  AliMCParticle *mcPart1=static_cast<AliMCParticle*>(GetMCTrackFromMCEvent(particle1->GetLabel()));
  AliMCParticle *mcPart2=static_cast<AliMCParticle*>(GetMCTrackFromMCEvent(particle2->GetLabel()));
  
  if (!mcPart1||!mcPart2) return kFALSE;
  
  Int_t lblMother1=mcPart1->GetMother();
  Int_t lblMother2=mcPart2->GetMother();
  
  AliMCParticle *mcMother1=static_cast<AliMCParticle*>(GetMCTrackFromMCEvent(lblMother1));
  if (!mcMother1) return kFALSE;
  if (lblMother1!=lblMother2) return kFALSE;
  if (mcMother1->PdgCode()!=pdgMother) return kFALSE;
  
  return kTRUE;
}

//____________________________________________________________
Bool_t AliDielectronMC::IsMotherPdgAOD(const AliVParticle *particle1, const AliVParticle *particle2, Int_t pdgMother)
{
  //
  // test if mother of particle 1 and 2 has pdgCode pdgMother and is the same;
  // AOD case
  //
  AliAODMCParticle *mcPart1=static_cast<AliAODMCParticle*>(GetMCTrackFromMCEvent(particle1->GetLabel()));
  AliAODMCParticle *mcPart2=static_cast<AliAODMCParticle*>(GetMCTrackFromMCEvent(particle2->GetLabel()));
  
  if (!mcPart1||!mcPart2) return kFALSE;
  
  Int_t lblMother1=mcPart1->GetMother();
  Int_t lblMother2=mcPart2->GetMother();
  
  AliAODMCParticle *mcMother1=static_cast<AliAODMCParticle*>(GetMCTrackFromMCEvent(lblMother1));
  
  if (!mcMother1) return kFALSE;
  if (lblMother1!=lblMother2) return kFALSE;
  if (mcMother1->GetPdgCode()!=pdgMother) return kFALSE;
  
  return kTRUE;
}

//____________________________________________________________
void AliDielectronMC::GetDaughters(const TObject *mother, AliVParticle* &d1, AliVParticle* &d2)
{
  //
  // Get First two daughters of the mother
  //

  Int_t lblD1=-1;
  Int_t lblD2=-1;
  d1=0;
  d2=0;
  if (!fMCEvent) return;
  if (fAnaType==kAOD){
    const AliAODMCParticle *aodMother=static_cast<const AliAODMCParticle*>(mother);
    lblD1=aodMother->GetDaughter(0);
    lblD2=aodMother->GetDaughter(1);
  } else if (fAnaType==kESD){
    const AliMCParticle *aodMother=static_cast<const AliMCParticle*>(mother);
    lblD1=aodMother->GetFirstDaughter();
    lblD2=aodMother->GetLastDaughter();
  }
  d1=fMCEvent->GetTrack(lblD1);
  d2=fMCEvent->GetTrack(lblD2);
}
