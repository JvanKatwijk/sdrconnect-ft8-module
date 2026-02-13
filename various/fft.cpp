/*
 *
 *    Copyright (C) 20125
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
#include	"fft.h"
#include	<cstring>
/*
 */
	fft_handler:: fft_handler (int32_t fft_size) {
	this	-> fftSize = fft_size;

	vector	= (std::complex<float> *) FFTW_MALLOC (sizeof (std::complex<float>) * fftSize);
	for (int i = 0; i < fftSize; i ++)
	   vector [i] = std::complex<float> (0, 0);
	plan	= FFTW_PLAN_DFT_1D (fftSize,
	                            reinterpret_cast <fftwf_complex *>(vector),
	                            reinterpret_cast <fftwf_complex *>(vector),
	                            FFTW_FORWARD, FFTW_ESTIMATE);
}

	fft_handler::~fft_handler () {
	   FFTW_DESTROY_PLAN (plan);
	   FFTW_FREE (vector);
}

void	fft_handler::do_FFT (std::complex<float> *v) {
	for (int i = 0; i < fftSize; i ++)
	   vector [i] = v [i];
	FFTW_EXECUTE (plan);
	for (int i = 0; i < fftSize; i ++)
	   v [i] = vector [i];
}

	ifft_handler::ifft_handler	(int32_t fft_size) {
	this	-> fftSize = fft_size;

	vector	= (std::complex<float> *) FFTW_MALLOC (sizeof (std::complex<float>) * fftSize);
	for (int i = 0; i < fftSize; i ++)
	   vector [i] = 0;
	plan	= FFTW_PLAN_DFT_1D (fftSize,
	                            reinterpret_cast <fftwf_complex *>(vector),
	                            reinterpret_cast <fftwf_complex *>(vector),
	                            FFTW_BACKWARD, FFTW_ESTIMATE);
}

	ifft_handler::~ifft_handler () {
	   FFTW_DESTROY_PLAN (plan);
	   FFTW_FREE (vector);
}

void	ifft_handler::do_IFFT (std::complex<float> *v) {
	for (int i = 0; i < fftSize; i ++)
	   vector [i] = v [i];
	FFTW_EXECUTE (plan);
	Scale	(vector);
	for (int i = 0; i < fftSize; i ++)
	   v [i] = vector [i];
}

void    ifft_handler::Scale (std::complex<float> *Data) {
const float  Factor = 1.0 / float (fftSize);
 
        // scale all entries
        for (int Position = 0; Position < fftSize; Position ++)
           Data [Position] *= Factor;
}

