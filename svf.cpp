//
//
#include <cstdlib>
#include <cmath>
#include "svf.h"

// constructor
SVF::SVF(double newCutoff, double newResonance, int newOversamplingFactor){
  // initialize filter parameters
  cutoffFrequency=newCutoff;
  Resonance=newResonance;
  oversamplingFactor=newOversamplingFactor;

  // initialize filter state
  hp=0.0;
  bp=0.0;
  lp=0.0;
}

// default constructor
SVF::SVF(){
  // initialize filter parameters
  cutoffFrequency=0.25;
  Resonance=0.5;
  oversamplingFactor=1;

  // initialize filter state
  hp=0.0;
  bp=0.0;
  lp=0.0;
}

// default destructor
SVF::~SVF(){
}

void SVF::SetFilterCutoff(double newCutoff){
  cutoffFrequency=newCutoff;
}

void SVF::SetFilterResonance(double newResonance){
  Resonance=newResonance;
}

void SVF::SetFilterOversamplingFactor(int newOversamplingFactor){
  oversamplingFactor=newOversamplingFactor;
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

void SVF::SVFfilter(double input){
  // noise terms
  double noise;
  double noise2;
  double noise3;

  // integration rate
  double dt = 0.001+1.75*(cutoffFrequency*cutoffFrequency*
			  cutoffFrequency*cutoffFrequency);

  // feedback amount
  double fb = 1.0-Resonance;
  
  // parameter filters
  static double dt_prime=0;
  static double fb_prime=0;

  // smooth parameter changes
  dt_prime = (dt * (1.0-0.999)) + (dt_prime * 0.999); 
  fb_prime = (fb * (1.0-0.999)) + (fb_prime * 0.999); 
   
  // update noise terms
  noise = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
  noise = 2.0*(noise - 0.5);
  noise2 = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
  noise2 = 2.0*(noise - 0.5);
  noise3 = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
  noise3 = 2.0*(noise - 0.5);

  // integrate filter state
  hp = input - (2.0*fb_prime-1.0)*bp - lp + 1.0e-6*noise;
  bp += dt_prime*hp;
  bp = std::tanh((1.0003335 + 1.0e-4*noise2)*bp);
  lp += dt_prime*bp;  
  lp = std::tanh((1.0002232 + 1.0e-4*noise3)*lp);
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

