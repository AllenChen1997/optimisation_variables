//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Oct 17 16:55:56 2022 by ROOT version 6.24/00
// from TTree treeMaker/tree
// found on file: ExoPieElementTuples_1.root
//////////////////////////////////////////////////////////

#ifndef dataTemplate_h
#define dataTemplate_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "TClonesArray.h"
#include "vector"
#include "vector"
#include "vector"
#include "vector"

class dataTemplate {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.
   static constexpr Int_t kMaxpfCHSMETPt = 1;
   static constexpr Int_t kMaxpfCHSMETPhi = 1;
   static constexpr Int_t kMaxpfCHSMETSumEt = 1;
   static constexpr Int_t kMaxpfTRKMETPt = 1;
   static constexpr Int_t kMaxpfTRKMETPhi = 1;
   static constexpr Int_t kMaxpfTRKMETPSumEt = 1;
   static constexpr Int_t kMaxFATN2_Beta1 = 1;
   static constexpr Int_t kMaxFATN3_Beta1 = 1;
   static constexpr Int_t kMaxFATN2_Beta2 = 1;
   static constexpr Int_t kMaxFATN3_Beta2 = 1;

   // Declaration of leaf types
   Float_t         pu_nTrueInt;
   Int_t           pu_nPUVert;
   Bool_t          isData;
   ULong64_t       eventId;
   ULong64_t       runId;
   ULong64_t       lumiSection;
   ULong64_t       bunchXing;
   Int_t           nVtx;
   TClonesArray    *vertexP3;
   Float_t         prefiringweight;
   Float_t         prefiringweightup;
   Float_t         prefiringweightdown;
   Float_t         pfMetCorrPt;
   Float_t         pfMetCorrPhi;
   Float_t         pfMetCorrSumEt;
   vector<float>   *pfMetCorrUnc;
   Float_t         pfMetCorrSig;
   Float_t         pfpatMet_smear;
   Float_t         pfmodifiedMetCorrPt;
   Float_t         pfmodifiedMetCorrPhi;
   Float_t         pfmodifiedMetCorrSumEt;
   Float_t         pfmodifiedMetCorrSig;
   vector<float>   *pfmodifiedMetCorrUnc;
   Float_t         pfpatmodifiedMet_smear;
   Float_t         pfMetRawPt;
   Float_t         pfMetRawPhi;
   Float_t         pfMetRawSumEt;
   Float_t         pfpatGenMETPt;
   Float_t         pfpatGenMETPhi;
   Float_t         pfpatGenMETSumEt;
   Float_t         pfpatCaloMETPt;
   Float_t         pfpatCaloMETPhi;
   Float_t         pfpatCaloMETSumEt;
   Float_t         pfCHSMETPt_;
   Float_t         pfCHSMETPhi_;
   Float_t         pfCHSMETSumEt_;
   Float_t         pfTRKMETPt_;
   Float_t         pfTRKMETPhi_;
   Float_t         pfTRKMETPSumEt_;
   Float_t         pfpuppiMETPt;
   Float_t         pfpuppiMETPhi;
   Float_t         pfpuppiMETSumEt;
   Float_t         pfpuppiMETSig;
   vector<float>   *pfpuppiMETUnc;
   Int_t           hlt_nTrigs;
   Int_t           hlt_nTrigObj;
   vector<bool>    *hlt_trigResult;
   vector<string>  *hlt_trigName;
   vector<int>     *hlt_trigPrescale;
   Int_t           hlt_nfilters;
   vector<bool>    *hlt_filterResult;
   vector<string>  *hlt_filterName;
   Int_t           nGenPar;
   vector<float>   *genParPx;
   vector<float>   *genParPy;
   vector<float>   *genParPz;
   vector<float>   *genParE;
   vector<int>     *genParId;
   vector<int>     *genParSt;
   vector<int>     *genMomParId;
   Float_t         mcWeight;
   Float_t         eleRho;
   Int_t           nEle;
   vector<float>   *elePx;
   vector<float>   *elePy;
   vector<float>   *elePz;
   vector<float>   *eleEnergy;
   vector<int>     *eleCharge;
   vector<bool>    *eleIsPassVeto;
   vector<bool>    *eleIsPassLoose;
   vector<bool>    *eleIsPassMedium;
   vector<bool>    *eleIsPassTight;
   vector<float>   *eleD0;
   vector<float>   *eleDz;
   Int_t           nMu;
   vector<float>   *muPx;
   vector<float>   *muPy;
   vector<float>   *muPz;
   vector<float>   *muEnergy;
   vector<int>     *muType;
   vector<int>     *muCharge;
   vector<bool>    *isGlobalMuon;
   vector<bool>    *isTrackerMuon;
   vector<bool>    *isPFMuon;
   vector<bool>    *isTightMuon;
   vector<bool>    *isLooseMuon;
   vector<bool>    *isMediumMuon;
   vector<bool>    *isMediumPromptMuon;
   vector<bool>    *isSoftMuon;
   vector<bool>    *PFIsoVeryLoose;
   vector<bool>    *PFIsoLoose;
   vector<bool>    *PFIsoMedium;
   vector<bool>    *PFIsoTight;
   vector<bool>    *PFIsoVeryTight;
   vector<float>   *muChHadIso;
   vector<float>   *muNeHadIso;
   vector<float>   *muGamIso;
   vector<float>   *muPUPt;
   Int_t           HPSTau_n;
   vector<float>   *taupt;
   TClonesArray    *HPSTau_Vposition;
   vector<float>   *HPSTau_Px;
   vector<float>   *HPSTau_Py;
   vector<float>   *HPSTau_Pz;
   vector<float>   *HPSTau_Energy;
   vector<bool>    *disc_againstElectronLooseMVA6;
   vector<bool>    *disc_againstElectronMediumMVA6;
   vector<bool>    *disc_againstElectronTightMVA6;
   vector<bool>    *disc_againstElectronVLooseMVA6;
   vector<bool>    *disc_againstElectronVTightMVA6;
   vector<bool>    *disc_againstMuonLoose3;
   vector<bool>    *disc_againstMuonTight3;
   vector<bool>    *disc_byLooseCombinedIsolationDeltaBetaCorr;
   vector<bool>    *disc_byMediumCombinedIsolationDeltaBetaCorr;
   vector<bool>    *disc_byTightCombinedIsolationDeltaBetaCorr;
   vector<bool>    *disc_byIsolationMVArun2017v2DBoldDMwLTraw2017;
   vector<bool>    *disc_byVVLooseIsolationMVArun2017v2DBoldDMwLT2017;
   vector<bool>    *disc_byVLooseIsolationMVArun2017v2DBoldDMwLT2017;
   vector<bool>    *disc_byLooseIsolationMVArun2017v2DBoldDMwLT2017;
   vector<bool>    *disc_byMediumIsolationMVArun2017v2DBoldDMwLT2017;
   vector<bool>    *disc_byTightIsolationMVArun2017v2DBoldDMwLT2017;
   vector<bool>    *disc_byVTightIsolationMVArun2017v2DBoldDMwLT2017;
   vector<bool>    *disc_byVVTightIsolationMVArun2017v2DBoldDMwLT2017;
   vector<bool>    *disc_byLooseCombinedIsolationDeltaBetaCorr3Hits;
   vector<bool>    *disc_byMediumCombinedIsolationDeltaBetaCorr3Hits;
   vector<bool>    *disc_byTightCombinedIsolationDeltaBetaCorr3Hits;
   vector<bool>    *disc_decayModeFinding;
   vector<bool>    *disc_decayModeFindingNewDMs;
   vector<float>   *HPSTau_NewVz;
   vector<int>     *HPSTau_charge;
   Int_t           nPho;
   vector<float>   *phoPx;
   vector<float>   *phoPy;
   vector<float>   *phoPz;
   vector<float>   *phoEnergy;
   vector<bool>    *phoIsPassTight;
   vector<bool>    *phoIsPassLoose;
   vector<bool>    *phoIsPassMedium;
   Int_t           THINnJet;
   vector<float>   *THINjetPx;
   vector<float>   *THINjetPy;
   vector<float>   *THINjetPz;
   vector<float>   *THINjetEnergy;
   Float_t         THINjetRho;
   Int_t           THINjetNPV;
   vector<float>   *THINjetCEmEF;
   vector<float>   *THINjetCHadEF;
   vector<float>   *THINjetNEmEF;
   vector<float>   *THINjetNHadEF;
   vector<int>     *THINjetCMulti;
   vector<int>     *THINjetNMultiplicity;
   vector<float>   *THINgenjetpx;
   vector<float>   *THINgenjetpy;
   vector<float>   *THINgenjetpz;
   vector<float>   *THINgenjetE;
   vector<float>   *THINjetCorrUncUp;
   vector<float>   *THINjetCorrUncDown;
   vector<int>     *THINjetHadronFlavor;
   vector<bool>    *THINjetPassIDTight;
   vector<float>   *THINjetCISVV2;
   vector<float>   *THINjetDeepCSV_b;
   vector<float>   *THINjetDeepCSV_c;
   vector<float>   *THINjetDeepCSV_udsg;
   vector<float>   *THINPUJetID;
   vector<bool>    *THINisPUJetIDLoose;
   vector<bool>    *THINisPUJetIDMedium;
   vector<bool>    *THINisPUJetIDTight;
   vector<float>   *THINbRegNNCorr;
   vector<float>   *THINbRegNNResolution;
   Int_t           FATnJet;
   vector<float>   *FATjetPx;
   vector<float>   *FATjetPy;
   vector<float>   *FATjetPz;
   vector<float>   *FATjetEnergy;
   Float_t         FATjetRho;
   Int_t           FATjetNPV;
   vector<float>   *FATjetCEmEF;
   vector<float>   *FATjetCHadEF;
   vector<float>   *FATjetNEmEF;
   vector<float>   *FATjetNHadEF;
   vector<int>     *FATjetCMulti;
   vector<int>     *FATjetNMultiplicity;
   vector<float>   *FATgenjetpx;
   vector<float>   *FATgenjetpy;
   vector<float>   *FATgenjetpz;
   vector<float>   *FATgenjetE;
   vector<float>   *FATjetCorrUncUp;
   vector<float>   *FATjetCorrUncDown;
   vector<int>     *FATjetHadronFlavor;
   vector<bool>    *FATjetPassIDTight;
   vector<float>   *FATjet_particleNet;
   vector<float>   *FATjet_DoubleSV;
   vector<float>   *FATjet_probQCDb;
   vector<float>   *FATjet_probHbb;
   vector<float>   *FATjet_probQCDc;
   vector<float>   *FATjet_probHcc;
   vector<float>   *FATjet_probHbbc;
   vector<float>   *FATjet_probHccb;
   vector<float>   *FATjet_prob_bbvsLight;
   vector<float>   *FATjet_prob_ccvsLight;
   vector<float>   *FATjet_prob_TvsQCD;
   vector<float>   *FATjet_prob_ZHccvsQCD;
   vector<float>   *FATjet_prob_WvsQCD;
   vector<float>   *FATjet_prob_ZHbbvsQCD;
   TClonesArray    *FATjetSDRawP4;
   vector<float>   *FATjetSDmass;
   vector<vector<int> > *FATsubjetSDHadronFlavor;
   vector<vector<float> > *FATsubjetSDCSV;
   vector<float>   *FATjetCHSSDmass;
   vector<float>   *FATjetCHSPRmass;
   vector<float>   *FATjetCHSPRmassL2L3Corr;
   vector<float>   *FATjetCHSSDmassL2L3Corr;
   vector<float>   *FATjetCHSTau1;
   vector<float>   *FATjetCHSTau2;
   vector<float>   *FATjetCHSTau3;
   vector<float>   *FATjetCHSPx;
   vector<float>   *FATjetCHSPy;
   vector<float>   *FATjetCHSPz;
   vector<float>   *FATjetCHSE;
   vector<float>   *FATjetTau1;
   vector<float>   *FATjetTau2;
   vector<float>   *FATjetTau3;
   vector<float>   *FATjetTau4;
   vector<float>   *FATN2_Beta1_;
   vector<float>   *FATN3_Beta1_;
   vector<float>   *FATN2_Beta2_;
   vector<float>   *FATN3_Beta2_;

