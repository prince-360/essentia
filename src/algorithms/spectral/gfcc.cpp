/* 
 * Copyright (C) 2006-2013  Music Technology Group - Universitat Pompeu Fabra
 *
 * This file is part of Essentia
 * 
 * Essentia is free software: you can redistribute it and/or modify it under 
 * the terms of the GNU Affero General Public License as published by the Free 
 * Software Foundation (FSF), either version 3 of the License, or (at your 
 * option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more 
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with 
 * this program.  If not, see http://www.gnu.org/licenses/
 */

#include "gfcc.h"
#include "essentiamath.h" 

using namespace std;
using namespace essentia;
using namespace standard;

const char* GFCC::name = "GFCC";
const char* GFCC::version = "1.0";
const char* GFCC::description = DOC("This algorithm computes the equivalent of MFCCs but using a gammatone filterbank (ERBBands) scaled on an Equivalent Rectangular Bandwidth (ERB) scale. These coefficients could be called 'Gammatone Feature Cepstral Coefficients.\n"
"\n"
"References:\n"
"  [1] Shao, Y., Jin, Z., Wang, D., Srinivasan, S.,  An auditory-based feature for robust speech recognition. Proc. ICASSP 2009");

void GFCC::configure() {
  _gtFilter->configure("sampleRate", parameter("sampleRate"),
                        "numberBands", parameter("numberBands"),
                        "lowFrequencyBound", parameter("lowFrequencyBound"),
                        "highFrequencyBound", parameter("highFrequencyBound"));

  _dct->configure("inputSize", parameter("numberBands"),
                  "outputSize", parameter("numberCoefficients"));
}

void GFCC::compute() {
  // get the inputs and outputs
  const vector<Real>& spectrum = _spectrum.get();
  vector<Real>& gfcc = _gfcc.get();
  vector<Real>& bands = _bands.get();

  // filter the spectrum using a gammatone filterbank
  _gtFilter->input("spectrum").set(spectrum);
  _gtFilter->output("bands").set(bands);
  _gtFilter->compute();


  for (int i=0; i<int(bands.size()); ++i) {
    bands[i] = amp2db(bands[i]);
  }

  // compute the DCT of these bands
  _dct->input("array").set(bands);
  _dct->output("dct").set(gfcc);
  _dct->compute();
}