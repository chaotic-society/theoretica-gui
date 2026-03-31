
#include "theoretica.h"
#include "gui.h"

#include <vector>
#include <cmath>
#include <iostream>

using namespace th;


// Lorenz chaotic attractor system
vec3 lorenz(real t, vec3 v) {

	const real a = 10.0;
	const real b = 28.0;
	const real c = 8.0 / 3.0;

	const real x = v[0];
	const real y = v[1];
	const real z = v[2];

	return {
		a * (y - x),
		x * (b - z) - y,
		x * y - c * z
	};
}


int main() {

	// Functions to plot
	auto damped_sine = [](real x) {
		return th::sin(10.0 * x) * th::exp(-x);
	};

	auto exp_decay = [](real x) {
		return th::exp(-x);
	};

	// Gaussian sample
	PRNG g = PRNG::xoshiro(42);
	std::vector<real> sample (10000);
	for (size_t i = 0; i < sample.size(); ++i)
		sample[i] = rand_gaussian(0.0, 1.0, g);

	// Create histogram from sample with 25 bins
	histogram hist (sample, 25);


	// Solve Lorenz attractor
	vec<real> x0 = {0.2, -0.3, 0.1};
	auto solution = ode::solve_rk4(lorenz, x0, 0.0, 40.0, 0.005);

	// Create window and show results
	gui::window app (1024, 720, "Theoretica GUI");

	app.plot(solution, "Lorenz Attractor");
	app.plot(damped_sine, 0.0, 5.0, 500, "Damped Sine Wave", 1, 0);
	app.plot(exp_decay, 0.0, 5.0, 500, "Exponential Decay", 1, 1);
	app.plot(hist, "Histogram", 0, 1);

	app.show();
}
