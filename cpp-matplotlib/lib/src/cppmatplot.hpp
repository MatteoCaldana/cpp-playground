
#include <pybind11/embed.h> 
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace py::literals;

class pyplot {
public:
  pyplot() : guard() {};

  
template<typename ...Args>
py::object Annotation(Args&&... args) { return plt.attr("Annotation")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object Arrow(Args&&... args) { return plt.attr("Arrow")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object Artist(Args&&... args) { return plt.attr("Artist")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object AutoLocator(Args&&... args) { return plt.attr("AutoLocator")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object Axes(Args&&... args) { return plt.attr("Axes")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object Button(Args&&... args) { return plt.attr("Button")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object Circle(Args&&... args) { return plt.attr("Circle")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object Figure(Args&&... args) { return plt.attr("Figure")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object FigureCanvasBase(Args&&... args) { return plt.attr("FigureCanvasBase")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object FixedFormatter(Args&&... args) { return plt.attr("FixedFormatter")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object FixedLocator(Args&&... args) { return plt.attr("FixedLocator")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object FormatStrFormatter(Args&&... args) { return plt.attr("FormatStrFormatter")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object Formatter(Args&&... args) { return plt.attr("Formatter")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object FuncFormatter(Args&&... args) { return plt.attr("FuncFormatter")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object GridSpec(Args&&... args) { return plt.attr("GridSpec")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object IndexLocator(Args&&... args) { return plt.attr("IndexLocator")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object Line2D(Args&&... args) { return plt.attr("Line2D")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object LinearLocator(Args&&... args) { return plt.attr("LinearLocator")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object Locator(Args&&... args) { return plt.attr("Locator")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object LogFormatter(Args&&... args) { return plt.attr("LogFormatter")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object LogFormatterExponent(Args&&... args) { return plt.attr("LogFormatterExponent")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object LogFormatterMathtext(Args&&... args) { return plt.attr("LogFormatterMathtext")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object LogLocator(Args&&... args) { return plt.attr("LogLocator")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object MaxNLocator(Args&&... args) { return plt.attr("MaxNLocator")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object MouseButton(Args&&... args) { return plt.attr("MouseButton")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object MultipleLocator(Args&&... args) { return plt.attr("MultipleLocator")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object Normalize(Args&&... args) { return plt.attr("Normalize")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object NullFormatter(Args&&... args) { return plt.attr("NullFormatter")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object NullLocator(Args&&... args) { return plt.attr("NullLocator")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object Number(Args&&... args) { return plt.attr("Number")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object PolarAxes(Args&&... args) { return plt.attr("PolarAxes")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object Polygon(Args&&... args) { return plt.attr("Polygon")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object Rectangle(Args&&... args) { return plt.attr("Rectangle")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object ScalarFormatter(Args&&... args) { return plt.attr("ScalarFormatter")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object Slider(Args&&... args) { return plt.attr("Slider")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object Subplot(Args&&... args) { return plt.attr("Subplot")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object SubplotSpec(Args&&... args) { return plt.attr("SubplotSpec")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object Text(Args&&... args) { return plt.attr("Text")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object TickHelper(Args&&... args) { return plt.attr("TickHelper")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object Widget(Args&&... args) { return plt.attr("Widget")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object _IoffContext(Args&&... args) { return plt.attr("_IoffContext")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object _IonContext(Args&&... args) { return plt.attr("_IonContext")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object _auto_draw_if_interactive(Args&&... args) { return plt.attr("_auto_draw_if_interactive")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object _backend_mod(Args&&... args) { return plt.attr("_backend_mod")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object _copy_docstring_and_deprecators(Args&&... args) { return plt.attr("_copy_docstring_and_deprecators")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object _get_required_interactive_framework(Args&&... args) { return plt.attr("_get_required_interactive_framework")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object _setup_pyplot_info_docstrings(Args&&... args) { return plt.attr("_setup_pyplot_info_docstrings")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object _warn_if_gui_out_of_main_thread(Args&&... args) { return plt.attr("_warn_if_gui_out_of_main_thread")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object _xkcd(Args&&... args) { return plt.attr("_xkcd")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object acorr(Args&&... args) { return plt.attr("acorr")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object angle_spectrum(Args&&... args) { return plt.attr("angle_spectrum")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object annotate(Args&&... args) { return plt.attr("annotate")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object arrow(Args&&... args) { return plt.attr("arrow")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object autoscale(Args&&... args) { return plt.attr("autoscale")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object autumn(Args&&... args) { return plt.attr("autumn")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object axes(Args&&... args) { return plt.attr("axes")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object axhline(Args&&... args) { return plt.attr("axhline")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object axhspan(Args&&... args) { return plt.attr("axhspan")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object axis(Args&&... args) { return plt.attr("axis")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object axline(Args&&... args) { return plt.attr("axline")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object axvline(Args&&... args) { return plt.attr("axvline")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object axvspan(Args&&... args) { return plt.attr("axvspan")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object bar(Args&&... args) { return plt.attr("bar")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object bar_label(Args&&... args) { return plt.attr("bar_label")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object barbs(Args&&... args) { return plt.attr("barbs")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object barh(Args&&... args) { return plt.attr("barh")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object bone(Args&&... args) { return plt.attr("bone")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object box(Args&&... args) { return plt.attr("box")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object boxplot(Args&&... args) { return plt.attr("boxplot")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object broken_barh(Args&&... args) { return plt.attr("broken_barh")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object cla(Args&&... args) { return plt.attr("cla")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object clabel(Args&&... args) { return plt.attr("clabel")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object clf(Args&&... args) { return plt.attr("clf")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object clim(Args&&... args) { return plt.attr("clim")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object close(Args&&... args) { return plt.attr("close")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object cohere(Args&&... args) { return plt.attr("cohere")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object colorbar(Args&&... args) { return plt.attr("colorbar")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object colormaps(Args&&... args) { return plt.attr("colormaps")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object connect(Args&&... args) { return plt.attr("connect")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object contour(Args&&... args) { return plt.attr("contour")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object contourf(Args&&... args) { return plt.attr("contourf")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object cool(Args&&... args) { return plt.attr("cool")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object copper(Args&&... args) { return plt.attr("copper")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object csd(Args&&... args) { return plt.attr("csd")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object cycler(Args&&... args) { return plt.attr("cycler")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object delaxes(Args&&... args) { return plt.attr("delaxes")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object disconnect(Args&&... args) { return plt.attr("disconnect")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object draw(Args&&... args) { return plt.attr("draw")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object draw_all(Args&&... args) { return plt.attr("draw_all")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object draw_if_interactive(Args&&... args) { return plt.attr("draw_if_interactive")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object errorbar(Args&&... args) { return plt.attr("errorbar")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object eventplot(Args&&... args) { return plt.attr("eventplot")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object figaspect(Args&&... args) { return plt.attr("figaspect")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object figimage(Args&&... args) { return plt.attr("figimage")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object figlegend(Args&&... args) { return plt.attr("figlegend")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object fignum_exists(Args&&... args) { return plt.attr("fignum_exists")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object figtext(Args&&... args) { return plt.attr("figtext")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object figure(Args&&... args) { return plt.attr("figure")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object fill(Args&&... args) { return plt.attr("fill")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object fill_between(Args&&... args) { return plt.attr("fill_between")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object fill_betweenx(Args&&... args) { return plt.attr("fill_betweenx")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object findobj(Args&&... args) { return plt.attr("findobj")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object flag(Args&&... args) { return plt.attr("flag")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object gca(Args&&... args) { return plt.attr("gca")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object gcf(Args&&... args) { return plt.attr("gcf")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object gci(Args&&... args) { return plt.attr("gci")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object get(Args&&... args) { return plt.attr("get")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object get_backend(Args&&... args) { return plt.attr("get_backend")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object get_cmap(Args&&... args) { return plt.attr("get_cmap")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object get_current_fig_manager(Args&&... args) { return plt.attr("get_current_fig_manager")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object get_figlabels(Args&&... args) { return plt.attr("get_figlabels")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object get_fignums(Args&&... args) { return plt.attr("get_fignums")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object get_plot_commands(Args&&... args) { return plt.attr("get_plot_commands")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object get_scale_names(Args&&... args) { return plt.attr("get_scale_names")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object getp(Args&&... args) { return plt.attr("getp")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object ginput(Args&&... args) { return plt.attr("ginput")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object gray(Args&&... args) { return plt.attr("gray")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object grid(Args&&... args) { return plt.attr("grid")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object hexbin(Args&&... args) { return plt.attr("hexbin")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object hist(Args&&... args) { return plt.attr("hist")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object hist2d(Args&&... args) { return plt.attr("hist2d")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object hlines(Args&&... args) { return plt.attr("hlines")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object hot(Args&&... args) { return plt.attr("hot")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object hsv(Args&&... args) { return plt.attr("hsv")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object imread(Args&&... args) { return plt.attr("imread")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object imsave(Args&&... args) { return plt.attr("imsave")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object imshow(Args&&... args) { return plt.attr("imshow")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object inferno(Args&&... args) { return plt.attr("inferno")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object install_repl_displayhook(Args&&... args) { return plt.attr("install_repl_displayhook")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object interactive(Args&&... args) { return plt.attr("interactive")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object ioff(Args&&... args) { return plt.attr("ioff")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object ion(Args&&... args) { return plt.attr("ion")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object isinteractive(Args&&... args) { return plt.attr("isinteractive")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object jet(Args&&... args) { return plt.attr("jet")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object legend(Args&&... args) { return plt.attr("legend")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object locator_params(Args&&... args) { return plt.attr("locator_params")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object loglog(Args&&... args) { return plt.attr("loglog")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object magma(Args&&... args) { return plt.attr("magma")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object magnitude_spectrum(Args&&... args) { return plt.attr("magnitude_spectrum")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object margins(Args&&... args) { return plt.attr("margins")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object matshow(Args&&... args) { return plt.attr("matshow")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object minorticks_off(Args&&... args) { return plt.attr("minorticks_off")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object minorticks_on(Args&&... args) { return plt.attr("minorticks_on")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object new_figure_manager(Args&&... args) { return plt.attr("new_figure_manager")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object nipy_spectral(Args&&... args) { return plt.attr("nipy_spectral")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object pause(Args&&... args) { return plt.attr("pause")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object pcolor(Args&&... args) { return plt.attr("pcolor")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object pcolormesh(Args&&... args) { return plt.attr("pcolormesh")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object phase_spectrum(Args&&... args) { return plt.attr("phase_spectrum")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object pie(Args&&... args) { return plt.attr("pie")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object pink(Args&&... args) { return plt.attr("pink")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object plasma(Args&&... args) { return plt.attr("plasma")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object plot(Args&&... args) { return plt.attr("plot")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object plot_date(Args&&... args) { return plt.attr("plot_date")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object plotting(Args&&... args) { return plt.attr("plotting")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object polar(Args&&... args) { return plt.attr("polar")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object prism(Args&&... args) { return plt.attr("prism")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object psd(Args&&... args) { return plt.attr("psd")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object quiver(Args&&... args) { return plt.attr("quiver")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object quiverkey(Args&&... args) { return plt.attr("quiverkey")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object rc(Args&&... args) { return plt.attr("rc")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object rc_context(Args&&... args) { return plt.attr("rc_context")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object rcdefaults(Args&&... args) { return plt.attr("rcdefaults")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object register_cmap(Args&&... args) { return plt.attr("register_cmap")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object rgrids(Args&&... args) { return plt.attr("rgrids")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object savefig(Args&&... args) { return plt.attr("savefig")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object sca(Args&&... args) { return plt.attr("sca")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object scatter(Args&&... args) { return plt.attr("scatter")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object sci(Args&&... args) { return plt.attr("sci")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object semilogx(Args&&... args) { return plt.attr("semilogx")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object semilogy(Args&&... args) { return plt.attr("semilogy")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object set_cmap(Args&&... args) { return plt.attr("set_cmap")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object set_loglevel(Args&&... args) { return plt.attr("set_loglevel")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object setp(Args&&... args) { return plt.attr("setp")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object show(Args&&... args) { return plt.attr("show")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object specgram(Args&&... args) { return plt.attr("specgram")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object spring(Args&&... args) { return plt.attr("spring")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object spy(Args&&... args) { return plt.attr("spy")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object stackplot(Args&&... args) { return plt.attr("stackplot")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object stairs(Args&&... args) { return plt.attr("stairs")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object stem(Args&&... args) { return plt.attr("stem")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object step(Args&&... args) { return plt.attr("step")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object streamplot(Args&&... args) { return plt.attr("streamplot")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object subplot(Args&&... args) { return plt.attr("subplot")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object subplot2grid(Args&&... args) { return plt.attr("subplot2grid")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object subplot_mosaic(Args&&... args) { return plt.attr("subplot_mosaic")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object subplot_tool(Args&&... args) { return plt.attr("subplot_tool")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object subplots(Args&&... args) { return plt.attr("subplots")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object subplots_adjust(Args&&... args) { return plt.attr("subplots_adjust")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object summer(Args&&... args) { return plt.attr("summer")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object suptitle(Args&&... args) { return plt.attr("suptitle")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object switch_backend(Args&&... args) { return plt.attr("switch_backend")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object table(Args&&... args) { return plt.attr("table")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object text(Args&&... args) { return plt.attr("text")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object thetagrids(Args&&... args) { return plt.attr("thetagrids")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object tick_params(Args&&... args) { return plt.attr("tick_params")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object ticklabel_format(Args&&... args) { return plt.attr("ticklabel_format")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object tight_layout(Args&&... args) { return plt.attr("tight_layout")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object title(Args&&... args) { return plt.attr("title")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object tricontour(Args&&... args) { return plt.attr("tricontour")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object tricontourf(Args&&... args) { return plt.attr("tricontourf")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object tripcolor(Args&&... args) { return plt.attr("tripcolor")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object triplot(Args&&... args) { return plt.attr("triplot")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object twinx(Args&&... args) { return plt.attr("twinx")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object twiny(Args&&... args) { return plt.attr("twiny")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object uninstall_repl_displayhook(Args&&... args) { return plt.attr("uninstall_repl_displayhook")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object violinplot(Args&&... args) { return plt.attr("violinplot")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object viridis(Args&&... args) { return plt.attr("viridis")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object vlines(Args&&... args) { return plt.attr("vlines")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object waitforbuttonpress(Args&&... args) { return plt.attr("waitforbuttonpress")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object winter(Args&&... args) { return plt.attr("winter")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object xcorr(Args&&... args) { return plt.attr("xcorr")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object xkcd(Args&&... args) { return plt.attr("xkcd")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object xlabel(Args&&... args) { return plt.attr("xlabel")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object xlim(Args&&... args) { return plt.attr("xlim")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object xscale(Args&&... args) { return plt.attr("xscale")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object xticks(Args&&... args) { return plt.attr("xticks")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object ylabel(Args&&... args) { return plt.attr("ylabel")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object ylim(Args&&... args) { return plt.attr("ylim")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object yscale(Args&&... args) { return plt.attr("yscale")(std::forward<Args>(args)...); };


template<typename ...Args>
py::object yticks(Args&&... args) { return plt.attr("yticks")(std::forward<Args>(args)...); };



private:
  py::scoped_interpreter guard;
  py::object plt = py::module_::import("matplotlib.pyplot");
};
