//
//
#ifndef __dspsvfh__
#define __dspsvfh__

class SVF{
public:
  // constructor/destructor
  SVF(double newCutoff, double newResonance, int newOversamplingFactor);
  SVF();
  ~SVF();

  // set filter parameters
  void SetFilterCutoff(double newCutoff);
  void SetFilterResonance(double newResonance);
  void SetFilterOversamplingFactor(int newOversamplingFactor);
  void SetFilterMode(int newFilterMode);

  // get filter parameters
  double GetFilterCutoff();
  double GetFilterResonance();
  int GetFilterOversamplingFactor();  
  int GetFilterMode();  
  
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

  // filter state
  double lp;
  double bp;
  double hp;

  // filter output
  double out;
};

#endif
