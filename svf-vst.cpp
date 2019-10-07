#include <stdio.h>
#include "svf-vst.h"
#include "svf.h"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new SVFPlugin (audioMaster);
}

//-------------------------------------------------------------------------------------------------------
SVFPlugin::SVFPlugin (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, 3)	// 1 program, 3 parameters
{
	setNumInputs (1);		// stereo in
	setNumOutputs (1);		// stereo out
	setUniqueID ('SVF');	// identify
	canProcessReplacing ();	// supports replacing output
	canDoubleReplacing ();	// supports double precision processing

	fGain = 1.f;			// default to 0 dB
	fCutoff = 0.125f;
	fResonance = 0.5f;
	
	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name

	// instantiate SVF class
	svf = new SVF((double)(fCutoff), (double)(fResonance), 1);
}

//-------------------------------------------------------------------------------------------------------
SVFPlugin::~SVFPlugin ()
{
  delete svf;
}

//-------------------------------------------------------------------------------------------------------
void SVFPlugin::setSampleRate (float sampleRate)
{
}

//-------------------------------------------------------------------------------------------------------
void SVFPlugin::setProgramName (char* name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void SVFPlugin::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void SVFPlugin::setParameter (VstInt32 index, float value)
{
  switch(index){
  case 0:
    fGain = value;
    break;
  case 1:
    fCutoff = value;
    svf->SetFilterCutoff((double)fCutoff);    
    break;
  case 2:
    fResonance = value;
    svf->SetFilterResonance((double)fResonance);    
    break;
  }
}

//-----------------------------------------------------------------------------------------
float SVFPlugin::getParameter (VstInt32 index)
{
  switch(index){
  case 0:
    return fGain;
  case 1:
    return fCutoff;
  case 2:
    return fResonance;
  default:
    return 0.0;
  }
}

//-----------------------------------------------------------------------------------------
void SVFPlugin::getParameterName (VstInt32 index, char* label)
{
  switch(index){
  case 0:
    vst_strncpy (label, "Gain", kVstMaxParamStrLen);
    break;
  case 1:
    vst_strncpy (label, "Cutoff", kVstMaxParamStrLen);
    break;
  case 2:
    vst_strncpy (label, "Resonance", kVstMaxParamStrLen);
    break; 
 }
}

//-----------------------------------------------------------------------------------------
void SVFPlugin::getParameterDisplay (VstInt32 index, char* text)
{
  char ftext[kVstMaxParamStrLen];
  
  switch(index){
  case 0:
    dB2string (fGain, text, kVstMaxParamStrLen);
    break;
  case 1:
    sprintf(ftext, "%2.3f", fCutoff);
    vst_strncpy (text, ftext, kVstMaxParamStrLen);
    break;
  case 2:
    sprintf(ftext, "%2.3f", fResonance);
    vst_strncpy (text, ftext, kVstMaxParamStrLen);
    break;
  }
}

//-----------------------------------------------------------------------------------------
void SVFPlugin::getParameterLabel (VstInt32 index, char* label)
{
  switch(index){
  case 0:
    vst_strncpy (label, "dB", kVstMaxParamStrLen);
    break;
  case 1:
    vst_strncpy (label, "", kVstMaxParamStrLen);
    break;
  case 2:
    vst_strncpy (label, "", kVstMaxParamStrLen);
    break;
  }
}

//------------------------------------------------------------------------
bool SVFPlugin::getEffectName (char* name)
{
	vst_strncpy (name, "SVF", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool SVFPlugin::getProductString (char* text)
{
	vst_strncpy (text, "SVF", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool SVFPlugin::getVendorString (char* text)
{
	vst_strncpy (text, "Kocmotronic Technologies", kVstMaxVendorStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 SVFPlugin::getVendorVersion ()
{ 
	return 1000; 
}

//-----------------------------------------------------------------------------------------
void SVFPlugin::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    float* in1  =  inputs[0];
    float* out1 = outputs[0];
    
    while (--sampleFrames >= 0)
    {
      svf->SVFfilter((double)((*in1++) * fGain));
      (*out1++) = (float)(svf->GetFilterLowpass());
    }
}

//-----------------------------------------------------------------------------------------
void SVFPlugin::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{
    double* in1  =  inputs[0];
    double* out1 = outputs[0];
    double dGain = fGain;

    while (--sampleFrames >= 0)
    {
      svf->SVFfilter((double)((*in1++) * dGain));
      (*out1++) = (double)(svf->GetFilterLowpass());
    }
}
