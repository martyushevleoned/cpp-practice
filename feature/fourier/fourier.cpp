#include <algorithm>
#include <complex>
#include <vector>

namespace fourier
{
static int nextPow2(const unsigned int n)
{
    unsigned int pow2 = 1;
    while (pow2 < n)
        pow2 <<= 1;
    return pow2;
}

template <typename T>
static std::vector<std::complex<T>> toComplex(const std::vector<T>& samples)
{
    std::vector<std::complex<T>> complexSamples(samples.size());
    std::transform(samples.begin(), samples.end(), complexSamples.begin(),
                   [](const T sample) { return std::complex<T>(sample, 0); });
    return complexSamples;
}

template <typename T>
static std::vector<T> toAmplSpectrum(const std::vector<std::complex<T>>& complexSamples)
{
    std::vector<T> samples(complexSamples.size());
    std::transform(complexSamples.begin(), complexSamples.end(), samples.begin(),
                   [](const std::complex<T> complexSample) { return std::abs(complexSample); });
    return samples;
}

template <typename T>
static std::vector<T> toPhaseSpectrum(const std::vector<std::complex<T>>& complexSamples)
{
    std::vector<T> samples(complexSamples.size());
    std::transform(complexSamples.begin(), complexSamples.end(), samples.begin(),
                   [](const std::complex<T> complexSample) { return std::arg(complexSample); });
    return samples;
}

template <typename T>
static std::vector<std::complex<T>> dft(const std::vector<std::complex<T>>& complexSamples)
{
    const unsigned int N = complexSamples.size();
    std::vector<std::complex<T>> spectrum(N);

    for (unsigned int k = 0; k < N; k++)
        for (unsigned int n = 0; n < N; n++)
            spectrum[k] +=
                complexSamples[n] * (std::complex<T>)std::polar(1.0, -2.0 * M_PI * k * n / N);

    return spectrum;
}

template <typename T>
static std::vector<std::complex<T>> fftN2(std::vector<std::complex<T>>& complexSamples)
{
    const unsigned int N = nextPow2(complexSamples.size());
    complexSamples.resize(N, std::complex<T>());

    if (N == 1)
        return dft(complexSamples);

    const unsigned int M = N / 2;
    std::vector<std::complex<T>> evenComplexSamples(M);
    std::vector<std::complex<T>> oddComplexSamples(M);

    for (unsigned int k = 0; k < M; k++)
    {
        evenComplexSamples[k] = complexSamples[2 * k];
        oddComplexSamples[k] = complexSamples[2 * k + 1];
    }

    evenComplexSamples = fftN2<T>(evenComplexSamples);
    oddComplexSamples = fftN2<T>(oddComplexSamples);

    std::vector<std::complex<T>> spectrum(N);
    for (unsigned long k = 0; k < M; k++)
    {
        spectrum[k] = evenComplexSamples[k] +
                      (std::complex<T>)std::polar(1.0, -2.0 * M_PI * k / N) * oddComplexSamples[k];
        spectrum[k + M] =
            evenComplexSamples[k] -
            (std::complex<T>)std::polar(1.0, -2.0 * M_PI * k / N) * oddComplexSamples[k];
    }

    return spectrum;
}
} // namespace fourier