
#include "gui.h"

#include <imgui.h>
#include <implot.h>
#include <implot3d.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <algorithm>


static const ImVec4 palette[9] = {
	ImVec4(0.9f, 0.1f, 0.1f, 1.0f), // Red
	ImVec4(0.2f, 0.5f, 0.7f, 1.0f), // Blue
	ImVec4(0.3f, 0.7f, 0.3f, 1.0f), // Green
	ImVec4(0.6f, 0.3f, 0.6f, 1.0f), // Purple
	ImVec4(1.0f, 0.5f, 0.0f, 1.0f), // Orange
	ImVec4(1.0f, 1.0f, 0.2f, 1.0f), // Yellow
	ImVec4(0.7f, 0.3f, 0.2f, 1.0f), // Brown
	ImVec4(1.0f, 0.5f, 0.7f, 1.0f), // Pink
	ImVec4(0.6f, 0.6f, 0.6f, 1.0f)  // Gray
};


namespace theoretica {
namespace gui {

	// Keep count of windows for GLFW termination
	int window::window_count = 0;

	void apply_style();


	window::window(int width, int height, const std::string& title) {

		if (window_count == 0) {
			if (!glfwInit()) {
				std::cerr << "Failed to initialize GLFW" << std::endl;
				return;
			}
		}

		window_count++;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

		window_id = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		
		// TO-DO: Handle failure
		if (!window_id)
			return;

		glfwMakeContextCurrent(window_id);

		// Enable V-Sync
		glfwSwapInterval(1);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();
		ImPlot3D::CreateContext();

		apply_style();

		ImGui_ImplGlfw_InitForOpenGL(window_id, true);
		ImGui_ImplOpenGL3_Init("#version 130");
	}

	window::~window() {

		if (window_id) {
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			
			ImPlot3D::DestroyContext();
			ImPlot::DestroyContext();
			ImGui::DestroyContext();

			glfwDestroyWindow(window_id);
			
			window_count--;
			if (window_count == 0)
				glfwTerminate();
		}
	}

