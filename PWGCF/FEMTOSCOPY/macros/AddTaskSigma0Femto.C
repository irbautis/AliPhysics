AliAnalysisTaskSE *AddTaskSigma0Femto(bool isMC = false, bool MomRes = false,
                                      bool fullBlastQA = false,
                                      TString trigger = "kINT7",
                                      const char *cutVariation = "0") {
  TString suffix = TString::Format("%s", cutVariation);

  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    Error("AddTaskSigma0Run2()", "No analysis manager found.");
    return 0x0;
  }

  // ================== GetInputEventHandler =============================
  AliVEventHandler *inputHandler = mgr->GetInputEventHandler();
  AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer();

  //=========  Set Cutnumber for V0Reader ================================
  TString cutnumberPhoton;
  cutnumberPhoton = "00200008400000002280920000";
  TString cutnumberEvent = "00000000";
  TString periodNameV0Reader = "";
  Bool_t enableV0findingEffi = kFALSE;
  Bool_t fillHistos = kTRUE;
  Bool_t runLightOutput = kFALSE;
  if (suffix != "0" && suffix != "999") {
    runLightOutput = kTRUE;
    fillHistos = kFALSE;
  }

  if (suffix == "1") {
    cutnumberPhoton = "0c200078000000002280920000";
  } else if (suffix == "2") {
    cutnumberPhoton = "002000a9400000001280920000";
  } else if (suffix == "3") {
    cutnumberPhoton = "002000081000000022c0920000";
  } else if (suffix == "4") {
    cutnumberPhoton = "00200009000000002280920000";
  } else if (suffix == "5") {
    cutnumberPhoton = "0c200078400000003280920000";
  } else if (suffix == "6") {
    cutnumberPhoton = "002000a84000000022c0920000";
  } else if (suffix == "7") {
    cutnumberPhoton = "00200008400000002280820000";
  } else if (suffix == "8") {
    cutnumberPhoton = "0c200008400000002280a20000";
  } else if (suffix == "9") {
    cutnumberPhoton = "00200008100000002240a20000";
  } else if (suffix == "10") {
    cutnumberPhoton = "002000a94000000012c0920000";
  } else if (suffix == "11") {
    cutnumberPhoton = "0c200008000000003280820000";
  } else if (suffix == "12") {
    cutnumberPhoton = "00200078400000002240a20000";
  } else if (suffix == "13") {
    cutnumberPhoton = "0c200008100000002280a20000";
  } else if (suffix == "14") {
    cutnumberPhoton = "002000084000000032c0920000";
  } else if (suffix == "15") {
    cutnumberPhoton = "0c200009400000001280920000";
  } else if (suffix == "16") {
    cutnumberPhoton = "002000a8400000002240820000";
  } else if (suffix == "17") {
    cutnumberPhoton = "002000a8000000003280920000";
  } else if (suffix == "18") {
    cutnumberPhoton = "0c2000a84000000022c0920000";
  } else if (suffix == "19") {
    cutnumberPhoton = "00200008100000001240920000";
  } else if (suffix == "20") {
    cutnumberPhoton = "0c2000784000000022c0920000";
  } else if (suffix == "21") {
    cutnumberPhoton = "002000a8400000003280820000";
  } else if (suffix == "22") {
    cutnumberPhoton = "0c2000080000000022c0920000";
  } else if (suffix == "23") {
    cutnumberPhoton = "002000a9400000002240920000";
  } else if (suffix == "24") {
    cutnumberPhoton = "002000a8100000001280920000";
  } else if (suffix == "25") {
    cutnumberPhoton = "0c200008000000002240920000";
  }

  //========= Add V0 Reader to  ANALYSIS manager if not yet existent =====
  TString V0ReaderName = TString::Format(
      "V0ReaderV1_%s_%s", cutnumberEvent.Data(), cutnumberPhoton.Data());
  AliConvEventCuts *fEventCuts = NULL;

  if (!(AliV0ReaderV1 *)mgr->GetTask(V0ReaderName.Data())) {
    AliV0ReaderV1 *fV0ReaderV1 = new AliV0ReaderV1(V0ReaderName.Data());
    if (periodNameV0Reader.CompareTo("") != 0)
      fV0ReaderV1->SetPeriodName(periodNameV0Reader);
    fV0ReaderV1->SetUseOwnXYZCalculation(kTRUE);
    fV0ReaderV1->SetCreateAODs(kFALSE);  // AOD Output
    fV0ReaderV1->SetUseAODConversionPhoton(kTRUE);
    fV0ReaderV1->SetProduceV0FindingEfficiency(enableV0findingEffi);

    if (!mgr) {
      Error("AddTask_V0ReaderV1", "No analysis manager found.");
      return NULL;
    }

    if (cutnumberEvent != "") {
      fEventCuts =
          new AliConvEventCuts(cutnumberEvent.Data(), cutnumberEvent.Data());
      fEventCuts->SetPreSelectionCutFlag(kTRUE);
      fEventCuts->SetV0ReaderName(V0ReaderName);
      fEventCuts->SetLightOutput(runLightOutput);
      if (periodNameV0Reader.CompareTo("") != 0)
        fEventCuts->SetPeriodEnum(periodNameV0Reader);
      fV0ReaderV1->SetEventCuts(fEventCuts);
      fEventCuts->SetFillCutHistograms("", kFALSE);
    }

    // Set AnalysisCut Number
    AliConversionPhotonCuts *fCuts = NULL;
    if (cutnumberPhoton != "") {
      fCuts = new AliConversionPhotonCuts(cutnumberPhoton.Data(),
                                          cutnumberPhoton.Data());
      fCuts->SetPreSelectionCutFlag(kTRUE);
      fCuts->SetIsHeavyIon(false);
      fCuts->SetV0ReaderName(V0ReaderName);
      fCuts->SetLightOutput(runLightOutput);
      fCuts->SetFillCutHistograms("", fillHistos);
      if (fCuts->InitializeCutsFromCutString(cutnumberPhoton.Data())) {
        fV0ReaderV1->SetConversionCuts(fCuts);
      }
    }
    fV0ReaderV1->Init();
    AliLog::SetGlobalLogLevel(AliLog::kFatal);

    // connect input V0Reader
    mgr->AddTask(fV0ReaderV1);
    mgr->ConnectInput(fV0ReaderV1, 0, cinput);
  }

  //========= Init subtasks and start analyis ============================
  // Event Cuts
  AliFemtoDreamEventCuts *evtCuts = AliFemtoDreamEventCuts::StandardCutsRun2();
  evtCuts->CleanUpMult(false, false, false, true);

  // Track Cuts
  AliFemtoDreamTrackCuts *TrackCuts =
      AliFemtoDreamTrackCuts::PrimProtonCuts(isMC, true, false, false);
  TrackCuts->SetCheckFilterBit(false);
  TrackCuts->SetCheckESDFiltering(true);
  TrackCuts->SetDCAReCalculation(false);
  TrackCuts->SetCutCharge(1);

  AliFemtoDreamTrackCuts *AntiTrackCuts =
      AliFemtoDreamTrackCuts::PrimProtonCuts(isMC, true, false, false);
  AntiTrackCuts->SetCheckFilterBit(false);
  AntiTrackCuts->SetCheckESDFiltering(true);
  AntiTrackCuts->SetDCAReCalculation(false);
  AntiTrackCuts->SetCutCharge(-1);

  if (suffix != "0" && suffix != "999") {
    TrackCuts->SetMinimalBooking(true);
    AntiTrackCuts->SetMinimalBooking(true);
  }
  if (suffix == "1") {
    TrackCuts->SetPtRange(0.4, 4.05);
    AntiTrackCuts->SetPtRange(0.4, 4.05);
    TrackCuts->SetEtaRange(-0.85, 0.85);
    AntiTrackCuts->SetEtaRange(-0.85, 0.85);
  } else if (suffix == "2") {
    TrackCuts->SetPtRange(0.6, 4.05);
    AntiTrackCuts->SetPtRange(0.6, 4.05);
    TrackCuts->SetPID(AliPID::kProton, 0.75, 2.5);
    AntiTrackCuts->SetPID(AliPID::kProton, 0.75, 2.5);
  } else if (suffix == "3") {
    TrackCuts->SetEtaRange(-0.85, 0.85);
    AntiTrackCuts->SetEtaRange(-0.85, 0.85);
    TrackCuts->SetNClsTPC(90);
    AntiTrackCuts->SetNClsTPC(90);
  } else if (suffix == "4") {
    TrackCuts->SetPtRange(0.6, 4.05);
    AntiTrackCuts->SetPtRange(0.6, 4.05);
    TrackCuts->SetPID(AliPID::kProton, 0.75, 3.5);
    AntiTrackCuts->SetPID(AliPID::kProton, 0.75, 3.5);
  } else if (suffix == "5") {
    TrackCuts->SetEtaRange(-0.75, 0.75);
    AntiTrackCuts->SetEtaRange(-0.75, 0.75);
    TrackCuts->SetNClsTPC(70);
    AntiTrackCuts->SetNClsTPC(70);
  } else if (suffix == "6") {
    TrackCuts->SetPtRange(0.6, 4.05);
    AntiTrackCuts->SetPtRange(0.6, 4.05);
    TrackCuts->SetPID(AliPID::kProton, 0.75, 3.5);
    AntiTrackCuts->SetPID(AliPID::kProton, 0.75, 3.5);
  } else if (suffix == "7") {
    TrackCuts->SetEtaRange(-0.85, 0.85);
    AntiTrackCuts->SetEtaRange(-0.85, 0.85);
    TrackCuts->SetNClsTPC(70);
    AntiTrackCuts->SetNClsTPC(70);
  } else if (suffix == "8") {
    TrackCuts->SetPtRange(0.6, 4.05);
    AntiTrackCuts->SetPtRange(0.6, 4.05);
    TrackCuts->SetPID(AliPID::kProton, 0.75, 2.5);
    AntiTrackCuts->SetPID(AliPID::kProton, 0.75, 2.5);
  } else if (suffix == "9") {
    TrackCuts->SetPtRange(0.6, 4.05);
    AntiTrackCuts->SetPtRange(0.6, 4.05);
    TrackCuts->SetNClsTPC(90);
    AntiTrackCuts->SetNClsTPC(90);
  } else if (suffix == "10") {
    TrackCuts->SetPID(AliPID::kProton, 0.75, 2.5);
    AntiTrackCuts->SetPID(AliPID::kProton, 0.75, 2.5);
    TrackCuts->SetNClsTPC(70);
    AntiTrackCuts->SetNClsTPC(70);
  } else if (suffix == "11") {
    TrackCuts->SetPtRange(0.6, 4.05);
    AntiTrackCuts->SetPtRange(0.6, 4.05);
    TrackCuts->SetPID(AliPID::kProton, 0.75, 2.5);
    AntiTrackCuts->SetPID(AliPID::kProton, 0.75, 2.5);
  } else if (suffix == "12") {
    TrackCuts->SetPtRange(0.6, 4.05);
    AntiTrackCuts->SetPtRange(0.6, 4.05);
    TrackCuts->SetNClsTPC(90);
    AntiTrackCuts->SetNClsTPC(90);
  } else if (suffix == "13") {
    TrackCuts->SetEtaRange(-0.85, 0.85);
    AntiTrackCuts->SetEtaRange(-0.85, 0.85);
    TrackCuts->SetPtRange(0.4, 4.05);
    AntiTrackCuts->SetPtRange(0.4, 4.05);
  } else if (suffix == "14") {
    TrackCuts->SetPID(AliPID::kProton, 0.75, 2.5);
    AntiTrackCuts->SetPID(AliPID::kProton, 0.75, 2.5);
    TrackCuts->SetEtaRange(-0.75, 0.75);
    AntiTrackCuts->SetEtaRange(-0.75, 0.75);
  } else if (suffix == "15") {
    TrackCuts->SetNClsTPC(90);
    AntiTrackCuts->SetNClsTPC(90);
    TrackCuts->SetPtRange(0.6, 4.05);
    AntiTrackCuts->SetPtRange(0.6, 4.05);
  } else if (suffix == "16") {
    TrackCuts->SetEtaRange(-0.75, 0.75);
    AntiTrackCuts->SetEtaRange(-0.75, 0.75);
    TrackCuts->SetPID(AliPID::kProton, 0.75, 2.5);
    AntiTrackCuts->SetPID(AliPID::kProton, 0.75, 2.5);
  } else if (suffix == "17") {
    TrackCuts->SetPtRange(0.6, 4.05);
    AntiTrackCuts->SetPtRange(0.6, 4.05);
    TrackCuts->SetNClsTPC(70);
    AntiTrackCuts->SetNClsTPC(70);
  } else if (suffix == "18") {
    TrackCuts->SetPtRange(0.6, 4.05);
    AntiTrackCuts->SetPtRange(0.6, 4.05);
    TrackCuts->SetPID(AliPID::kProton, 0.75, 3.5);
    AntiTrackCuts->SetPID(AliPID::kProton, 0.75, 3.5);
  } else if (suffix == "19") {
    TrackCuts->SetPtRange(0.6, 4.05);
    AntiTrackCuts->SetPtRange(0.6, 4.05);
    TrackCuts->SetPID(AliPID::kProton, 0.75, 2.5);
    AntiTrackCuts->SetPID(AliPID::kProton, 0.75, 2.5);
  } else if (suffix == "20") {
    TrackCuts->SetPtRange(0.6, 4.05);
    AntiTrackCuts->SetPtRange(0.6, 4.05);
    TrackCuts->SetPID(AliPID::kProton, 0.75, 2.5);
    AntiTrackCuts->SetPID(AliPID::kProton, 0.75, 2.5);
  } else if (suffix == "21") {
    TrackCuts->SetPtRange(0.6, 4.05);
    AntiTrackCuts->SetPtRange(0.6, 4.05);
    TrackCuts->SetNClsTPC(70);
    AntiTrackCuts->SetNClsTPC(70);
  } else if (suffix == "22") {
    TrackCuts->SetPtRange(0.6, 4.05);
    AntiTrackCuts->SetPtRange(0.6, 4.05);
    TrackCuts->SetPID(AliPID::kProton, 0.75, 2.5);
    AntiTrackCuts->SetPID(AliPID::kProton, 0.75, 2.5);
  } else if (suffix == "23") {
    TrackCuts->SetPtRange(0.4, 4.05);
    AntiTrackCuts->SetPtRange(0.4, 4.05);
    TrackCuts->SetNClsTPC(90);
    AntiTrackCuts->SetNClsTPC(90);
  } else if (suffix == "24") {
    TrackCuts->SetEtaRange(-0.75, 0.75);
    AntiTrackCuts->SetEtaRange(-0.75, 0.75);
    TrackCuts->SetPID(AliPID::kProton, 0.75, 3.5);
    AntiTrackCuts->SetPID(AliPID::kProton, 0.75, 3.5);
  } else if (suffix == "25") {
    TrackCuts->SetEtaRange(-0.85, 0.85);
    AntiTrackCuts->SetEtaRange(-0.85, 0.85);
    TrackCuts->SetNClsTPC(70);
    AntiTrackCuts->SetNClsTPC(70);
  }

  AliSigma0V0Cuts *v0Cuts = AliSigma0V0Cuts::LambdaCuts();
  v0Cuts->SetIsMC(isMC);
  v0Cuts->SetPID(3122);
  v0Cuts->SetPosPID(AliPID::kProton, 2212);
  v0Cuts->SetNegPID(AliPID::kPion, -211);

  AliSigma0V0Cuts *antiv0Cuts = AliSigma0V0Cuts::LambdaCuts();
  antiv0Cuts->SetIsMC(isMC);
  antiv0Cuts->SetPID(-3122);
  antiv0Cuts->SetPosPID(AliPID::kPion, 211);
  antiv0Cuts->SetNegPID(AliPID::kProton, -2212);

  if (suffix != "0") {
    v0Cuts->SetLightweight(true);
    antiv0Cuts->SetLightweight(true);
  }
  if (suffix == "1") {
    v0Cuts->SetArmenterosCut(0., 0.15, 0.4, 1);
    antiv0Cuts->SetArmenterosCut(0, 0.15, 0.4, 1);
    v0Cuts->SetV0CosPAMin(0.98);
    antiv0Cuts->SetV0CosPAMin(0.98);
    v0Cuts->SetEtaMax(0.85);
    antiv0Cuts->SetEtaMax(0.85);
  } else if (suffix == "2") {
    v0Cuts->SetV0PtMin(0.24);
    antiv0Cuts->SetV0PtMin(0.24);
    v0Cuts->SetPIDnSigma(6);
    antiv0Cuts->SetPIDnSigma(6);
    v0Cuts->SetTPCclusterMin(60);
    antiv0Cuts->SetTPCclusterMin(60);
  } else if (suffix == "3") {
    v0Cuts->SetV0PtMin(0.36);
    antiv0Cuts->SetV0PtMin(0.36);
    v0Cuts->SetArmenterosCut(0., 0.11, 0.2, 1);
    antiv0Cuts->SetArmenterosCut(0, 0.11, 0.2, 1);
    v0Cuts->SetPIDnSigma(6);
    antiv0Cuts->SetPIDnSigma(6);
  } else if (suffix == "4") {
    v0Cuts->SetPIDnSigma(6);
    antiv0Cuts->SetPIDnSigma(6);
    v0Cuts->SetEtaMax(0.85);
    antiv0Cuts->SetEtaMax(0.85);
    v0Cuts->SetLambdaSelection(1.115683 - 0.008, 1.115683 + 0.008);
    antiv0Cuts->SetLambdaSelection(1.115683 - 0.008, 1.115683 + 0.008);
  } else if (suffix == "5") {
    v0Cuts->SetV0CosPAMin(0.98);
    antiv0Cuts->SetV0CosPAMin(0.98);
    v0Cuts->SetArmenterosCut(0., 0.11, 0.2, 1);
    antiv0Cuts->SetArmenterosCut(0, 0.11, 0.2, 1);
  } else if (suffix == "6") {
    v0Cuts->SetV0PtMin(0.36);
    antiv0Cuts->SetV0PtMin(0.36);
    v0Cuts->SetPIDnSigma(4);
    antiv0Cuts->SetPIDnSigma(4);
    v0Cuts->SetEtaMax(0.85);
    antiv0Cuts->SetEtaMax(0.85);
  } else if (suffix == "7") {
    v0Cuts->SetV0PtMin(0.24);
    antiv0Cuts->SetV0PtMin(0.24);
    v0Cuts->SetArmenterosCut(0., 0.15, 0.4, 1);
    antiv0Cuts->SetArmenterosCut(0, 0.15, 0.4, 1);
    v0Cuts->SetTPCclusterMin(80);
    antiv0Cuts->SetTPCclusterMin(80);
  } else if (suffix == "8") {
    v0Cuts->SetV0CosPAMin(0.995);
    antiv0Cuts->SetV0CosPAMin(0.995);
    v0Cuts->SetArmenterosCut(0., 0.11, 0.2, 1);
    antiv0Cuts->SetArmenterosCut(0, 0.11, 0.2, 1);
  } else if (suffix == "9") {
    v0Cuts->SetV0CosPAMin(0.98);
    antiv0Cuts->SetV0CosPAMin(0.98);
    v0Cuts->SetPIDnSigma(6);
    antiv0Cuts->SetPIDnSigma(6);
    v0Cuts->SetTPCclusterMin(60);
    antiv0Cuts->SetTPCclusterMin(60);
  } else if (suffix == "10") {
    v0Cuts->SetV0PtMin(0.36);
    antiv0Cuts->SetV0PtMin(0.36);
    v0Cuts->SetTPCclusterMin(60);
    antiv0Cuts->SetTPCclusterMin(60);
    v0Cuts->SetEtaMax(0.85);
    antiv0Cuts->SetEtaMax(0.85);
  } else if (suffix == "11") {
    v0Cuts->SetV0PtMin(0.36);
    antiv0Cuts->SetV0PtMin(0.36);
    v0Cuts->SetArmenterosCut(0., 0.15, 0.4, 1);
    antiv0Cuts->SetArmenterosCut(0, 0.15, 0.4, 1);
    v0Cuts->SetV0CosPAMin(0.98);
    antiv0Cuts->SetV0CosPAMin(0.98);
  } else if (suffix == "12") {
    v0Cuts->SetV0CosPAMin(0.995);
    antiv0Cuts->SetV0CosPAMin(0.995);
    v0Cuts->SetPIDnSigma(4);
    antiv0Cuts->SetPIDnSigma(4);
    v0Cuts->SetTPCclusterMin(80);
    antiv0Cuts->SetTPCclusterMin(80);
  } else if (suffix == "13") {
    v0Cuts->SetV0CosPAMin(0.995);
    antiv0Cuts->SetV0CosPAMin(0.995);
    v0Cuts->SetArmenterosCut(0., 0.11, 0.2, 1);
    antiv0Cuts->SetArmenterosCut(0, 0.11, 0.2, 1);
    v0Cuts->SetTPCclusterMin(60);
    antiv0Cuts->SetTPCclusterMin(60);
  } else if (suffix == "14") {
    v0Cuts->SetPIDnSigma(6);
    antiv0Cuts->SetPIDnSigma(6);
    v0Cuts->SetEtaMax(0.85);
    antiv0Cuts->SetEtaMax(0.85);
    v0Cuts->SetTPCclusterMin(60);
    antiv0Cuts->SetTPCclusterMin(60);
  } else if (suffix == "15") {
    v0Cuts->SetV0CosPAMin(0.995);
    antiv0Cuts->SetV0CosPAMin(0.995);
    v0Cuts->SetLambdaSelection(1.115683 - 0.008, 1.115683 + 0.008);
    antiv0Cuts->SetLambdaSelection(1.115683 - 0.008, 1.115683 + 0.008);
    v0Cuts->SetTPCclusterMin(60);
    antiv0Cuts->SetTPCclusterMin(60);
  } else if (suffix == "16") {
    v0Cuts->SetV0PtMin(0.36);
    antiv0Cuts->SetV0PtMin(0.36);
    v0Cuts->SetTPCclusterMin(60);
    antiv0Cuts->SetTPCclusterMin(60);
    v0Cuts->SetArmenterosCut(0., 0.11, 0.2, 1);
    antiv0Cuts->SetArmenterosCut(0, 0.11, 0.2, 1);
  } else if (suffix == "17") {
    v0Cuts->SetV0PtMin(0.36);
    antiv0Cuts->SetV0PtMin(0.36);
    v0Cuts->SetV0CosPAMin(0.98);
    antiv0Cuts->SetV0CosPAMin(0.98);
    v0Cuts->SetTPCclusterMin(60);
    antiv0Cuts->SetTPCclusterMin(60);
  } else if (suffix == "18") {
    v0Cuts->SetV0CosPAMin(0.98);
    antiv0Cuts->SetV0CosPAMin(0.98);
    v0Cuts->SetArmenterosCut(0., 0.11, 0.2, 1);
    antiv0Cuts->SetArmenterosCut(0, 0.11, 0.2, 1);
    v0Cuts->SetTPCclusterMin(60);
    antiv0Cuts->SetTPCclusterMin(60);
  } else if (suffix == "19") {
    v0Cuts->SetV0CosPAMin(0.98);
    antiv0Cuts->SetV0CosPAMin(0.98);
    v0Cuts->SetArmenterosCut(0., 0.11, 0.2, 1);
    antiv0Cuts->SetArmenterosCut(0, 0.11, 0.2, 1);
    v0Cuts->SetPIDnSigma(4);
    antiv0Cuts->SetPIDnSigma(4);
  } else if (suffix == "20") {
    v0Cuts->SetV0CosPAMin(0.995);
    antiv0Cuts->SetV0CosPAMin(0.995);
    v0Cuts->SetLambdaSelection(1.115683 - 0.008, 1.115683 + 0.008);
    antiv0Cuts->SetLambdaSelection(1.115683 - 0.008, 1.115683 + 0.008);
    v0Cuts->SetV0PtMin(0.36);
    antiv0Cuts->SetV0PtMin(0.36);
  } else if (suffix == "21") {
    v0Cuts->SetV0CosPAMin(0.995);
    antiv0Cuts->SetV0CosPAMin(0.995);
    v0Cuts->SetTPCclusterMin(60);
    antiv0Cuts->SetTPCclusterMin(60);
    v0Cuts->SetEtaMax(0.85);
    antiv0Cuts->SetEtaMax(0.85);
  } else if (suffix == "22") {
    v0Cuts->SetV0PtMin(0.36);
    antiv0Cuts->SetV0PtMin(0.36);
    v0Cuts->SetPIDnSigma(4);
    antiv0Cuts->SetPIDnSigma(4);
    v0Cuts->SetArmenterosCut(0., 0.15, 0.4, 1);
    antiv0Cuts->SetArmenterosCut(0, 0.15, 0.4, 1);
  } else if (suffix == "23") {
    v0Cuts->SetV0PtMin(0.24);
    antiv0Cuts->SetV0PtMin(0.24);
    v0Cuts->SetEtaMax(0.85);
    antiv0Cuts->SetEtaMax(0.85);
    v0Cuts->SetArmenterosCut(0., 0.11, 0.2, 1);
    antiv0Cuts->SetArmenterosCut(0, 0.11, 0.2, 1);
  } else if (suffix == "24") {
    v0Cuts->SetArmenterosCut(0., 0.11, 0.2, 1);
    antiv0Cuts->SetArmenterosCut(0, 0.11, 0.2, 1);
    v0Cuts->SetV0PtMin(0.36);
    antiv0Cuts->SetV0PtMin(0.36);
    v0Cuts->SetTPCclusterMin(60);
    antiv0Cuts->SetTPCclusterMin(60);
  } else if (suffix == "25") {
    v0Cuts->SetV0PtMin(0.24);
    antiv0Cuts->SetV0PtMin(0.24);
    v0Cuts->SetV0CosPAMin(0.995);
    antiv0Cuts->SetV0CosPAMin(0.995);
    v0Cuts->SetPIDnSigma(4);
    antiv0Cuts->SetPIDnSigma(4);
  }
  if (suffix == "999") {
    v0Cuts->SetCheckCutsMC(true);
    antiv0Cuts->SetCheckCutsMC(true);
    v0Cuts->SetLightweight(false);
    antiv0Cuts->SetLightweight(false);
  }

  AliSigma0PhotonMotherCuts *sigmaCuts =
      AliSigma0PhotonMotherCuts::DefaultCuts();
  sigmaCuts->SetIsMC(isMC);
  sigmaCuts->SetPDG(3212, 3122, 22);
  sigmaCuts->SetLambdaCuts(v0Cuts);
  sigmaCuts->SetV0ReaderName(V0ReaderName.Data());
  sigmaCuts->SetIsSpectrum(false);
  if (suffix != "0" && suffix != "999") {
    sigmaCuts->SetLightweight(true);
  }

  AliSigma0PhotonMotherCuts *antiSigmaCuts =
      AliSigma0PhotonMotherCuts::DefaultCuts();
  antiSigmaCuts->SetIsMC(isMC);
  antiSigmaCuts->SetPDG(-3212, -3122, 22);
  antiSigmaCuts->SetLambdaCuts(antiv0Cuts);
  antiSigmaCuts->SetV0ReaderName(V0ReaderName.Data());
  antiSigmaCuts->SetIsSpectrum(false);
  if (suffix != "0" && suffix != "999") {
    antiSigmaCuts->SetLightweight(true);
  }

  // vary the sidebands
  if (suffix == "33") {
    sigmaCuts->SetSigmaSideband(0.01, 0.075);
    antiSigmaCuts->SetSigmaSideband(0.01, 0.075);
  } else if (suffix == "34") {
    sigmaCuts->SetSigmaSideband(0.01, 0.1);
    antiSigmaCuts->SetSigmaSideband(0.01, 0.1);
  } else if (suffix == "35") {
    sigmaCuts->SetSigmaSideband(0.025, 0.05);
    antiSigmaCuts->SetSigmaSideband(0.025, 0.05);
  } else if (suffix == "36") {
    sigmaCuts->SetSigmaSideband(0.025, 0.075);
    antiSigmaCuts->SetSigmaSideband(0.025, 0.075);
  } else if (suffix == "37") {
    sigmaCuts->SetSigmaSideband(0.025, 0.1);
    antiSigmaCuts->SetSigmaSideband(0.025, 0.1);
  } else if (suffix == "38") {
    sigmaCuts->SetSigmaSideband(0.005, 0.025);
    antiSigmaCuts->SetSigmaSideband(0.005, 0.025);
  } else if (suffix == "39") {
    sigmaCuts->SetSigmaSideband(0.005, 0.05);
    antiSigmaCuts->SetSigmaSideband(0.005, 0.05);
  } else if (suffix == "40") {
    sigmaCuts->SetSigmaSideband(0.005, 0.075);
    antiSigmaCuts->SetSigmaSideband(0.005, 0.075);
  } else if (suffix == "41") {
    sigmaCuts->SetSigmaSideband(0.005, 0.1);
    antiSigmaCuts->SetSigmaSideband(0.005, 0.1);
  }

  if (trigger == "kINT7") {
    sigmaCuts->SetMultiplicityMode(AliVEvent::kINT7);
    antiSigmaCuts->SetMultiplicityMode(AliVEvent::kINT7);
  } else if (trigger == "kHighMultV0") {
    sigmaCuts->SetMultiplicityMode(AliVEvent::kHighMultV0);
    antiSigmaCuts->SetMultiplicityMode(AliVEvent::kHighMultV0);
  } else if (trigger == "AliVEvent::kMB") {
    sigmaCuts->SetMultiplicityMode(AliVEvent::kINT7);
    antiSigmaCuts->SetMultiplicityMode(AliVEvent::kINT7);
  }

  // Femto Collection
  std::vector<int> PDGParticles;
  PDGParticles.push_back(2212);
  PDGParticles.push_back(2212);
  PDGParticles.push_back(3212);
  PDGParticles.push_back(3212);
  PDGParticles.push_back(3212);
  PDGParticles.push_back(3212);
  PDGParticles.push_back(3212);
  PDGParticles.push_back(3212);
  if (suffix == "0" && fullBlastQA) {
    PDGParticles.push_back(3122);
    PDGParticles.push_back(22);
    PDGParticles.push_back(3122);
    PDGParticles.push_back(22);
  }

  std::vector<float> ZVtxBins;
  ZVtxBins.push_back(-10);
  ZVtxBins.push_back(-8);
  ZVtxBins.push_back(-6);
  ZVtxBins.push_back(-4);
  ZVtxBins.push_back(-2);
  ZVtxBins.push_back(0);
  ZVtxBins.push_back(2);
  ZVtxBins.push_back(4);
  ZVtxBins.push_back(6);
  ZVtxBins.push_back(8);
  ZVtxBins.push_back(10);

  std::vector<int> NBins;
  std::vector<float> kMin;
  std::vector<float> kMax;
  std::vector<int> pairQA;
  std::vector<bool> closeRejection;
  const int nPairs = (suffix == "0" && fullBlastQA) ? 78 : 36;
  for (int i = 0; i < nPairs; ++i) {
    pairQA.push_back(0);
    closeRejection.push_back(false);
    if (suffix == "0") {
      NBins.push_back(600);
      kMin.push_back(0.);
      kMax.push_back(3.);
    } else {
      NBins.push_back(200);
      kMin.push_back(0.);
      kMax.push_back(1.);
    }
  }

  NBins[0] = 250;  // pp
  NBins[8] = 250;  // barp barp

  closeRejection[0] = true;  // pp
  closeRejection[8] = true;  // barp barp

  // do extended QA for the pairs in default mode
  if (suffix == "0" && !fullBlastQA) {
    NBins[0] = 750;  // pp
    NBins[8] = 750;  // barp barp

    pairQA[0] = 11;   // pp
    pairQA[2] = 14;   // pSigma
    pairQA[8] = 11;   // barp barp
    pairQA[10] = 14;  // barp bSigma
  } else if (suffix == "0" && fullBlastQA) {
    NBins[0] = 750;   // pp
    NBins[12] = 750;  // barp barp

    pairQA[0] = 11;   // pp
    pairQA[2] = 14;   // pSigma
    pairQA[12] = 11;  // barp barp
    pairQA[14] = 14;  // barp bSigma

    closeRejection[8] = false;  // no more barp barp
    closeRejection[12] = true;  // new barp barp
  }

  AliFemtoDreamCollConfig *config =
      new AliFemtoDreamCollConfig("Femto", "Femto");
  std::vector<int> MultBins;
  if (trigger == "kHighMultV0") {
    std::vector<int> MultBins;
    MultBins.push_back(0);
    MultBins.push_back(4);
    MultBins.push_back(8);
    MultBins.push_back(12);
    MultBins.push_back(16);
    MultBins.push_back(20);
    MultBins.push_back(24);
    MultBins.push_back(28);
    MultBins.push_back(32);
    MultBins.push_back(36);
    MultBins.push_back(40);
    MultBins.push_back(44);
    MultBins.push_back(48);
    MultBins.push_back(52);
    MultBins.push_back(56);
    MultBins.push_back(60);
    MultBins.push_back(64);
    MultBins.push_back(68);
    MultBins.push_back(72);
    MultBins.push_back(76);
    MultBins.push_back(80);
    MultBins.push_back(84);
    MultBins.push_back(88);
    MultBins.push_back(92);
    MultBins.push_back(96);
    MultBins.push_back(100);
    config->SetMultBins(MultBins);
  } else {
    std::vector<int> MultBins;
    MultBins.push_back(0);
    MultBins.push_back(4);
    MultBins.push_back(8);
    MultBins.push_back(12);
    MultBins.push_back(16);
    MultBins.push_back(20);
    MultBins.push_back(24);
    MultBins.push_back(28);
    MultBins.push_back(32);
    MultBins.push_back(36);
    MultBins.push_back(40);
    MultBins.push_back(60);
    MultBins.push_back(80);
    config->SetMultBins(MultBins);
  }
  config->SetMultBinning(true);

  config->SetExtendedQAPairs(pairQA);
  config->SetdPhidEtaPlotsSmallK(false);
  config->SetZBins(ZVtxBins);
  if (MomRes && isMC) {
    config->SetMomentumResolution(true);
  }

  if (trigger == "kHighMultV0") {
    config->SetDeltaEtaMax(0.01);
    config->SetDeltaPhiMax(0.01);
    config->SetClosePairRejection(closeRejection);
  }

  if (suffix == "0" && fullBlastQA) {
    config->SetPhiEtaBinnign(true);
    config->SetkTBinning(true);
    config->SetmTBinning(true);
    config->SetPtQA(true);
  }
  config->SetdPhidEtaPlots(false);
  config->SetPDGCodes(PDGParticles);
  config->SetNBinsHist(NBins);
  config->SetMinKRel(kMin);
  config->SetMaxKRel(kMax);
  config->SetMixingDepth(10);
  config->SetUseEventMixing(true);
  config->SetMultiplicityEstimator(AliFemtoDreamEvent::kRef08);
  if (suffix != "0") {
    config->SetMinimalBookingME(true);
  }

  AliAnalysisTaskSigma0Femto *task =
      new AliAnalysisTaskSigma0Femto("AnalysisTaskSigma0Femto", isMC);
  if (trigger == "kINT7") {
    task->SetTrigger(AliVEvent::kINT7);
    task->SetMultiplicityMode(AliVEvent::kINT7);
    task->SelectCollisionCandidates(AliVEvent::kINT7);
  } else if (trigger == "kHighMultV0") {
    if (isMC) {
      task->SetTrigger(AliVEvent::kINT7);
      task->SelectCollisionCandidates(AliVEvent::kINT7);
      task->SetMultiplicityMode(AliVEvent::kHighMultV0);
    } else {
      task->SetTrigger(AliVEvent::kHighMultV0);
      task->SelectCollisionCandidates(AliVEvent::kHighMultV0);
      task->SetMultiplicityMode(AliVEvent::kHighMultV0);
    }
  } else if (trigger == "AliVEvent::kMB") {
    task->SetTrigger(AliVEvent::kMB);
    task->SelectCollisionCandidates(AliVEvent::kMB);
    task->SetMultiplicityMode(AliVEvent::kINT7);
  }
  task->SetEventCuts(evtCuts);
  task->SetV0ReaderName(V0ReaderName.Data());
  task->SetProtonCuts(TrackCuts);
  task->SetAntiProtonCuts(AntiTrackCuts);
  task->SetV0Cuts(v0Cuts);
  task->SetAntiV0Cuts(antiv0Cuts);
  task->SetSigmaCuts(sigmaCuts);
  task->SetAntiSigmaCuts(antiSigmaCuts);
  task->SetCollectionConfig(config);

  if (suffix != "0" && suffix != "999") {
    task->SetLightweight(true);
  }

  if (suffix == "0" && fullBlastQA) {
    task->SetCheckDaughterCF(true);
  }

  mgr->AddTask(task);

  TString addon = "";
  if (trigger == "kINT7") {
    addon += "MB";
  } else if (trigger == "kHighMultV0") {
    addon += "HM";
  }

  TString file = AliAnalysisManager::GetCommonFileName();

  mgr->ConnectInput(task, 0, cinput);

  TString QAName = Form("%sQA%s", addon.Data(), suffix.Data());
  AliAnalysisDataContainer *coutputQA = mgr->CreateContainer(
      //@suppress("Invalid arguments") it works ffs
      QAName.Data(), TList::Class(), AliAnalysisManager::kOutputContainer,
      Form("%s:%s", file.Data(), QAName.Data()));
  mgr->ConnectOutput(task, 1, coutputQA);

  TString EvtCutsName = Form("%sEvtCuts%s", addon.Data(), suffix.Data());
  AliAnalysisDataContainer *coutputEvtCuts = mgr->CreateContainer(
      EvtCutsName.Data(), TList::Class(), AliAnalysisManager::kOutputContainer,
      Form("%s:%s", file.Data(), EvtCutsName.Data()));
  mgr->ConnectOutput(task, 2, coutputEvtCuts);

  TString TrackCutsName = Form("%sTrackCuts%s", addon.Data(), suffix.Data());
  AliAnalysisDataContainer *couputTrkCuts =
      mgr->CreateContainer(TrackCutsName.Data(), TList::Class(),
                           AliAnalysisManager::kOutputContainer,
                           Form("%s:%s", file.Data(), TrackCutsName.Data()));
  mgr->ConnectOutput(task, 3, couputTrkCuts);

  TString AntiTrackCutsName =
      Form("%sAntiTrackCuts%s", addon.Data(), suffix.Data());
  AliAnalysisDataContainer *coutputAntiTrkCuts = mgr->CreateContainer(
      AntiTrackCutsName.Data(), TList::Class(),
      AliAnalysisManager::kOutputContainer,
      Form("%s:%s", file.Data(), AntiTrackCutsName.Data()));
  mgr->ConnectOutput(task, 4, coutputAntiTrkCuts);

  TString v0CutsName = Form("%sv0Cuts%s", addon.Data(), suffix.Data());
  AliAnalysisDataContainer *coutputv0Cuts = mgr->CreateContainer(
      v0CutsName.Data(), TList::Class(), AliAnalysisManager::kOutputContainer,
      Form("%s:%s", file.Data(), v0CutsName.Data()));
  mgr->ConnectOutput(task, 5, coutputv0Cuts);

  TString Antiv0CutsName = Form("%sAntiv0Cuts%s", addon.Data(), suffix.Data());
  AliAnalysisDataContainer *coutputAntiv0Cuts =
      mgr->CreateContainer(Antiv0CutsName.Data(), TList::Class(),
                           AliAnalysisManager::kOutputContainer,
                           Form("%s:%s", file.Data(), Antiv0CutsName.Data()));
  mgr->ConnectOutput(task, 6, coutputAntiv0Cuts);

  TString Photonv0CutsName =
      Form("%sPhotonCuts%s", addon.Data(), suffix.Data());
  AliAnalysisDataContainer *coutputPhotonv0Cuts =
      mgr->CreateContainer(Photonv0CutsName.Data(), TList::Class(),
                           AliAnalysisManager::kOutputContainer,
                           Form("%s:%s", file.Data(), Photonv0CutsName.Data()));
  mgr->ConnectOutput(task, 7, coutputPhotonv0Cuts);

  TString SigmaCutsName = Form("%sSigma0Cuts%s", addon.Data(), suffix.Data());
  AliAnalysisDataContainer *coutputSigmaCuts =
      mgr->CreateContainer(SigmaCutsName.Data(), TList::Class(),
                           AliAnalysisManager::kOutputContainer,
                           Form("%s:%s", file.Data(), SigmaCutsName.Data()));
  mgr->ConnectOutput(task, 8, coutputSigmaCuts);

  TString AntiSigmaCutsName =
      Form("%sAntiSigma0Cuts%s", addon.Data(), suffix.Data());
  AliAnalysisDataContainer *coutputAntiSigmaCuts = mgr->CreateContainer(
      AntiSigmaCutsName.Data(), TList::Class(),
      AliAnalysisManager::kOutputContainer,
      Form("%s:%s", file.Data(), AntiSigmaCutsName.Data()));
  mgr->ConnectOutput(task, 9, coutputAntiSigmaCuts);

  AliAnalysisDataContainer *coutputResults;
  TString ResultsName = Form("%sResults%s", addon.Data(), suffix.Data());
  coutputResults = mgr->CreateContainer(
      ResultsName.Data(), TList::Class(), AliAnalysisManager::kOutputContainer,
      Form("%s:%s", file.Data(), ResultsName.Data()));
  mgr->ConnectOutput(task, 10, coutputResults);

  AliAnalysisDataContainer *coutputResultQA;
  TString ResultQAName = Form("%sResultQA%s", addon.Data(), suffix.Data());
  coutputResultQA = mgr->CreateContainer(
      ResultQAName.Data(), TList::Class(), AliAnalysisManager::kOutputContainer,
      Form("%s:%s", file.Data(), ResultQAName.Data()));
  mgr->ConnectOutput(task, 11, coutputResultQA);

  if (isMC) {
    TString TrkCutsMCName =
        Form("%sTrackCutsMC%s", addon.Data(), suffix.Data());
    AliAnalysisDataContainer *coutputTrkCutsMC =
        mgr->CreateContainer(TrkCutsMCName.Data(), TList::Class(),
                             AliAnalysisManager::kOutputContainer,
                             Form("%s:%s", file.Data(), TrkCutsMCName.Data()));
    mgr->ConnectOutput(task, 12, coutputTrkCutsMC);

    TString AntiTrkCutsMCName =
        Form("%sAntiTrackCutsMC%s", addon.Data(), suffix.Data());
    AliAnalysisDataContainer *coutputAntiTrkCutsMC = mgr->CreateContainer(
        AntiTrkCutsMCName.Data(), TList::Class(),
        AliAnalysisManager::kOutputContainer,
        Form("%s:%s", file.Data(), AntiTrkCutsMCName.Data()));
    mgr->ConnectOutput(task, 13, coutputAntiTrkCutsMC);
  }

  return task;
}
