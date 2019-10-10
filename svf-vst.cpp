/*
 *  (C) 2019 Janne Heikkarainen <janne808@radiofreerobotron.net>
 *
 *  All rights reserved.
 *
 *  This file is part of State Variable Filter VST plugin.
 *
 *  State Variable Filter VST plugin is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  State Variable Filter VST plugin is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with State Variable Filter VST plugin.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <math.h>
#include "svf-vst.h"
#include "svf.h"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new SVFPlugin (audioMaster);
}

//-------------------------------------------------------------------------------------------------------
SVFPlugin::SVFPlugin (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, 5)	// 1 program, 4 parameters
{
	setNumInputs (1);		// mono in
	setNumOutputs (1);		// mono out
	setUniqueID ('SVF');	        // identify
	canProcessReplacing ();	        // supports replacing output
	canDoubleReplacing ();	        // supports double precision processing

	fGain = 0.25f;			// defaults
	fCutoff = 0.125f;
	fResonance = 0.0f;

	fMode = 0.0f;
	
	fSampleRate = 44100.0f;
	fOversamplingFactor = 0.0f;
	iOversamplingFactor = 2;
	
	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name

	// instantiate SVF class
	svf = new SVF((double)(fCutoff), (double)(fResonance), iOversamplingFactor, 0, (double)(fSampleRate));
}

//-------------------------------------------------------------------------------------------------------
SVFPlugin::~SVFPlugin ()
{
  delete svf;
}

//-------------------------------------------------------------------------------------------------------
void SVFPlugin::setSampleRate (float sampleRate)
{
  fSampleRate = sampleRate;
  svf->SetFilterSampleRate(fSampleRate);
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
  case 3:
    fMode = value;
    svf->SetFilterMode((int)(floor(2.999*fMode)));    
    break;
  case 4:
    fOversamplingFactor = value;
    int newOversamplingFactor;
    newOversamplingFactor = (int)(pow(2.0, floor(1.0 + 2.999 * fOversamplingFactor)));
    if(newOversamplingFactor != iOversamplingFactor){
      iOversamplingFactor = newOversamplingFactor;
      svf->SetFilterOversamplingFactor(iOversamplingFactor);    
    }
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
  case 3:
    return fMode;
  case 4:
    return fOversamplingFactor;
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
    vst_strncpy (label, "Freq", kVstMaxParamStrLen);
    break;
  case 2:
    vst_strncpy (label, "Reso", kVstMaxParamStrLen);
    break; 
  case 3:
    vst_strncpy (label, "Mode", kVstMaxParamStrLen);
    break;
  case 4:
    vst_strncpy (label, "OverSmpl", kVstMaxParamStrLen);
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
    sprintf(ftext, "%2.3f", svf->GetFilterCutoff());
    vst_strncpy (text, ftext, kVstMaxParamStrLen);
    break;
  case 2:
    sprintf(ftext, "%2.3f", svf->GetFilterResonance());
    vst_strncpy (text, ftext, kVstMaxParamStrLen);
    break;
  case 3:
    {
      int mode = svf->GetFilterMode();

      switch(mode){
      case 0:
	sprintf(ftext, "lp");
	break;
      case 1:
	sprintf(ftext, "bp");
	break;
      case 2:
	sprintf(ftext, "hp");
	break;
      }
      vst_strncpy (text, ftext, kVstMaxParamStrLen);
      break;
    }
  case 4:
    sprintf(ftext, "%d", svf->GetFilterOversamplingFactor());
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
  case 3:
    vst_strncpy (label, "", kVstMaxParamStrLen);
    break;
  case 4:
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
      svf->SVFfilter((double)((*in1++) * 0.25 * fGain));
      (*out1++) = (float)(svf->GetFilterOutput());
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
      svf->SVFfilter((double)((*in1++) * 0.25 * dGain));
      (*out1++) = (double)(svf->GetFilterOutput());
    }
}
