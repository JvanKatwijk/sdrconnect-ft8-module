
#
/*
 *    Copyright (C)  2020 .. 2025
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of the qt-ft8 decoder
 *
 *    qt-ft8 decoder is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    qt-ft8 decoder is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with qt-ft8 decoder; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include	"decimating_filter.h"
#include	<cstring>

	decimating_filter::decimating_filter (int inputrate,
	                                      int outrate, int freq):
	                                          fft_in (inputrate),
	                                          fft_out (outrate),
	                                          inwindow (inputrate),
	                                          freqWindow (inputrate) {
float	f	= (float)freq / 2 / inputrate;
float	sum	= 0;
float	tmp [115];
int	degree	= 115;

	fprintf (stderr, "new filter. In: %d, out %d, freq %d\n",
	                               inputrate, outrate, freq);
	this	-> inrate	= inputrate;
	this	-> outrate	= outrate;
	inbuffer		= new std::complex<float> (inputrate);
	outbuffer		= new std::complex<float> (outrate);
        for (int i = 0; i < inrate / 2; i ++)
           inwindow [i] = 5 *  (0.42 -
                    0.5 * cos (2 * M_PI * (float)i / (float)inrate / 2) +
                    0.08 * cos (4 * M_PI * (float)i / (float)inrate / 2));
//
//	filter:
	for (int i = 0; i < degree; i ++) {
           if (i == degree / 2)
              tmp [i] = 2 * M_PI * f;
           else
              tmp [i] = sin (2 * M_PI * f * (i - degree / 2))/ (i - degree / 2);
//
//      Blackman window
           tmp [i]  *= (0.42 -
                        0.5 * cos (2 * M_PI * (float)i / degree) +
                        0.08 * cos (4 * M_PI * (float)i / degree));

           sum += tmp [i];
        }

	hulpbuffer	= new std::complex<float> [inrate];
	for (int i = 0; i < inrate; i ++) {
	   hulpbuffer [i] = std::complex<float> (0, 0);
	   inbuffer   [i] = std::complex<float> (0, 0);
	}

	for (int i = 0; i < degree; i ++)
	   inbuffer [i] = std::complex< float> (tmp [i] / sum,
	                                        tmp [i] / sum);

	fft_in. do_FFT (inbuffer);
	for (int i = 0; i < inrate; i ++) {
	   freqWindow [i] = inbuffer [i];
	}

	inp		= 0;
	outp		= outrate;
}

	decimating_filter::~decimating_filter () {
	fprintf (stderr, "filter being deleted\n");
}

bool decimating_filter::Pass (std::complex<float> in,
	                      std::complex<float>* out) {
bool success = false;
int	i;

	if (outp < outrate / 2) {
	   *out = outbuffer [outp];
	   outp += 1;
	   success = true;	
	}

	inbuffer   [inrate / 2 + inp] = in;
	hulpbuffer [inp] = in;

	inp ++;
	if (inp < inrate / 2) 
	   return success;

	for (i = 0; i < inrate; i ++)
	   inbuffer [i] = std::complex<float> (
	                                  real (inbuffer [i]) * inwindow [i],
	                                  imag (inbuffer [i]) * inwindow [i]);

	fft_in. do_FFT (inbuffer);
	for (i = 0; i < inrate; i ++)
	   inbuffer [i] = inbuffer [i] * freqWindow [i];

	for (i = 0; i < outrate / 2; i ++) {
	   outbuffer [i] = inbuffer [i];
	   outbuffer [outrate / 2 + i] = inbuffer [inrate - outrate / 2 + i];
	}

	fft_out. do_IFFT (outbuffer);
	outp	= 0;
	inp	= 0;
	for (i = 0; i < inrate / 2; i ++)
	   inbuffer [i] = hulpbuffer [i];
	return success;
}

