//
//
#include <cstdlib>
#include <cmath>
#include "svf.h"
#include "fir.h"

// constructor
SVF::SVF(double newCutoff, double newResonance, int newOversamplingFactor, int newFilterMode, double newSampleRate){
  // initialize filter parameters
  cutoffFrequency = newCutoff;
  Resonance = newResonance;
  oversamplingFactor = newOversamplingFactor;
  filterMode = newFilterMode;
  sampleRate = newSampleRate;

  // initialize filter state
  hp = 0.0;
  bp = 0.0;
  lp = 0.0;
  out = 0.0;

  // instantiate downsampling filter
  fir = new FIRLowpass(sampleRate * oversamplingFactor, (sampleRate / (double)(oversamplingFactor)), 32);
}

// default constructor
SVF::SVF(){
  // initialize filter parameters
  cutoffFrequency = 0.25;
  Resonance = 0.5;
  oversamplingFactor = 4;
  filterMode = 0;
  sampleRate = 44100.0;
  
  // initialize filter state
  hp = 0.0;
  bp = 0.0;
  lp = 0.0;
  out = 0.0;

  // instantiate downsampling filter
  fir = new FIRLowpass(sampleRate * oversamplingFactor, (sampleRate / (double)(oversamplingFactor)), 32);
}

// default destructor
SVF::~SVF(){
  delete fir;
}

void SVF::SetFilterCutoff(double newCutoff){
  cutoffFrequency = newCutoff;
}

void SVF::SetFilterResonance(double newResonance){
  Resonance = newResonance;
}

void SVF::SetFilterOversamplingFactor(int newOversamplingFactor){
  oversamplingFactor = newOversamplingFactor;
  fir->SetFilterSamplerate(sampleRate * oversamplingFactor);
}

void SVF::SetFilterMode(int newFilterMode){
  filterMode = newFilterMode;
}

void SVF::SetFilterSampleRate(double newSampleRate){
  sampleRate = newSampleRate;
  fir->SetFilterSamplerate(sampleRate * (double)(oversamplingFactor));
  fir->SetFilterCutoff((sampleRate / (double)(oversamplingFactor)));
}

double SVF::GetFilterCutoff(){
  return cutoffFrequency;
}

double SVF::GetFilterResonance(){
  return Resonance;
}

int SVF::GetFilterOversamplingFactor(){
  return oversamplingFactor;
}

double SVF::GetFilterOutput(){
  return out;
}

int SVF::GetFilterMode(){
  return filterMode;
}

double SVF::GetFilterSampleRate(){
  return sampleRate;
}

void SVF::SVFfilter(double input){
  // noise term
  double noise;

  // integration rate
  double dt = 0.001+2.25*(cutoffFrequency*cutoffFrequency*
			  cutoffFrequency*cutoffFrequency);

  // feedback amount
  double fb = 1.0-Resonance;
  
  // parameter filters
  static double dt_prime=0;
  static double fb_prime=0;

  // smooth parameter changes
  dt_prime = (dt * (1.0-0.999)) + (dt_prime * 0.999); 
  fb_prime = (fb * (1.0-0.993)) + (fb_prime * 0.993); 
   
  // update noise terms
  noise = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
  noise = 2.0 * (noise - 0.5);

  // integrate filter state
  // with oversampling
  for(int nn = 0; nn < oversamplingFactor; nn++){
    hp = input - (2.0*fb_prime-1.0)*bp - lp + 1.0e-6*noise;
    bp += (dt_prime / oversamplingFactor) * hp;
    bp = std::tanh(bp);
    lp += (dt_prime / oversamplingFactor) * bp;  
    lp = std::tanh(lp);
    
    switch(filterMode){
    case 0:
      out = lp;
      break;
    
    case 1:
      out = bp;
      break;
    
    case 2:
      out = hp;
      break;
    
    default:
      out = 0.0;
    }

    // downsampling filter
    out = fir->FIRfilter(out);
  }
}

double SVF::GetFilterLowpass(){
  return lp;
}

double SVF::GetFilterBandpass(){
  return bp;
}

double SVF::GetFilterHighpass(){
  return hp;
}

