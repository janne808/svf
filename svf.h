//
//
#ifndef __dspsvfh__
#define __dspsvfh__

#include "fir.h"

class SVF{
public:
  // constructor/destructor
  SVF(double newCutoff, double newResonance, int newOversamplingFactor, int newFilterMode, double newSampleRate);
  SVF();
  ~SVF();

  // set filter parameters
  void SetFilterCutoff(double newCutoff);
  void SetFilterResonance(double newResonance);
  void SetFilterOversamplingFactor(int newOversamplingFactor);
  void SetFilterMode(int newFilterMode);
  void SetFilterSampleRate(double newSampleRate);

  // get filter parameters
  double GetFilterCutoff();
  double GetFilterResonance();
  int GetFilterOversamplingFactor();  
  int GetFilterMode();  
  double GetFilterSampleRate();
  
  // tick filter state
  void SVFfilter(double input);

  // get filter responses
  double GetFilterLowpass();
  double GetFilterBandpass();
  double GetFilterHighpass();

  // get filter output
  double GetFilterOutput();
  
private:
  // filter parameters
  double cutoffFrequency;
  double Resonance;
  int oversamplingFactor;
  int filterMode;
  double sampleRate;

  // filter state
  double lp;
  double bp;
  double hp;

  // filter output
  double out;

  // FIR downsampling filter
  FIRLowpass *fir;
};

#endif
