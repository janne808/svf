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

  // get filter parameters
  double GetFilterCutoff();
  double GetFilterResonance();
  int GetFilterOversamplingFactor();  
  
  // tick filter state
  void SVFfilter(double input);

  // get filter responses
  double GetFilterLowpass();
  double GetFilterBandpass();
  double GetFilterHighpass();
  
private:
  // filter parameters
  double cutoffFrequency;
  double Resonance;
  int oversamplingFactor;

  // filter state
  double lp;
  double bp;
  double hp;
};

#endif
