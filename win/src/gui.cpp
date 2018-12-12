#include "gui.hpp"
#include "com.hpp"
#include "worker.hpp"

#include <algorithm>

#include <asio.hpp>

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
int gui()
{
  using namespace nana;

  asio::io_context ctx;

  form fm { API::make_center(600, 400) };
  fm.caption("EEE Weather Station Control");

  place vbox { fm };
  vbox.div("<vert vbox gap=10 margin=[10,10,10,10] arrange=[70,110,variable]>");

  // Connection group
  group conn_mgmt { fm, "Connection", true };
  conn_mgmt.div("<conn gap=5 margin=[10,10,10,10] arrange=[50,100,100,100,100]>");
  label port_l { conn_mgmt, "Port: " };
  combox com { conn_mgmt, "", true };

  button update_btn{ conn_mgmt, "Update" };

  conn_mgmt["conn"] << port_l << com << update_btn;

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

  stop_btn.enabled(false);

  out_mgmt["daq"] << start_btn << stop_btn;

  filebox fb(false);
  fb.add_filter( ("VWS file"), ("dbase.csv"));
  fb.init_file("dbase.csv");

  // Current values group
  group curr_mgmt{ fm, "Current readings", true };
  curr_mgmt.div("<values gap=5 margin=[10,10,10,10]>");

  textbox values { curr_mgmt };
  values.multi_lines(true).editable(false);

  curr_mgmt["values"] << values;

  // General window
  vbox.field("vbox") << conn_mgmt << out_mgmt << curr_mgmt;

  vbox.collocate();

  fm.show();

  bool empty = false;

  // Code associated with GUI events
  auto const com_update = [&]() {
    auto const ports = probe_filter(ctx, enumerate());

    com.clear();
    for (auto&& port : ports) {
      com.push_back(port);
    }

    if (ports.empty()) {
      empty = true;
      com.push_back("No port found");
    } else {
      empty = false;
    }

    com.option(0);
  };

  auto update_fn = [&]() {
    update_btn.enabled(false);
    start_btn.enabled(false);
    com.enabled(false);

    com_update();

    com.enabled(true);
    start_btn.enabled(!empty);
    update_btn.enabled(true);
  };

  update_btn.events().click(update_fn);

  Worker worker;

  start_btn.events().click([&]() {
    com.enabled(false);
    start_btn.enabled(false);
    stop_btn.enabled(true);
    update_btn.enabled(false);
    browse_btn.enabled(false);
    path.enabled(false);

    std::string filename;
    path.getline(0, filename);

    worker.run(
        com.text(com.option())
      , filename
      , [&](std::string const& message) {
          values.append(message, true);
        }
      , [&](std::string const& message) {
          values.append(message, true);
          worker.stop();

          update_fn();

          com.enabled(true);
          stop_btn.enabled(false);
          start_btn.enabled(!empty);
          update_btn.enabled(true);
          browse_btn.enabled(true);
          path.enabled(true);
        }
    );
  });

  stop_btn.events().click([&]() {
    worker.stop();

    com.enabled(true);
    stop_btn.enabled(false);
    start_btn.enabled(true);
    update_btn.enabled(true);
    browse_btn.enabled(true);
    path.enabled(true);
  });

  browse_btn.events().click([&]() {
    if (fb()) {
      path.reset(fb.file());
    }
  });

  std::thread startup(update_fn);

  exec();

  startup.join();

  return EXIT_SUCCESS;
}

} // ns wsc
} // ns eee