	void apply_style() {

		ImGui::StyleColorsLight(); 
		ImGuiStyle& style = ImGui::GetStyle();
		style.AntiAliasedLines = true;
		style.AntiAliasedLinesUseTex = true;
		style.AntiAliasedFill = true;

		// Force high quality curve generation
		style.CurveTessellationTol = 0.1f;	   
		style.CircleTessellationMaxError = 0.1f; 
		ImGui::GetIO().BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset; 

		style.WindowRounding	= 4.0f;
		style.FrameRounding	 = 4.0f;
		style.PopupRounding	 = 4.0f;
		style.GrabRounding	  = 4.0f;
		style.ChildRounding	 = 4.0f;
		style.TabRounding	   = 4.0f;

		style.WindowPadding	 = ImVec2(16.0f, 16.0f);
		style.FramePadding	  = ImVec2(10.0f, 6.0f);
		style.ItemSpacing	   = ImVec2(16.0f, 16.0f);
		style.ItemInnerSpacing  = ImVec2(8.0f, 6.0f);
		
		// Remove borders
		style.WindowBorderSize  = 0.0f;
		style.FrameBorderSize   = 0.0f;
		style.ChildBorderSize   = 0.0f;
		style.PopupBorderSize   = 1.0f;

		ImVec4* colors = style.Colors;
		const ImVec4 bg_figure	 = ImVec4(0.96f, 0.96f, 0.96f, 1.00f); 
		const ImVec4 text_main	 = ImVec4(0.10f, 0.10f, 0.10f, 1.00f); 
		const ImVec4 text_muted	= ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		const ImVec4 accent		= ImVec4(0.12f, 0.45f, 0.70f, 1.00f);
		const ImVec4 divisor = ImVec4(0.85f, 0.85f, 0.85f, 0.50f); 
		
		colors[ImGuiCol_Text]				  = text_main;
		colors[ImGuiCol_TextDisabled]		  = text_muted;
		colors[ImGuiCol_WindowBg]			  = bg_figure;
		colors[ImGuiCol_ChildBg]			   = bg_figure; 
		colors[ImGuiCol_PopupBg]			   = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		colors[ImGuiCol_Border]				= ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		colors[ImGuiCol_FrameBg]			   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_FrameBgHovered]		= ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_FrameBgActive]		 = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
		colors[ImGuiCol_TitleBg]			   = bg_figure;
		colors[ImGuiCol_TitleBgActive]		 = bg_figure;
		colors[ImGuiCol_Button]				= ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_ButtonHovered]		 = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
		colors[ImGuiCol_ButtonActive]		  = accent;
		colors[ImGuiCol_TableBorderLight]	  = divisor;
		colors[ImGuiCol_TableBorderStrong]	 = divisor;

		ImPlotStyle& plot_style = ImPlot::GetStyle();
		plot_style.PlotBorderSize = 0.0f;	   
		plot_style.PlotPadding = ImVec2(10.0f, 10.0f);
		
		plot_style.Colors[ImPlotCol_FrameBg]		= bg_figure;					  
		plot_style.Colors[ImPlotCol_PlotBg]		 = bg_figure;
		plot_style.Colors[ImPlotCol_PlotBorder]	 = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		plot_style.Colors[ImPlotCol_AxisText]	   = text_main;					  
		plot_style.Colors[ImPlotCol_AxisGrid]	   = ImVec4(0.85f, 0.85f, 0.85f, 1.0f); 
		plot_style.Colors[ImPlotCol_LegendBg]	   = ImVec4(0.96f, 0.96f, 0.96f, 0.8f); 
		plot_style.Colors[ImPlotCol_LegendBorder]   = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		plot_style.Colors[ImPlotCol_LegendText]	 = text_main;

		plot_style.Colormap = ImPlotColormap_Plasma; 


		ImPlot3DStyle& plot3d_style = ImPlot3D::GetStyle();
		plot3d_style.LineWeight = 2.5f; 
		plot3d_style.MarkerSize = 6.0f;
		plot3d_style.PlotPadding = ImVec2(2.0f, 2.0f); 
		
		plot3d_style.Colors[ImPlot3DCol_FrameBg]		= bg_figure;
		plot3d_style.Colors[ImPlot3DCol_PlotBg]		 = bg_figure;
		plot3d_style.Colors[ImPlot3DCol_AxisGrid]	   = ImVec4(0.80f, 0.80f, 0.80f, 1.0f);
		plot3d_style.Colors[ImPlot3DCol_AxisText]	   = text_main;
		plot3d_style.Colors[ImPlot3DCol_LegendBg]	   = ImVec4(0.96f, 0.96f, 0.96f, 0.8f);
		plot3d_style.Colors[ImPlot3DCol_LegendBorder]   = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		plot3d_style.Colors[ImPlot3DCol_LegendText]	 = text_main;

		plot3d_style.Colormap = ImPlot3DColormap_Dark;

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
		const float font_size = 18.0f;
		
		ImFontConfig font_config;
		font_config.OversampleH = 2;
		font_config.OversampleV = 2;
		font_config.PixelSnapH = false;

	#ifdef _WIN32
		io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\DejaVuSans.ttf", font_size, &font_config);
	#elif defined(__APPLE__)
		io.Fonts->AddFontFromFileTTF("/Library/Fonts/DejaVuSans.ttf", font_size, &font_config);
	#else
		io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", font_size, &font_config);
	#endif

	}


	void window::resize(int width, int height) {

		if (window_id)
			glfwSetWindowSize(window_id, width, height);
	}


	void window::set_title(const std::string& title) {

		if (window_id)
			glfwSetWindowTitle(window_id, title.c_str());
	}


	void window::plot(const th::histogram& h, const std::string& title, int row, int col) {

		const std::vector<unsigned int>& bin_counts = h.bins();

		int bins = h.bins().size();
		float range_min = h.range_lower();
		float range_max = h.range_upper();
		float bin_width = (range_max - range_min) / bins;
		std::string plot_title = title;
		int color_id = (int) items.size();

		std::vector<double> data (bin_counts.size());
		std::vector<double> values (bin_counts.size());

		for (size_t i = 0; i < bin_counts.size(); ++i) {
			data[i]	 = bin_counts[i];
			values[i]   = range_min + (i + 0.5f) * bin_width;
		}

		auto plot_function = [=](const ImVec2& size) {

			if (ImPlot::BeginPlot(title.c_str(), size)) {

				ImPlot::SetupAxes("Value", "Count");
				ImPlot::SetupAxisFormat(ImAxis_Y1, "%g");

				ImPlot::SetupAxesLimits(
					values.front() - bin_width,
					values.back() + bin_width,
					0,
					th::max(data) * 1.10f,
					ImPlotCond_Once
				);

				// Set custom color palette
				ImVec4 color = palette[color_id % 9];
				ImPlotSpec spec(
					ImPlotProp_FillColor, color, 
					ImPlotProp_FillAlpha, 0.75f,
					ImPlotProp_LineColor, color
				);
				
				// Render histogram as bar plot
				ImPlot::PlotBars("Data", values.data(), data.data(), bins, bin_width, spec);
				ImPlot::EndPlot();
			}
		};

		items.push_back({row, col, plot_function});
	}


	void window::plot(
		const std::function<th::real(th::real)>& func, 
		th::real x_min, th::real x_max, 
		int samples, const std::string& title, int row, int col)  {

		std::vector<double> x_values (samples);
		std::vector<double> y_values (samples);
		double step = (x_max - x_min) / (samples - 1);

		for (int i = 0; i < samples; ++i) {
			x_values[i] = x_min + i * step;
			y_values[i] = func(x_values[i]);
		}

		int color_id = (int) items.size();
		
		std::string plot_title = title;
		items.push_back({row, col, [=](const ImVec2& size) {

			if (ImPlot::BeginPlot(plot_title.c_str(), size)) {
				ImPlot::SetupAxes("x", "f(x)");

				ImVec4 color = palette[color_id % 9];
				
				ImPlotSpec spec(
					ImPlotProp_LineColor, color, 
					ImPlotProp_LineWeight, 2.5f
				);
				
				ImPlot::PlotLine("f(x)", x_values.data(), y_values.data(), x_values.size(), spec);
				ImPlot::EndPlot();
			}
		}});
	}


	void window::plot(const th::ode::ode_solution& sol, const std::string& title, int row, int col) {
		
		size_t n = sol.t.size();
		std::vector<double> x_values(n), y_values(n), z_values(n);
		for (size_t i = 0; i < n; ++i) {
			x_values[i] = static_cast<double>(sol.x[i][0]);
			y_values[i] = static_cast<double>(sol.x[i][1]);
			z_values[i] = static_cast<double>(sol.x[i][2]);
		}

		int color_id = (int) items.size();
		
		std::string plot_title = title;
		items.push_back({row, col, [=](const ImVec2& size) {
			if (ImPlot3D::BeginPlot(plot_title.c_str(), size)) {
				ImPlot3D::SetupAxes("x", "y", "z");

				ImVec4 color = palette[color_id % 9];
				ImPlot3DSpec spec(
					ImPlotProp_LineColor, color, 
					ImPlotProp_LineWeight, 1.0f
				);

				ImPlot3D::PlotLine("x(t)", x_values.data(), y_values.data(), z_values.data(), n, spec);
				ImPlot3D::EndPlot();
			}
		}});
	}


	void window::plot(const std::vector<th::vec3>& points, const std::string& title, int row, int col) {

		const size_t n = points.size();
		std::vector<double> x_values(n);
		std::vector<double> y_values(n);
		std::vector<double> z_values(n);
		
		for (size_t i = 0; i < n; ++i) {
			x_values[i] = points[i][0];
			y_values[i] = points[i][1];
			z_values[i] = points[i][2];
		}

		int color_id = (int) items.size();
		std::string plot_title = title;

		items.push_back({row, col, [=](const ImVec2& size) {

			if (ImPlot3D::BeginPlot(plot_title.c_str(), size)) {
				ImPlot3D::SetupAxes("x", "y", "z");

				ImVec4 color = palette[color_id % 9];
				ImPlot3DSpec spec(
					ImPlotProp_LineColor, color, 
					ImPlotProp_LineWeight, 1.0f
				);

				ImPlot3D::PlotScatter("Data Points", x_values.data(), y_values.data(), z_values.data(), n, spec);
				ImPlot3D::EndPlot();
			}
		}});
	}


	bool window::is_open() const {
		return window_id && !glfwWindowShouldClose(window_id);
	}


	void window::render() {

		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
		ImGui::Begin("Theoretica GUI", nullptr, window_flags);
		
		if (!items.empty()) {

			// Determine grid dimensions
			int max_row = 0, max_col = 0;
			for (const auto& item : items) {
				if (item.row > max_row) max_row = item.row;
				if (item.col > max_col) max_col = item.col;
			}

			int rows = max_row + 1;
			int cols = max_col + 1;

			// Determine available space for grid elements
			ImVec2 avail = ImGui::GetContentRegionAvail();
			float cell_height = th::max(10.0f, avail.y / rows);

			bool res = ImGui::BeginTable(
				"GridTable", cols,
				ImGuiTableFlags_BordersInner |
				ImGuiTableFlags_SizingStretchSame
			);

			if (res) {

				for (int r = 0; r < rows; ++r) {

					ImGui::TableNextRow(ImGuiTableRowFlags_None, cell_height);

					for (int c = 0; c < cols; ++c) {

						ImGui::TableSetColumnIndex(c);
						
						// Count items in the same cell
						int items_in_cell = 0;
						for (const auto& item : items) {
							if (item.row == r && item.col == c) items_in_cell++;
						}

						if (items_in_cell > 0) {
							ImVec2 item_size (-1, cell_height / items_in_cell);
							for (auto& item : items) {
								if (item.row == r && item.col == c) {
									item.draw(item_size);
								}
							}
						}
					}
				}

				ImGui::EndTable();
			}
		}
		
		ImGui::End();
		ImGui::Render();

		int display_w, display_h;
		glfwGetFramebufferSize(window_id, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);

		glClearColor(0.07f, 0.07f, 0.08f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window_id);
	}


	void window::show() {
		
		if (!window_id)
			return;
		
		while (is_open())
			render();
	}


	void window::clear() {
		items.clear();
	}

}}
