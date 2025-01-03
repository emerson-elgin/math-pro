#include <iostream>
#include <cmath>
#include <vector>
#include <fftw3.h>

void compute_fft(const std::vector<double>& signal, double sampling_rate) {
    size_t n = signal.size();
    
    // Allocate input and output arrays for FFTW
    fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * n);
    double* in = (double*)fftw_malloc(sizeof(double) * n);

    // Copy the input signal to FFTW input
    for (size_t i = 0; i < n; ++i) {
        in[i] = signal[i];
    }

    // Plan FFT
    fftw_plan plan = fftw_plan_dft_r2c_1d(n, in, out, FFTW_ESTIMATE);

    // Execute FFT
    fftw_execute(plan);

    // Compute frequencies and magnitudes
    std::cout << "Frequency (Hz)\tMagnitude\n";
    for (size_t i = 0; i < n / 2; ++i) {
        double freq = i * sampling_rate / n;
        double magnitude = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]) / n;
        std::cout << freq << "\t\t" << magnitude << "\n";
    }

    // Cleanup
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);
}

std::vector<double> generate_signal(size_t n, double sampling_rate) {
    std::vector<double> signal(n);
    double duration = n / sampling_rate;
    double t_step = 1.0 / sampling_rate;

    for (size_t i = 0; i < n; ++i) {
        double t = i * t_step;
        signal[i] = 10.0 * sin(2.0 * M_PI * 50.0 * t) + 5.0 * sin(2.0 * M_PI * 120.0 * t);
    }
    return signal;
}

int main() {
    // Parameters
    double sampling_rate = 1000.0; // 1000 Hz
    size_t n = 2048;               // Number of samples

    // Generate a signal
    auto signal = generate_signal(n, sampling_rate);

    // Perform FFT
    compute_fft(signal, sampling_rate);

    return 0;
}
