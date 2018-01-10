#include <chrono>
#include <string>
#include <thread>

#include "prometheus/exposer.h"

#include "CivetServer.h"
#include "handler.h"

namespace prometheus {
Exposer& Exposer::GetInstance() {
  static Exposer g_exposer{"127.0.0.1:12345"};
  return g_exposer;
}

Exposer::Exposer(const std::string& bind_address, const std::string& uri)
    : server_(new CivetServer{
          {"listening_ports", bind_address.c_str(), "num_threads", "2"}}),
      exposer_registry_(std::make_shared<Registry>()),
      metrics_handler_(
          new detail::MetricsHandler{collectables_, *exposer_registry_}),
      uri_(uri) {
  RegisterCollectable(exposer_registry_);
  server_->addHandler(uri, metrics_handler_.get());
}

Exposer::~Exposer() { server_->removeHandler(uri_); }

void Exposer::RegisterCollectable(
    const std::weak_ptr<Collectable>& collectable) {
  collectables_.push_back(collectable);
}
}  // namespace prometheus
