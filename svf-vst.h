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
  float fMode;
  
  float fSampleRate;
  float fOversamplingFactor;
  int iOversamplingFactor;
  
  char programName[kVstMaxProgNameLen + 1];

  SVF *svf;
};

#endif