   // List of branches
   TBranch        *b_pu_nTrueInt;   //!
   TBranch        *b_pu_nPUVert;   //!
   TBranch        *b_isData;   //!
   TBranch        *b_eventId;   //!
   TBranch        *b_runId;   //!
   TBranch        *b_lumiSection;   //!
   TBranch        *b_bunchXing;   //!
   TBranch        *b_nVtx;   //!
   TBranch        *b_vertexP3;   //!
   TBranch        *b_prefiringweight;   //!
   TBranch        *b_prefiringweightup;   //!
   TBranch        *b_prefiringweightdown;   //!
   TBranch        *b_pfMetCorrPt;   //!
   TBranch        *b_pfMetCorrPhi;   //!
   TBranch        *b_pfMetCorrSumEt;   //!
   TBranch        *b_pfMetCorrUnc;   //!
   TBranch        *b_pfMetCorrSig;   //!
   TBranch        *b_pfpatMet_smear;   //!
   TBranch        *b_pfmodifiedMetCorrPt;   //!
   TBranch        *b_pfmodifiedMetCorrPhi;   //!
   TBranch        *b_pfmodifiedMetCorrSumEt;   //!
   TBranch        *b_pfmodifiedMetCorrSig;   //!
   TBranch        *b_pfmodifiedMetCorrUnc;   //!
   TBranch        *b_pfpatmodifiedMet_smear;   //!
   TBranch        *b_pfMetRawPt;   //!
   TBranch        *b_pfMetRawPhi;   //!
   TBranch        *b_pfMetRawSumEt;   //!
   TBranch        *b_pfpatGenMETPt;   //!
   TBranch        *b_pfpatGenMETPhi;   //!
   TBranch        *b_pfpatGenMETSumEt;   //!
   TBranch        *b_pfpatCaloMETPt;   //!
   TBranch        *b_pfpatCaloMETPhi;   //!
   TBranch        *b_pfpatCaloMETSumEt;   //!
   TBranch        *b_pfCHSMETPt_;   //!
   TBranch        *b_pfCHSMETPhi_;   //!
   TBranch        *b_pfCHSMETSumEt_;   //!
   TBranch        *b_pfTRKMETPt_;   //!
   TBranch        *b_pfTRKMETPhi_;   //!
   TBranch        *b_pfTRKMETPSumEt_;   //!
   TBranch        *b_pfpuppiMETPt;   //!
   TBranch        *b_pfpuppiMETPhi;   //!
   TBranch        *b_pfpuppiMETSumEt;   //!
   TBranch        *b_pfpuppiMETSig;   //!
   TBranch        *b_pfpuppiMETUnc;   //!
   TBranch        *b_hlt_nTrigs;   //!
   TBranch        *b_hlt_nTrigObj;   //!
   TBranch        *b_hlt_trigResult;   //!
   TBranch        *b_hlt_trigName;   //!
   TBranch        *b_hlt_trigPrescale;   //!
   TBranch        *b_hlt_nfilters;   //!
   TBranch        *b_hlt_filterResult;   //!
   TBranch        *b_hlt_filterName;   //!
   TBranch        *b_nGenPar;   //!
   TBranch        *b_genParPx;   //!
   TBranch        *b_genParPy;   //!
   TBranch        *b_genParPz;   //!
   TBranch        *b_genParE;   //!
   TBranch        *b_genParId;   //!
   TBranch        *b_genParSt;   //!
   TBranch        *b_genMomParId;   //!
   TBranch        *b_mcWeight;   //!
   TBranch        *b_eleRho;   //!
   TBranch        *b_nEle;   //!
   TBranch        *b_elePx;   //!
   TBranch        *b_elePy;   //!
   TBranch        *b_elePz;   //!
   TBranch        *b_eleEnergy;   //!
   TBranch        *b_eleCharge;   //!
   TBranch        *b_eleIsPassVeto;   //!
   TBranch        *b_eleIsPassLoose;   //!
   TBranch        *b_eleIsPassMedium;   //!
   TBranch        *b_eleIsPassTight;   //!
   TBranch        *b_eleD0;   //!
   TBranch        *b_eleDz;   //!
   TBranch        *b_nMu;   //!
   TBranch        *b_muPx;   //!
   TBranch        *b_muPy;   //!
   TBranch        *b_muPz;   //!
   TBranch        *b_muEnergy;   //!
   TBranch        *b_muType;   //!
   TBranch        *b_muCharge;   //!
   TBranch        *b_isGlobalMuon;   //!
   TBranch        *b_isTrackerMuon;   //!
   TBranch        *b_isPFMuon;   //!
   TBranch        *b_isTightMuon;   //!
   TBranch        *b_isLooseMuon;   //!
   TBranch        *b_isMediumMuon;   //!
   TBranch        *b_isMediumPromptMuon;   //!
   TBranch        *b_isSoftMuon;   //!
   TBranch        *b_PFIsoVeryLoose;   //!
   TBranch        *b_PFIsoLoose;   //!
   TBranch        *b_PFIsoMedium;   //!
   TBranch        *b_PFIsoTight;   //!
   TBranch        *b_PFIsoVeryTight;   //!
   TBranch        *b_muChHadIso;   //!
   TBranch        *b_muNeHadIso;   //!
   TBranch        *b_muGamIso;   //!
   TBranch        *b_muPUPt;   //!
   TBranch        *b_HPSTau_n;   //!
   TBranch        *b_taupt;   //!
   TBranch        *b_HPSTau_Vposition;   //!
   TBranch        *b_HPSTau_Px;   //!
   TBranch        *b_HPSTau_Py;   //!
   TBranch        *b_HPSTau_Pz;   //!
   TBranch        *b_HPSTau_Energy;   //!
   TBranch        *b_disc_againstElectronLooseMVA6;   //!
   TBranch        *b_disc_againstElectronMediumMVA6;   //!
   TBranch        *b_disc_againstElectronTightMVA6;   //!
   TBranch        *b_disc_againstElectronVLooseMVA6;   //!
   TBranch        *b_disc_againstElectronVTightMVA6;   //!
   TBranch        *b_disc_againstMuonLoose3;   //!
   TBranch        *b_disc_againstMuonTight3;   //!
   TBranch        *b_disc_byLooseCombinedIsolationDeltaBetaCorr;   //!
   TBranch        *b_disc_byMediumCombinedIsolationDeltaBetaCorr;   //!
   TBranch        *b_disc_byTightCombinedIsolationDeltaBetaCorr;   //!
   TBranch        *b_disc_byIsolationMVArun2017v2DBoldDMwLTraw2017;   //!
   TBranch        *b_disc_byVVLooseIsolationMVArun2017v2DBoldDMwLT2017;   //!
   TBranch        *b_disc_byVLooseIsolationMVArun2017v2DBoldDMwLT2017;   //!
   TBranch        *b_disc_byLooseIsolationMVArun2017v2DBoldDMwLT2017;   //!
   TBranch        *b_disc_byMediumIsolationMVArun2017v2DBoldDMwLT2017;   //!
   TBranch        *b_disc_byTightIsolationMVArun2017v2DBoldDMwLT2017;   //!
   TBranch        *b_disc_byVTightIsolationMVArun2017v2DBoldDMwLT2017;   //!
   TBranch        *b_disc_byVVTightIsolationMVArun2017v2DBoldDMwLT2017;   //!
   TBranch        *b_disc_byLooseCombinedIsolationDeltaBetaCorr3Hits;   //!
   TBranch        *b_disc_byMediumCombinedIsolationDeltaBetaCorr3Hits;   //!
   TBranch        *b_disc_byTightCombinedIsolationDeltaBetaCorr3Hits;   //!
   TBranch        *b_disc_decayModeFinding;   //!
   TBranch        *b_disc_decayModeFindingNewDMs;   //!
   TBranch        *b_HPSTau_NewVz;   //!
   TBranch        *b_HPSTau_charge;   //!
   TBranch        *b_nPho;   //!
   TBranch        *b_phoPx;   //!
   TBranch        *b_phoPy;   //!
   TBranch        *b_phoPz;   //!
   TBranch        *b_phoEnergy;   //!
   TBranch        *b_phoIsPassTight;   //!
   TBranch        *b_phoIsPassLoose;   //!
   TBranch        *b_phoIsPassMedium;   //!
   TBranch        *b_THINnJet;   //!
   TBranch        *b_THINjetPx;   //!
   TBranch        *b_THINjetPy;   //!
   TBranch        *b_THINjetPz;   //!
   TBranch        *b_THINjetEnergy;   //!
   TBranch        *b_THINjetRho;   //!
   TBranch        *b_THINjetNPV;   //!
   TBranch        *b_THINjetCEmEF;   //!
   TBranch        *b_THINjetCHadEF;   //!
   TBranch        *b_THINjetNEmEF;   //!
   TBranch        *b_THINjetNHadEF;   //!
   TBranch        *b_THINjetCMulti;   //!
   TBranch        *b_THINjetNMultiplicity;   //!
   TBranch        *b_THINgenjetpx;   //!
   TBranch        *b_THINgenjetpy;   //!
   TBranch        *b_THINgenjetpz;   //!
   TBranch        *b_THINgenjetE;   //!
   TBranch        *b_THINjetCorrUncUp;   //!
   TBranch        *b_THINjetCorrUncDown;   //!
   TBranch        *b_THINjetHadronFlavor;   //!
   TBranch        *b_THINjetPassIDTight;   //!
   TBranch        *b_THINjetCISVV2;   //!
   TBranch        *b_THINjetDeepCSV_b;   //!
   TBranch        *b_THINjetDeepCSV_c;   //!
   TBranch        *b_THINjetDeepCSV_udsg;   //!
   TBranch        *b_THINPUJetID;   //!
   TBranch        *b_THINisPUJetIDLoose;   //!
   TBranch        *b_THINisPUJetIDMedium;   //!
   TBranch        *b_THINisPUJetIDTight;   //!
   TBranch        *b_THINbRegNNCorr;   //!
   TBranch        *b_THINbRegNNResolution;   //!
   TBranch        *b_FATnJet;   //!
   TBranch        *b_FATjetPx;   //!
   TBranch        *b_FATjetPy;   //!
   TBranch        *b_FATjetPz;   //!
   TBranch        *b_FATjetEnergy;   //!
   TBranch        *b_FATjetRho;   //!
   TBranch        *b_FATjetNPV;   //!
   TBranch        *b_FATjetCEmEF;   //!
   TBranch        *b_FATjetCHadEF;   //!
   TBranch        *b_FATjetNEmEF;   //!
   TBranch        *b_FATjetNHadEF;   //!
   TBranch        *b_FATjetCMulti;   //!
   TBranch        *b_FATjetNMultiplicity;   //!
   TBranch        *b_FATgenjetpx;   //!
   TBranch        *b_FATgenjetpy;   //!
   TBranch        *b_FATgenjetpz;   //!
   TBranch        *b_FATgenjetE;   //!
   TBranch        *b_FATjetCorrUncUp;   //!
   TBranch        *b_FATjetCorrUncDown;   //!
   TBranch        *b_FATjetHadronFlavor;   //!
   TBranch        *b_FATjetPassIDTight;   //!
   TBranch        *b_FATjet_particleNet;   //!
   TBranch        *b_FATjet_DoubleSV;   //!
   TBranch        *b_FATjet_probQCDb;   //!
   TBranch        *b_FATjet_probHbb;   //!
   TBranch        *b_FATjet_probQCDc;   //!
   TBranch        *b_FATjet_probHcc;   //!
   TBranch        *b_FATjet_probHbbc;   //!
   TBranch        *b_FATjet_probHccb;   //!
   TBranch        *b_FATjet_prob_bbvsLight;   //!
   TBranch        *b_FATjet_prob_ccvsLight;   //!
   TBranch        *b_FATjet_prob_TvsQCD;   //!
   TBranch        *b_FATjet_prob_ZHccvsQCD;   //!
   TBranch        *b_FATjet_prob_WvsQCD;   //!
   TBranch        *b_FATjet_prob_ZHbbvsQCD;   //!
   TBranch        *b_FATjetSDRawP4;   //!
   TBranch        *b_FATjetSDmass;   //!
   TBranch        *b_FATsubjetSDHadronFlavor;   //!
   TBranch        *b_FATsubjetSDCSV;   //!
   TBranch        *b_FATjetCHSSDmass;   //!
   TBranch        *b_FATjetCHSPRmass;   //!
   TBranch        *b_FATjetCHSPRmassL2L3Corr;   //!
   TBranch        *b_FATjetCHSSDmassL2L3Corr;   //!
   TBranch        *b_FATjetCHSTau1;   //!
   TBranch        *b_FATjetCHSTau2;   //!
   TBranch        *b_FATjetCHSTau3;   //!
   TBranch        *b_FATjetCHSPx;   //!
   TBranch        *b_FATjetCHSPy;   //!
   TBranch        *b_FATjetCHSPz;   //!
   TBranch        *b_FATjetCHSE;   //!
   TBranch        *b_FATjetTau1;   //!
   TBranch        *b_FATjetTau2;   //!
   TBranch        *b_FATjetTau3;   //!
   TBranch        *b_FATjetTau4;   //!
   TBranch        *b_FATN2_Beta1_;   //!
   TBranch        *b_FATN3_Beta1_;   //!
   TBranch        *b_FATN2_Beta2_;   //!
   TBranch        *b_FATN3_Beta2_;   //!

