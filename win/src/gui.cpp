#include "gui.hpp"
#include "com.hpp"

#include <algorithm>

#include <nana/gui.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/filebox.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/textbox.hpp>

namespace eee {
namespace wsc {

// This function shall draw the graphical interface, then block until
// the user issues an exit event.
int gui(asio::io_context& ctx)
{
  using namespace nana;

  auto const ports = probe_filter(ctx, enumerate());

  form fm { API::make_center(600, 400) };
  fm.caption("EEE Weather Station Control");

  place vbox { fm };
  vbox.div("<vert vbox gap=10 margin=[10,10,10,10] arrange=[70,110,variable]>");

  // Connection group
  group conn_mgmt { fm, "Connection", true };
  conn_mgmt.div("<conn gap=5 margin=[10,10,10,10] arrange=[50,100,100,100]>");
  label port_l { conn_mgmt, "Port: " };
  combox com { conn_mgmt, "", true };
  for (auto&& port : ports) {
    com.push_back(port);
  }

  com.option(0);

  button connect_btn{ conn_mgmt, "Connect!" };
  button disconnect_btn{ conn_mgmt, "Disconnect" };
  disconnect_btn.enabled(false);

  conn_mgmt["conn"] << port_l << com << connect_btn << disconnect_btn;

  // Output group
  group out_mgmt{ fm, "Output", true };
  out_mgmt.div(
    "<vert margin=[10,10,10,10] " \
    "<gap=5 margin=[0,0,10,0] arrange=[variable,100] saveto>" \
    "<gap=5 arrange=[100,100] daq>>"
  );

  button browse_btn{ out_mgmt, "Browse...", true };
  textbox path{ out_mgmt };
  path.tip_string("path...").multi_lines(false);

  out_mgmt["saveto"] << path << browse_btn;

  button start_btn{ out_mgmt, "Start!" };
  button stop_btn{ out_mgmt, "Stop" };

  out_mgmt["daq"] << start_btn << stop_btn;

  // Current values group
  group curr_mgmt{ fm, "Current readings", true };
  curr_mgmt.div("<values gap=5 margin=[10,10,10,10]>");

  // General window
  vbox.field("vbox") << conn_mgmt << out_mgmt << curr_mgmt;

  vbox.collocate();

  fm.show();
  exec();

  return EXIT_SUCCESS;
}

} // ns wsc
} // ns eee
