#ifndef ALIJETREADERHEADER_H
#define ALIJETREADERHEADER_H
 
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */
 
//---------------------------------------------------------------------
// base class for Jet Reader Header 
// Author: jgcn@mda.cinvestav.mx
//---------------------------------------------------------------------
  
#include <TNamed.h>
#include <TString.h>
 
class AliJetReaderHeader : public TNamed
{

 public:
  AliJetReaderHeader(const char* name);
  AliJetReaderHeader();
  virtual ~AliJetReaderHeader();
  
  // Getters
  virtual TString GetComment() {return fComment;}
  virtual const char* GetDirectory() {return fDir.Data();}
  virtual const char* GetPattern() {return fPattern.Data();}
  Int_t   GetNEvents() const {return fLast-fFirst;}
  Int_t   GetLastEvent() const {return fLast;}
  Int_t   GetFirstEvent() const {return fFirst;}
  
  // Setters
  virtual void SetComment(const char* s) {fComment=TString(s);}
  virtual void SetPattern(const char* s) {fPattern=TString(s);}
  virtual void SetDirectory(const char* s) {fDir=TString(s);}
  virtual void SetFirstEvent(Int_t i=0) {fFirst=i;}
  virtual void SetLastEvent(Int_t i=-1) {fLast=i;}
  
 protected:

  Int_t fFirst;          // First and last events analyzed
  Int_t fLast;           // in current set of files
  TString fComment;      // a comment
  TString fDir;          // directory with input files
  TString fPattern;      // pattern to look for input files
  
  ClassDef(AliJetReaderHeader,1);
};
 
#endif