   dataTemplate(TTree *tree=0);
   virtual ~dataTemplate();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef dataTemplate_cxx
dataTemplate::dataTemplate(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("ExoPieElementTuples_1.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("ExoPieElementTuples_1.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("ExoPieElementTuples_1.root:/tree");
      dir->GetObject("treeMaker",tree);

   }
   Init(tree);
}

dataTemplate::~dataTemplate()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t dataTemplate::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t dataTemplate::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void dataTemplate::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   vertexP3 = 0;
   pfMetCorrUnc = 0;
   pfmodifiedMetCorrUnc = 0;
   pfpuppiMETUnc = 0;
   hlt_trigResult = 0;
   hlt_trigName = 0;
   hlt_trigPrescale = 0;
   hlt_filterResult = 0;
   hlt_filterName = 0;
   genParPx = 0;
   genParPy = 0;
   genParPz = 0;
   genParE = 0;
   genParId = 0;
   genParSt = 0;
   genMomParId = 0;
   elePx = 0;
   elePy = 0;
   elePz = 0;
   eleEnergy = 0;
   eleCharge = 0;
   eleIsPassVeto = 0;
   eleIsPassLoose = 0;
   eleIsPassMedium = 0;
   eleIsPassTight = 0;
   eleD0 = 0;
   eleDz = 0;
   muPx = 0;
   muPy = 0;
   muPz = 0;
   muEnergy = 0;
   muType = 0;
   muCharge = 0;
   isGlobalMuon = 0;
   isTrackerMuon = 0;
   isPFMuon = 0;
   isTightMuon = 0;
   isLooseMuon = 0;
   isMediumMuon = 0;
   isMediumPromptMuon = 0;
   isSoftMuon = 0;
   PFIsoVeryLoose = 0;
   PFIsoLoose = 0;
   PFIsoMedium = 0;
   PFIsoTight = 0;
   PFIsoVeryTight = 0;
   muChHadIso = 0;
   muNeHadIso = 0;
   muGamIso = 0;
   muPUPt = 0;
   taupt = 0;
   HPSTau_Vposition = 0;
   HPSTau_Px = 0;
   HPSTau_Py = 0;
   HPSTau_Pz = 0;
   HPSTau_Energy = 0;
   disc_againstElectronLooseMVA6 = 0;
   disc_againstElectronMediumMVA6 = 0;
   disc_againstElectronTightMVA6 = 0;
   disc_againstElectronVLooseMVA6 = 0;
   disc_againstElectronVTightMVA6 = 0;
   disc_againstMuonLoose3 = 0;
   disc_againstMuonTight3 = 0;
   disc_byLooseCombinedIsolationDeltaBetaCorr = 0;
   disc_byMediumCombinedIsolationDeltaBetaCorr = 0;
   disc_byTightCombinedIsolationDeltaBetaCorr = 0;
   disc_byIsolationMVArun2017v2DBoldDMwLTraw2017 = 0;
   disc_byVVLooseIsolationMVArun2017v2DBoldDMwLT2017 = 0;
   disc_byVLooseIsolationMVArun2017v2DBoldDMwLT2017 = 0;
   disc_byLooseIsolationMVArun2017v2DBoldDMwLT2017 = 0;
   disc_byMediumIsolationMVArun2017v2DBoldDMwLT2017 = 0;
   disc_byTightIsolationMVArun2017v2DBoldDMwLT2017 = 0;
   disc_byVTightIsolationMVArun2017v2DBoldDMwLT2017 = 0;
   disc_byVVTightIsolationMVArun2017v2DBoldDMwLT2017 = 0;
   disc_byLooseCombinedIsolationDeltaBetaCorr3Hits = 0;
   disc_byMediumCombinedIsolationDeltaBetaCorr3Hits = 0;
   disc_byTightCombinedIsolationDeltaBetaCorr3Hits = 0;
   disc_decayModeFinding = 0;
   disc_decayModeFindingNewDMs = 0;
   HPSTau_NewVz = 0;
   HPSTau_charge = 0;
   phoPx = 0;
   phoPy = 0;
   phoPz = 0;
   phoEnergy = 0;
   phoIsPassTight = 0;
   phoIsPassLoose = 0;
   phoIsPassMedium = 0;
   THINjetPx = 0;
   THINjetPy = 0;
   THINjetPz = 0;
   THINjetEnergy = 0;
   THINjetCEmEF = 0;
   THINjetCHadEF = 0;
   THINjetNEmEF = 0;
   THINjetNHadEF = 0;
   THINjetCMulti = 0;
   THINjetNMultiplicity = 0;
   THINgenjetpx = 0;
   THINgenjetpy = 0;
   THINgenjetpz = 0;
   THINgenjetE = 0;
   THINjetCorrUncUp = 0;
   THINjetCorrUncDown = 0;
   THINjetHadronFlavor = 0;
   THINjetPassIDTight = 0;
   THINjetCISVV2 = 0;
   THINjetDeepCSV_b = 0;
   THINjetDeepCSV_c = 0;
   THINjetDeepCSV_udsg = 0;
   THINPUJetID = 0;
   THINisPUJetIDLoose = 0;
   THINisPUJetIDMedium = 0;
   THINisPUJetIDTight = 0;
   THINbRegNNCorr = 0;
   THINbRegNNResolution = 0;
   FATjetPx = 0;
   FATjetPy = 0;
   FATjetPz = 0;
   FATjetEnergy = 0;
   FATjetCEmEF = 0;
   FATjetCHadEF = 0;
   FATjetNEmEF = 0;
   FATjetNHadEF = 0;
   FATjetCMulti = 0;
   FATjetNMultiplicity = 0;
   FATgenjetpx = 0;
   FATgenjetpy = 0;
   FATgenjetpz = 0;
   FATgenjetE = 0;
   FATjetCorrUncUp = 0;
   FATjetCorrUncDown = 0;
   FATjetHadronFlavor = 0;
   FATjetPassIDTight = 0;
   FATjet_particleNet = 0;
   FATjet_DoubleSV = 0;
   FATjet_probQCDb = 0;
   FATjet_probHbb = 0;
   FATjet_probQCDc = 0;
   FATjet_probHcc = 0;
   FATjet_probHbbc = 0;
   FATjet_probHccb = 0;
   FATjet_prob_bbvsLight = 0;
   FATjet_prob_ccvsLight = 0;
   FATjet_prob_TvsQCD = 0;
   FATjet_prob_ZHccvsQCD = 0;
   FATjet_prob_WvsQCD = 0;
   FATjet_prob_ZHbbvsQCD = 0;
   FATjetSDRawP4 = 0;
   FATjetSDmass = 0;
   FATsubjetSDHadronFlavor = 0;
   FATsubjetSDCSV = 0;
   FATjetCHSSDmass = 0;
   FATjetCHSPRmass = 0;
   FATjetCHSPRmassL2L3Corr = 0;
   FATjetCHSSDmassL2L3Corr = 0;
   FATjetCHSTau1 = 0;
   FATjetCHSTau2 = 0;
   FATjetCHSTau3 = 0;
   FATjetCHSPx = 0;
   FATjetCHSPy = 0;
   FATjetCHSPz = 0;
   FATjetCHSE = 0;
   FATjetTau1 = 0;
   FATjetTau2 = 0;
   FATjetTau3 = 0;
   FATjetTau4 = 0;
   FATN2_Beta1_ = 0;
   FATN3_Beta1_ = 0;
   FATN2_Beta2_ = 0;
   FATN3_Beta2_ = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("pu_nTrueInt", &pu_nTrueInt, &b_pu_nTrueInt);
   fChain->SetBranchAddress("pu_nPUVert", &pu_nPUVert, &b_pu_nPUVert);
   fChain->SetBranchAddress("isData", &isData, &b_isData);
   fChain->SetBranchAddress("eventId", &eventId, &b_eventId);
   fChain->SetBranchAddress("runId", &runId, &b_runId);
   fChain->SetBranchAddress("lumiSection", &lumiSection, &b_lumiSection);
   fChain->SetBranchAddress("bunchXing", &bunchXing, &b_bunchXing);
   fChain->SetBranchAddress("nVtx", &nVtx, &b_nVtx);
   fChain->SetBranchAddress("vertexP3", &vertexP3, &b_vertexP3);
   fChain->SetBranchAddress("prefiringweight", &prefiringweight, &b_prefiringweight);
   fChain->SetBranchAddress("prefiringweightup", &prefiringweightup, &b_prefiringweightup);
   fChain->SetBranchAddress("prefiringweightdown", &prefiringweightdown, &b_prefiringweightdown);
   fChain->SetBranchAddress("pfMetCorrPt", &pfMetCorrPt, &b_pfMetCorrPt);
   fChain->SetBranchAddress("pfMetCorrPhi", &pfMetCorrPhi, &b_pfMetCorrPhi);
   fChain->SetBranchAddress("pfMetCorrSumEt", &pfMetCorrSumEt, &b_pfMetCorrSumEt);
   fChain->SetBranchAddress("pfMetCorrUnc", &pfMetCorrUnc, &b_pfMetCorrUnc);
   fChain->SetBranchAddress("pfMetCorrSig", &pfMetCorrSig, &b_pfMetCorrSig);
   fChain->SetBranchAddress("pfpatMet_smear", &pfpatMet_smear, &b_pfpatMet_smear);
   fChain->SetBranchAddress("pfmodifiedMetCorrPt", &pfmodifiedMetCorrPt, &b_pfmodifiedMetCorrPt);
   fChain->SetBranchAddress("pfmodifiedMetCorrPhi", &pfmodifiedMetCorrPhi, &b_pfmodifiedMetCorrPhi);
   fChain->SetBranchAddress("pfmodifiedMetCorrSumEt", &pfmodifiedMetCorrSumEt, &b_pfmodifiedMetCorrSumEt);
   fChain->SetBranchAddress("pfmodifiedMetCorrSig", &pfmodifiedMetCorrSig, &b_pfmodifiedMetCorrSig);
   fChain->SetBranchAddress("pfmodifiedMetCorrUnc", &pfmodifiedMetCorrUnc, &b_pfmodifiedMetCorrUnc);
   fChain->SetBranchAddress("pfpatmodifiedMet_smear", &pfpatmodifiedMet_smear, &b_pfpatmodifiedMet_smear);
   fChain->SetBranchAddress("pfMetRawPt", &pfMetRawPt, &b_pfMetRawPt);
   fChain->SetBranchAddress("pfMetRawPhi", &pfMetRawPhi, &b_pfMetRawPhi);
   fChain->SetBranchAddress("pfMetRawSumEt", &pfMetRawSumEt, &b_pfMetRawSumEt);
   fChain->SetBranchAddress("pfpatGenMETPt", &pfpatGenMETPt, &b_pfpatGenMETPt);
   fChain->SetBranchAddress("pfpatGenMETPhi", &pfpatGenMETPhi, &b_pfpatGenMETPhi);
   fChain->SetBranchAddress("pfpatGenMETSumEt", &pfpatGenMETSumEt, &b_pfpatGenMETSumEt);
   fChain->SetBranchAddress("pfpatCaloMETPt", &pfpatCaloMETPt, &b_pfpatCaloMETPt);
   fChain->SetBranchAddress("pfpatCaloMETPhi", &pfpatCaloMETPhi, &b_pfpatCaloMETPhi);
   fChain->SetBranchAddress("pfpatCaloMETSumEt", &pfpatCaloMETSumEt, &b_pfpatCaloMETSumEt);
   fChain->SetBranchAddress("pfCHSMETPt_", &pfCHSMETPt_, &b_pfCHSMETPt_);
   fChain->SetBranchAddress("pfCHSMETPhi_", &pfCHSMETPhi_, &b_pfCHSMETPhi_);
   fChain->SetBranchAddress("pfCHSMETSumEt_", &pfCHSMETSumEt_, &b_pfCHSMETSumEt_);
   fChain->SetBranchAddress("pfTRKMETPt_", &pfTRKMETPt_, &b_pfTRKMETPt_);
   fChain->SetBranchAddress("pfTRKMETPhi_", &pfTRKMETPhi_, &b_pfTRKMETPhi_);
   fChain->SetBranchAddress("pfTRKMETPSumEt_", &pfTRKMETPSumEt_, &b_pfTRKMETPSumEt_);
   fChain->SetBranchAddress("pfpuppiMETPt", &pfpuppiMETPt, &b_pfpuppiMETPt);
   fChain->SetBranchAddress("pfpuppiMETPhi", &pfpuppiMETPhi, &b_pfpuppiMETPhi);
   fChain->SetBranchAddress("pfpuppiMETSumEt", &pfpuppiMETSumEt, &b_pfpuppiMETSumEt);
   fChain->SetBranchAddress("pfpuppiMETSig", &pfpuppiMETSig, &b_pfpuppiMETSig);
   fChain->SetBranchAddress("pfpuppiMETUnc", &pfpuppiMETUnc, &b_pfpuppiMETUnc);
   fChain->SetBranchAddress("hlt_nTrigs", &hlt_nTrigs, &b_hlt_nTrigs);
   fChain->SetBranchAddress("hlt_nTrigObj", &hlt_nTrigObj, &b_hlt_nTrigObj);
   fChain->SetBranchAddress("hlt_trigResult", &hlt_trigResult, &b_hlt_trigResult);
   fChain->SetBranchAddress("hlt_trigName", &hlt_trigName, &b_hlt_trigName);
   fChain->SetBranchAddress("hlt_trigPrescale", &hlt_trigPrescale, &b_hlt_trigPrescale);
   fChain->SetBranchAddress("hlt_nfilters", &hlt_nfilters, &b_hlt_nfilters);
   fChain->SetBranchAddress("hlt_filterResult", &hlt_filterResult, &b_hlt_filterResult);
   fChain->SetBranchAddress("hlt_filterName", &hlt_filterName, &b_hlt_filterName);
   fChain->SetBranchAddress("nGenPar", &nGenPar, &b_nGenPar);
   fChain->SetBranchAddress("genParPx", &genParPx, &b_genParPx);
   fChain->SetBranchAddress("genParPy", &genParPy, &b_genParPy);
   fChain->SetBranchAddress("genParPz", &genParPz, &b_genParPz);
   fChain->SetBranchAddress("genParE", &genParE, &b_genParE);
   fChain->SetBranchAddress("genParId", &genParId, &b_genParId);
   fChain->SetBranchAddress("genParSt", &genParSt, &b_genParSt);
   fChain->SetBranchAddress("genMomParId", &genMomParId, &b_genMomParId);
   fChain->SetBranchAddress("mcWeight", &mcWeight, &b_mcWeight);
   fChain->SetBranchAddress("eleRho", &eleRho, &b_eleRho);
   fChain->SetBranchAddress("nEle", &nEle, &b_nEle);
   fChain->SetBranchAddress("elePx", &elePx, &b_elePx);
   fChain->SetBranchAddress("elePy", &elePy, &b_elePy);
   fChain->SetBranchAddress("elePz", &elePz, &b_elePz);
   fChain->SetBranchAddress("eleEnergy", &eleEnergy, &b_eleEnergy);
   fChain->SetBranchAddress("eleCharge", &eleCharge, &b_eleCharge);
   fChain->SetBranchAddress("eleIsPassVeto", &eleIsPassVeto, &b_eleIsPassVeto);
   fChain->SetBranchAddress("eleIsPassLoose", &eleIsPassLoose, &b_eleIsPassLoose);
   fChain->SetBranchAddress("eleIsPassMedium", &eleIsPassMedium, &b_eleIsPassMedium);
   fChain->SetBranchAddress("eleIsPassTight", &eleIsPassTight, &b_eleIsPassTight);
   fChain->SetBranchAddress("eleD0", &eleD0, &b_eleD0);
   fChain->SetBranchAddress("eleDz", &eleDz, &b_eleDz);
   fChain->SetBranchAddress("nMu", &nMu, &b_nMu);
   fChain->SetBranchAddress("muPx", &muPx, &b_muPx);
   fChain->SetBranchAddress("muPy", &muPy, &b_muPy);
   fChain->SetBranchAddress("muPz", &muPz, &b_muPz);
   fChain->SetBranchAddress("muEnergy", &muEnergy, &b_muEnergy);
   fChain->SetBranchAddress("muType", &muType, &b_muType);
   fChain->SetBranchAddress("muCharge", &muCharge, &b_muCharge);
   fChain->SetBranchAddress("isGlobalMuon", &isGlobalMuon, &b_isGlobalMuon);
   fChain->SetBranchAddress("isTrackerMuon", &isTrackerMuon, &b_isTrackerMuon);
   fChain->SetBranchAddress("isPFMuon", &isPFMuon, &b_isPFMuon);
   fChain->SetBranchAddress("isTightMuon", &isTightMuon, &b_isTightMuon);
   fChain->SetBranchAddress("isLooseMuon", &isLooseMuon, &b_isLooseMuon);
   fChain->SetBranchAddress("isMediumMuon", &isMediumMuon, &b_isMediumMuon);
   fChain->SetBranchAddress("isMediumPromptMuon", &isMediumPromptMuon, &b_isMediumPromptMuon);
   fChain->SetBranchAddress("isSoftMuon", &isSoftMuon, &b_isSoftMuon);
   fChain->SetBranchAddress("PFIsoVeryLoose", &PFIsoVeryLoose, &b_PFIsoVeryLoose);
   fChain->SetBranchAddress("PFIsoLoose", &PFIsoLoose, &b_PFIsoLoose);
   fChain->SetBranchAddress("PFIsoMedium", &PFIsoMedium, &b_PFIsoMedium);
   fChain->SetBranchAddress("PFIsoTight", &PFIsoTight, &b_PFIsoTight);
   fChain->SetBranchAddress("PFIsoVeryTight", &PFIsoVeryTight, &b_PFIsoVeryTight);
   fChain->SetBranchAddress("muChHadIso", &muChHadIso, &b_muChHadIso);
   fChain->SetBranchAddress("muNeHadIso", &muNeHadIso, &b_muNeHadIso);
   fChain->SetBranchAddress("muGamIso", &muGamIso, &b_muGamIso);
   fChain->SetBranchAddress("muPUPt", &muPUPt, &b_muPUPt);
   fChain->SetBranchAddress("HPSTau_n", &HPSTau_n, &b_HPSTau_n);
   fChain->SetBranchAddress("taupt", &taupt, &b_taupt);
   fChain->SetBranchAddress("HPSTau_Vposition", &HPSTau_Vposition, &b_HPSTau_Vposition);
   fChain->SetBranchAddress("HPSTau_Px", &HPSTau_Px, &b_HPSTau_Px);
   fChain->SetBranchAddress("HPSTau_Py", &HPSTau_Py, &b_HPSTau_Py);
   fChain->SetBranchAddress("HPSTau_Pz", &HPSTau_Pz, &b_HPSTau_Pz);
   fChain->SetBranchAddress("HPSTau_Energy", &HPSTau_Energy, &b_HPSTau_Energy);
   fChain->SetBranchAddress("disc_againstElectronLooseMVA6", &disc_againstElectronLooseMVA6, &b_disc_againstElectronLooseMVA6);
   fChain->SetBranchAddress("disc_againstElectronMediumMVA6", &disc_againstElectronMediumMVA6, &b_disc_againstElectronMediumMVA6);
   fChain->SetBranchAddress("disc_againstElectronTightMVA6", &disc_againstElectronTightMVA6, &b_disc_againstElectronTightMVA6);
   fChain->SetBranchAddress("disc_againstElectronVLooseMVA6", &disc_againstElectronVLooseMVA6, &b_disc_againstElectronVLooseMVA6);
   fChain->SetBranchAddress("disc_againstElectronVTightMVA6", &disc_againstElectronVTightMVA6, &b_disc_againstElectronVTightMVA6);
   fChain->SetBranchAddress("disc_againstMuonLoose3", &disc_againstMuonLoose3, &b_disc_againstMuonLoose3);
   fChain->SetBranchAddress("disc_againstMuonTight3", &disc_againstMuonTight3, &b_disc_againstMuonTight3);
   fChain->SetBranchAddress("disc_byLooseCombinedIsolationDeltaBetaCorr", &disc_byLooseCombinedIsolationDeltaBetaCorr, &b_disc_byLooseCombinedIsolationDeltaBetaCorr);
   fChain->SetBranchAddress("disc_byMediumCombinedIsolationDeltaBetaCorr", &disc_byMediumCombinedIsolationDeltaBetaCorr, &b_disc_byMediumCombinedIsolationDeltaBetaCorr);
   fChain->SetBranchAddress("disc_byTightCombinedIsolationDeltaBetaCorr", &disc_byTightCombinedIsolationDeltaBetaCorr, &b_disc_byTightCombinedIsolationDeltaBetaCorr);
   fChain->SetBranchAddress("disc_byIsolationMVArun2017v2DBoldDMwLTraw2017", &disc_byIsolationMVArun2017v2DBoldDMwLTraw2017, &b_disc_byIsolationMVArun2017v2DBoldDMwLTraw2017);
   fChain->SetBranchAddress("disc_byVVLooseIsolationMVArun2017v2DBoldDMwLT2017", &disc_byVVLooseIsolationMVArun2017v2DBoldDMwLT2017, &b_disc_byVVLooseIsolationMVArun2017v2DBoldDMwLT2017);
   fChain->SetBranchAddress("disc_byVLooseIsolationMVArun2017v2DBoldDMwLT2017", &disc_byVLooseIsolationMVArun2017v2DBoldDMwLT2017, &b_disc_byVLooseIsolationMVArun2017v2DBoldDMwLT2017);
   fChain->SetBranchAddress("disc_byLooseIsolationMVArun2017v2DBoldDMwLT2017", &disc_byLooseIsolationMVArun2017v2DBoldDMwLT2017, &b_disc_byLooseIsolationMVArun2017v2DBoldDMwLT2017);
   fChain->SetBranchAddress("disc_byMediumIsolationMVArun2017v2DBoldDMwLT2017", &disc_byMediumIsolationMVArun2017v2DBoldDMwLT2017, &b_disc_byMediumIsolationMVArun2017v2DBoldDMwLT2017);
   fChain->SetBranchAddress("disc_byTightIsolationMVArun2017v2DBoldDMwLT2017", &disc_byTightIsolationMVArun2017v2DBoldDMwLT2017, &b_disc_byTightIsolationMVArun2017v2DBoldDMwLT2017);
   fChain->SetBranchAddress("disc_byVTightIsolationMVArun2017v2DBoldDMwLT2017", &disc_byVTightIsolationMVArun2017v2DBoldDMwLT2017, &b_disc_byVTightIsolationMVArun2017v2DBoldDMwLT2017);
   fChain->SetBranchAddress("disc_byVVTightIsolationMVArun2017v2DBoldDMwLT2017", &disc_byVVTightIsolationMVArun2017v2DBoldDMwLT2017, &b_disc_byVVTightIsolationMVArun2017v2DBoldDMwLT2017);
   fChain->SetBranchAddress("disc_byLooseCombinedIsolationDeltaBetaCorr3Hits", &disc_byLooseCombinedIsolationDeltaBetaCorr3Hits, &b_disc_byLooseCombinedIsolationDeltaBetaCorr3Hits);
   fChain->SetBranchAddress("disc_byMediumCombinedIsolationDeltaBetaCorr3Hits", &disc_byMediumCombinedIsolationDeltaBetaCorr3Hits, &b_disc_byMediumCombinedIsolationDeltaBetaCorr3Hits);
   fChain->SetBranchAddress("disc_byTightCombinedIsolationDeltaBetaCorr3Hits", &disc_byTightCombinedIsolationDeltaBetaCorr3Hits, &b_disc_byTightCombinedIsolationDeltaBetaCorr3Hits);
   fChain->SetBranchAddress("disc_decayModeFinding", &disc_decayModeFinding, &b_disc_decayModeFinding);
   fChain->SetBranchAddress("disc_decayModeFindingNewDMs", &disc_decayModeFindingNewDMs, &b_disc_decayModeFindingNewDMs);
   fChain->SetBranchAddress("HPSTau_NewVz", &HPSTau_NewVz, &b_HPSTau_NewVz);
   fChain->SetBranchAddress("HPSTau_charge", &HPSTau_charge, &b_HPSTau_charge);
   fChain->SetBranchAddress("nPho", &nPho, &b_nPho);
   fChain->SetBranchAddress("phoPx", &phoPx, &b_phoPx);
   fChain->SetBranchAddress("phoPy", &phoPy, &b_phoPy);
   fChain->SetBranchAddress("phoPz", &phoPz, &b_phoPz);
   fChain->SetBranchAddress("phoEnergy", &phoEnergy, &b_phoEnergy);
   fChain->SetBranchAddress("phoIsPassTight", &phoIsPassTight, &b_phoIsPassTight);
   fChain->SetBranchAddress("phoIsPassLoose", &phoIsPassLoose, &b_phoIsPassLoose);
   fChain->SetBranchAddress("phoIsPassMedium", &phoIsPassMedium, &b_phoIsPassMedium);
   fChain->SetBranchAddress("THINnJet", &THINnJet, &b_THINnJet);
   fChain->SetBranchAddress("THINjetPx", &THINjetPx, &b_THINjetPx);
   fChain->SetBranchAddress("THINjetPy", &THINjetPy, &b_THINjetPy);
   fChain->SetBranchAddress("THINjetPz", &THINjetPz, &b_THINjetPz);
   fChain->SetBranchAddress("THINjetEnergy", &THINjetEnergy, &b_THINjetEnergy);
   fChain->SetBranchAddress("THINjetRho", &THINjetRho, &b_THINjetRho);
   fChain->SetBranchAddress("THINjetNPV", &THINjetNPV, &b_THINjetNPV);
   fChain->SetBranchAddress("THINjetCEmEF", &THINjetCEmEF, &b_THINjetCEmEF);
   fChain->SetBranchAddress("THINjetCHadEF", &THINjetCHadEF, &b_THINjetCHadEF);
   fChain->SetBranchAddress("THINjetNEmEF", &THINjetNEmEF, &b_THINjetNEmEF);
   fChain->SetBranchAddress("THINjetNHadEF", &THINjetNHadEF, &b_THINjetNHadEF);
   fChain->SetBranchAddress("THINjetCMulti", &THINjetCMulti, &b_THINjetCMulti);
   fChain->SetBranchAddress("THINjetNMultiplicity", &THINjetNMultiplicity, &b_THINjetNMultiplicity);
   fChain->SetBranchAddress("THINgenjetpx", &THINgenjetpx, &b_THINgenjetpx);
   fChain->SetBranchAddress("THINgenjetpy", &THINgenjetpy, &b_THINgenjetpy);
   fChain->SetBranchAddress("THINgenjetpz", &THINgenjetpz, &b_THINgenjetpz);
   fChain->SetBranchAddress("THINgenjetE", &THINgenjetE, &b_THINgenjetE);
   fChain->SetBranchAddress("THINjetCorrUncUp", &THINjetCorrUncUp, &b_THINjetCorrUncUp);
   fChain->SetBranchAddress("THINjetCorrUncDown", &THINjetCorrUncDown, &b_THINjetCorrUncDown);
   fChain->SetBranchAddress("THINjetHadronFlavor", &THINjetHadronFlavor, &b_THINjetHadronFlavor);
   fChain->SetBranchAddress("THINjetPassIDTight", &THINjetPassIDTight, &b_THINjetPassIDTight);
   fChain->SetBranchAddress("THINjetCISVV2", &THINjetCISVV2, &b_THINjetCISVV2);
   fChain->SetBranchAddress("THINjetDeepCSV_b", &THINjetDeepCSV_b, &b_THINjetDeepCSV_b);
   fChain->SetBranchAddress("THINjetDeepCSV_c", &THINjetDeepCSV_c, &b_THINjetDeepCSV_c);
   fChain->SetBranchAddress("THINjetDeepCSV_udsg", &THINjetDeepCSV_udsg, &b_THINjetDeepCSV_udsg);
   fChain->SetBranchAddress("THINPUJetID", &THINPUJetID, &b_THINPUJetID);
   fChain->SetBranchAddress("THINisPUJetIDLoose", &THINisPUJetIDLoose, &b_THINisPUJetIDLoose);
   fChain->SetBranchAddress("THINisPUJetIDMedium", &THINisPUJetIDMedium, &b_THINisPUJetIDMedium);
   fChain->SetBranchAddress("THINisPUJetIDTight", &THINisPUJetIDTight, &b_THINisPUJetIDTight);
   fChain->SetBranchAddress("THINbRegNNCorr", &THINbRegNNCorr, &b_THINbRegNNCorr);
   fChain->SetBranchAddress("THINbRegNNResolution", &THINbRegNNResolution, &b_THINbRegNNResolution);
   fChain->SetBranchAddress("FATnJet", &FATnJet, &b_FATnJet);
   fChain->SetBranchAddress("FATjetPx", &FATjetPx, &b_FATjetPx);
   fChain->SetBranchAddress("FATjetPy", &FATjetPy, &b_FATjetPy);
   fChain->SetBranchAddress("FATjetPz", &FATjetPz, &b_FATjetPz);
   fChain->SetBranchAddress("FATjetEnergy", &FATjetEnergy, &b_FATjetEnergy);
   fChain->SetBranchAddress("FATjetRho", &FATjetRho, &b_FATjetRho);
   fChain->SetBranchAddress("FATjetNPV", &FATjetNPV, &b_FATjetNPV);
   fChain->SetBranchAddress("FATjetCEmEF", &FATjetCEmEF, &b_FATjetCEmEF);
   fChain->SetBranchAddress("FATjetCHadEF", &FATjetCHadEF, &b_FATjetCHadEF);
   fChain->SetBranchAddress("FATjetNEmEF", &FATjetNEmEF, &b_FATjetNEmEF);
   fChain->SetBranchAddress("FATjetNHadEF", &FATjetNHadEF, &b_FATjetNHadEF);
   fChain->SetBranchAddress("FATjetCMulti", &FATjetCMulti, &b_FATjetCMulti);
   fChain->SetBranchAddress("FATjetNMultiplicity", &FATjetNMultiplicity, &b_FATjetNMultiplicity);
   fChain->SetBranchAddress("FATgenjetpx", &FATgenjetpx, &b_FATgenjetpx);
   fChain->SetBranchAddress("FATgenjetpy", &FATgenjetpy, &b_FATgenjetpy);
   fChain->SetBranchAddress("FATgenjetpz", &FATgenjetpz, &b_FATgenjetpz);
   fChain->SetBranchAddress("FATgenjetE", &FATgenjetE, &b_FATgenjetE);
   fChain->SetBranchAddress("FATjetCorrUncUp", &FATjetCorrUncUp, &b_FATjetCorrUncUp);
   fChain->SetBranchAddress("FATjetCorrUncDown", &FATjetCorrUncDown, &b_FATjetCorrUncDown);
   fChain->SetBranchAddress("FATjetHadronFlavor", &FATjetHadronFlavor, &b_FATjetHadronFlavor);
   fChain->SetBranchAddress("FATjetPassIDTight", &FATjetPassIDTight, &b_FATjetPassIDTight);
   fChain->SetBranchAddress("FATjet_particleNet", &FATjet_particleNet, &b_FATjet_particleNet);
   fChain->SetBranchAddress("FATjet_DoubleSV", &FATjet_DoubleSV, &b_FATjet_DoubleSV);
   fChain->SetBranchAddress("FATjet_probQCDb", &FATjet_probQCDb, &b_FATjet_probQCDb);
   fChain->SetBranchAddress("FATjet_probHbb", &FATjet_probHbb, &b_FATjet_probHbb);
   fChain->SetBranchAddress("FATjet_probQCDc", &FATjet_probQCDc, &b_FATjet_probQCDc);
   fChain->SetBranchAddress("FATjet_probHcc", &FATjet_probHcc, &b_FATjet_probHcc);
   fChain->SetBranchAddress("FATjet_probHbbc", &FATjet_probHbbc, &b_FATjet_probHbbc);
   fChain->SetBranchAddress("FATjet_probHccb", &FATjet_probHccb, &b_FATjet_probHccb);
   fChain->SetBranchAddress("FATjet_prob_bbvsLight", &FATjet_prob_bbvsLight, &b_FATjet_prob_bbvsLight);
   fChain->SetBranchAddress("FATjet_prob_ccvsLight", &FATjet_prob_ccvsLight, &b_FATjet_prob_ccvsLight);
   fChain->SetBranchAddress("FATjet_prob_TvsQCD", &FATjet_prob_TvsQCD, &b_FATjet_prob_TvsQCD);
   fChain->SetBranchAddress("FATjet_prob_ZHccvsQCD", &FATjet_prob_ZHccvsQCD, &b_FATjet_prob_ZHccvsQCD);
   fChain->SetBranchAddress("FATjet_prob_WvsQCD", &FATjet_prob_WvsQCD, &b_FATjet_prob_WvsQCD);
   fChain->SetBranchAddress("FATjet_prob_ZHbbvsQCD", &FATjet_prob_ZHbbvsQCD, &b_FATjet_prob_ZHbbvsQCD);
   fChain->SetBranchAddress("FATjetSDRawP4", &FATjetSDRawP4, &b_FATjetSDRawP4);
   fChain->SetBranchAddress("FATjetSDmass", &FATjetSDmass, &b_FATjetSDmass);
   fChain->SetBranchAddress("FATsubjetSDHadronFlavor", &FATsubjetSDHadronFlavor, &b_FATsubjetSDHadronFlavor);
   fChain->SetBranchAddress("FATsubjetSDCSV", &FATsubjetSDCSV, &b_FATsubjetSDCSV);
   fChain->SetBranchAddress("FATjetCHSSDmass", &FATjetCHSSDmass, &b_FATjetCHSSDmass);
   fChain->SetBranchAddress("FATjetCHSPRmass", &FATjetCHSPRmass, &b_FATjetCHSPRmass);
   fChain->SetBranchAddress("FATjetCHSPRmassL2L3Corr", &FATjetCHSPRmassL2L3Corr, &b_FATjetCHSPRmassL2L3Corr);
   fChain->SetBranchAddress("FATjetCHSSDmassL2L3Corr", &FATjetCHSSDmassL2L3Corr, &b_FATjetCHSSDmassL2L3Corr);
   fChain->SetBranchAddress("FATjetCHSTau1", &FATjetCHSTau1, &b_FATjetCHSTau1);
   fChain->SetBranchAddress("FATjetCHSTau2", &FATjetCHSTau2, &b_FATjetCHSTau2);
   fChain->SetBranchAddress("FATjetCHSTau3", &FATjetCHSTau3, &b_FATjetCHSTau3);
   fChain->SetBranchAddress("FATjetCHSPx", &FATjetCHSPx, &b_FATjetCHSPx);
   fChain->SetBranchAddress("FATjetCHSPy", &FATjetCHSPy, &b_FATjetCHSPy);
   fChain->SetBranchAddress("FATjetCHSPz", &FATjetCHSPz, &b_FATjetCHSPz);
   fChain->SetBranchAddress("FATjetCHSE", &FATjetCHSE, &b_FATjetCHSE);
   fChain->SetBranchAddress("FATjetTau1", &FATjetTau1, &b_FATjetTau1);
   fChain->SetBranchAddress("FATjetTau2", &FATjetTau2, &b_FATjetTau2);
   fChain->SetBranchAddress("FATjetTau3", &FATjetTau3, &b_FATjetTau3);
   fChain->SetBranchAddress("FATjetTau4", &FATjetTau4, &b_FATjetTau4);
   fChain->SetBranchAddress("FATN2_Beta1_", &FATN2_Beta1_, &b_FATN2_Beta1_);
   fChain->SetBranchAddress("FATN3_Beta1_", &FATN3_Beta1_, &b_FATN3_Beta1_);
   fChain->SetBranchAddress("FATN2_Beta2_", &FATN2_Beta2_, &b_FATN2_Beta2_);
   fChain->SetBranchAddress("FATN3_Beta2_", &FATN3_Beta2_, &b_FATN3_Beta2_);
   Notify();
}

Bool_t dataTemplate::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void dataTemplate::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t dataTemplate::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef dataTemplate_cxx
