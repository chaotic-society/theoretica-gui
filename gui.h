#pragma once

#include "theoretica.h"

#include <string>
#include <functional>
#include <vector>

struct GLFWwindow;
struct ImVec2;


namespace theoretica {
namespace gui {

	class window {
	public:

		/// Initialize a new window and underlying contexts
		window (int width = 1200, int height = 800, const std::string& title = "Theoretica GUI");
		
		// Destroy window context.
		~window();

		// Prevent copying
		window(const window&) = delete;
		window& operator=(const window&) = delete;

		/// Change the size of the window
		void resize(int width, int height);

		/// Change the title of the window
		void set_title(const std::string& title);


		// Plotting functions


		/// Plot a histogram
		void plot(
			const th::histogram& h,
			const std::string& title = "",
			int row = 0, int col = 0
		);


		// Plot a function
		void plot(
			const std::function<th::real(th::real)>& func,
			th::real x_min, th::real x_max, int samples = 1000, 
			const std::string& title = "", int row = 0, int col = 0
		);


		// Plot an ODE solution as a curve in 3D space
		void plot(
			const th::ode::ode_solution& sol,
			const std::string& title = "",
			int row = 0, int col = 0
		);


		// Plot a set of points in 3D space as a scatter plot
		void plot(
			const std::vector<th::vec3>& points,
			const std::string& title = "",
			int row = 0, int col = 0
		);


		// Rendering

		/// Open and render the window, blocking execution
		/// until it is closed.
		void show();

		/// Check whether the window is open.
		bool is_open() const;

		/// Render a single frame, instead of blocking
		/// execution. May be used for non-blocking rendering
		/// or custom render loops.
		void render();

		/// Remove all plots
		void clear();

	private:

		// GLFW window handle
		GLFWwindow* window_id;

		// GLFW window count
		static int window_count;

		// Internal rendering callback registry
		struct render_item {

			/// Vertical grid position
			int row;

			/// Horizontal grid position
			int col;

			/// Rendering callback
			std::function<void(const ImVec2&)> draw;
		};

		/// Items registered for rendering
		std::vector<render_item> items;
	};

}}