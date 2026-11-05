#include <exception>
#include <memory>

#include <rclcpp/rclcpp.hpp>

#include <ublox_gps/node.hpp>

int main(int argc, char** argv) {
  // Force flush of the stdout buffer.
  setvbuf(stdout, nullptr, _IONBF, BUFSIZ);

  rclcpp::init(argc, argv);

  std::shared_ptr<ublox_node::UbloxNode> node;
  try {
    node = std::make_shared<ublox_node::UbloxNode>(rclcpp::NodeOptions());
  } catch (const std::exception & e) {
    // Such as the serial port not opening, or the device not answering
    RCLCPP_FATAL(rclcpp::get_logger("ublox_gps_node"), "Could not start: %s", e.what());
    rclcpp::shutdown();
    return 1;
  }

  rclcpp::spin(node);

  const bool read_failed = rclcpp::contexts::get_global_default_context()->shutdown_reason() ==
                           ublox_gps::kReadErrorShutdownReason;
  node.reset();
  rclcpp::shutdown();

  return read_failed ? 1 : 0;
}
