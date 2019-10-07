//
//

#ifndef __dspfirh__
#define __dspfirh__

class FIRLowpass{
public:
  // constructor/destructor
  FIRLowpass(double newSamplerate, double newCutoff, int newOrder);
  FIRLowpass();
  ~FIRLowpass();

  // set filter parameters
  void SetFilterOrder(int newOrder);
  void SetFilterSamplerate(double newSamplerate);
  void SetFilterCutoff(double newCutoff);
  
  // FIR filter signal 
  double FIRfilter(double input);

  // get impulse response
  double* GetImpulseResponse();
  
private:
  // compute windowed ideal impulse response
  void ComputeImpulseResponse();

  // filter design variables
  double samplerate;
  double cutoff;
  int order;
  
  // dsp variables
  double omega_c;
  double *h_d;
  double *h;
  double *w;

  // ring buffer delay line
  double *ringBuffer;
  int ringBufferIndex;
};

#endif
