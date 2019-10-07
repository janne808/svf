#ifndef __svfvsth__
#define __svfvsth__

#include "public.sdk/source/vst2.x/audioeffectx.h"
#include "svf.h"

//-------------------------------------------------------------------------------------------------------
class SVFPlugin : public AudioEffectX
{
public:
  SVFPlugin (audioMasterCallback audioMaster);
  ~SVFPlugin ();
  
  // Processing
  virtual void setSampleRate (float sampleRate);
  
  virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
  virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);
  
  // Program
  virtual void setProgramName (char* name);
  virtual void getProgramName (char* name);
  
  // Parameters
  virtual void setParameter (VstInt32 index, float value);
  virtual float getParameter (VstInt32 index);
  virtual void getParameterLabel (VstInt32 index, char* label);
  virtual void getParameterDisplay (VstInt32 index, char* text);
  virtual void getParameterName (VstInt32 index, char* text);
  
  virtual bool getEffectName (char* name);
  virtual bool getVendorString (char* text);
  virtual bool getProductString (char* text);
  virtual VstInt32 getVendorVersion ();

protected:
  float fGain;
  float fCutoff;
  float fResonance;

  float fSampleRate;
  
  char programName[kVstMaxProgNameLen + 1];

  SVF *svf;
};

#endif
