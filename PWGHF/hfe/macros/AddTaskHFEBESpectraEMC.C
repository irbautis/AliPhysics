AliAnalysisTask *AddTaskHFEBESpectraEMC(
                                 TString ContNameExt = "",
                                 Bool_t UseTender=kTRUE,
                                 Bool_t FillElecSparse=kFALSE,
                                 Bool_t ClsTypeEMC=kTRUE, Bool_t ClsTypeDCAL=kTRUE,
                                 Bool_t SwitchPi0EtaWeightCalc = kTRUE,
                                 Bool_t SwitchNHFEeffi = kTRUE,
                                 Bool_t SwitchEleRecoEffi = kTRUE,
                                 Bool_t SwitchMCTempWeightCalc = kTRUE,
                                 Bool_t SwitchFillMCTemp = kTRUE,
                                 Bool_t SwitchRecalIP = kTRUE,
                                 Double_t deltaEta=0.05, Double_t deltaPhi=0.05,
                                 Double_t m02Min=0.05, Double_t m02Max1=0.9, Double_t m02Max2=0.7,
                                 Double_t m20Min=0.0, Double_t m20Max=20000,
                                 Double_t eovpMin=0.9, Double_t eovpMax=1.2,
                                 Int_t MimCent = -1, Int_t MaxCent = -1,
                                 TString centrality="V0M",
                                 Bool_t hasTwoEMCTrigThres=kFALSE, Int_t thEG1ADC=140, Int_t thEG2ADC=89)
{
    //get the current analysis manager
    AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
    if (!mgr) {
        Error("AddTask", "No analysis manager found.");
        return NULL;
    }
    if (!mgr->GetInputEventHandler()) {
        ::Error("AddTask", "This task requires an input event handler");
        return NULL;
    }
    TString type = mgr->GetInputEventHandler()->GetDataType(); // can be "ESD" or "AOD"
    
    Bool_t MCthere=kFALSE;
    AliMCEventHandler *mcH = dynamic_cast<AliMCEventHandler*>(mgr->GetMCtruthEventHandler());
    if(!mcH){
        MCthere=kFALSE;
    }else{
        MCthere=kTRUE;
    }
    
    char calib[100];
    if(UseTender)
    {
        if(MimCent==-1)
        {
            sprintf(calib,"wTender");
        }
        else
        {
            sprintf(calib,"wTender_%d_%d",MimCent,MaxCent);
        }
    }
    else
    {
        if(MimCent==-1)
        {
            sprintf(calib,"woTender_");
        }
        else
        {
            sprintf(calib,"woTender_%d_%d",MimCent,MaxCent);
        }
    }
    
    if(ClsTypeEMC && !ClsTypeDCAL)ContNameExt+="_EMC";
    if(!ClsTypeEMC && ClsTypeDCAL)ContNameExt+="_DCAL";
    
    // INT7
    AliAnalysisTaskHFEBESpectraEMC *hfecalqa7 = new AliAnalysisTaskHFEBESpectraEMC("emcqa");
    mgr->AddTask(hfecalqa7);
    hfecalqa7->SelectCollisionCandidates(AliVEvent::kINT7);
    hfecalqa7->SetElecIDsparse(FillElecSparse);
    hfecalqa7->SetTenderSwitch(UseTender);
    hfecalqa7->SetClusterTypeEMC(ClsTypeEMC);
    hfecalqa7->SetClusterTypeDCAL(ClsTypeDCAL);
    hfecalqa7->SetCentralityMim(MimCent);
    hfecalqa7->SetCentralityMax(MaxCent);
    hfecalqa7->SetCentralityEstimator(centrality.Data());
    hfecalqa7->SwitchPi0EtaWeightCalc(SwitchPi0EtaWeightCalc);
    hfecalqa7->SetNonHFEEffi(SwitchNHFEeffi);
    hfecalqa7->SetElecRecoEffi(SwitchEleRecoEffi);
    hfecalqa7->SwitchMCTemplateWeightCalc(SwitchMCTempWeightCalc);
    hfecalqa7->SwitchFillMCTemplate(SwitchFillMCTemp);
    hfecalqa7->SwitchRecalImpPar(SwitchRecalIP);
    hfecalqa7->SetTrackMatchPar(deltaEta, deltaPhi);
    hfecalqa7->SetM02Cut(m02Min,m02Max1,m02Max2);
    hfecalqa7->SetM20Cut(m20Min,m20Max);
    hfecalqa7->SetEovPCut(eovpMin,eovpMax);
    
    if(SwitchFillMCTemp){
        TString DMesonWeightMaps, BMesonWeightMaps;
        
        DMesonWeightMaps = "alien:///alice/cern.ch/user/d/dthomas/DandBmesonpTweightCorrectionFiles/DMesonpTWeight.root";
        BMesonWeightMaps = "alien:///alice/cern.ch/user/d/dthomas/DandBmesonpTweightCorrectionFiles/BMesonpTWeight.root";
        
        printf("\n### reading file %s ...\n",DMesonWeightMaps.Data());
        printf("\n### reading file %s ...\n",BMesonWeightMaps.Data());
        
        TFile* f2 = TFile::Open(DMesonWeightMaps.Data());
        if(f2){
            TH1 *D1 = (TH1*)f2->Get("RatD0");
            TH1 *D2 = (TH1*)f2->Get("RatD0Up");
            TH1 *D3 = (TH1*)f2->Get("RatD0Down");
            
            hfecalqa7->SetDmesonWeightHist(D1,D2,D3);
        }
        //  f2->Close();
        TFile* f3 = TFile::Open(BMesonWeightMaps.Data());
        if(f3){
            TH1 *B1 = (TH1*)f3->Get("RatBMes");
            TH1 *B2 = (TH1*)f3->Get("RatBMesMin");
            TH1 *B3 = (TH1*)f3->Get("RatBMesMax");
            
            hfecalqa7->SetBmesonWeightHist(B1,B2,B3);
        }
        //  f3->Close();
    }
    
    TString containerName7 = mgr->GetCommonFileName();
    containerName7 += ":PWGHF_HFEBESpectraEMC";
    containerName7 += ContNameExt;
    TString SubcontainerName7 = Form("HFEBESpectraEMC_INT7_%s",calib);
    SubcontainerName7 += ContNameExt;
    AliAnalysisDataContainer *cinput7  = mgr->GetCommonInputContainer();
    AliAnalysisDataContainer *coutput7 = mgr->CreateContainer(SubcontainerName7, TList::Class(),AliAnalysisManager::kOutputContainer, containerName7.Data());
    mgr->ConnectInput(hfecalqa7, 0, cinput7);
    mgr->ConnectOutput(hfecalqa7, 1, coutput7);
    
    return NULL;
}

