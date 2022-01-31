#pragma once

#include <asio.hpp>
#include "Packet.hpp"

namespace network {
class client {
private:
    asio::ip::udp::endpoint endpoint;
    std::vector<packet> buffer{};
    bool m_alive = true;
public:
    client(asio::ip::udp::endpoint &&endp) : endpoint(std::move(endp)) {}
    ~client() = default;
    std::string get_ip() const {return endpoint.address().to_string();}
    unsigned short get_port() {return endpoint.port();}
    const asio::ip::udp::endpoint &get_endpoint() const noexcept {return endpoint;}
    std::vector<packet> &get_packets() noexcept {return buffer;}
    std::vector<packet> pop_packets() noexcept {return std::move(buffer);}
    bool &alive() {return m_alive;}
};
}