#pragma once

#include <asio.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>

#include "Header.hpp"
#include "Packet.hpp"
#include "Client.hpp"

namespace network {
class server {
private:
    asio::io_context m_io_context{};
    asio::ip::udp::socket m_socket;
    std::vector<client> m_clients{};
    uint8_t m_login_handshake;
    uint8_t m_logout_handshake;
    uint8_t m_alive_handshake;
    size_t m_max_clients;
    bool m_allow_new_clients = true;
    std::chrono::system_clock::time_point m_check_alive = std::chrono::system_clock::now();
public:
    server(unsigned short port, size_t max_clients, uint8_t login_handshake, uint8_t logout_handshake, uint8_t alive_handshake)
        : m_socket(m_io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)),
        m_login_handshake(login_handshake), m_logout_handshake(logout_handshake), m_alive_handshake(alive_handshake), m_max_clients(max_clients) {
        m_socket.non_blocking(true);
    }
    ~server() = default;
    server(server &) = delete;
    server &operator=(server &) = delete;
    server(server &&) = delete;
    server &operator=(server &&) = delete;

    /**
     *  this function is used to send packets to a client
     *
     *  @param c client to send the packet
     *  @param type type of the request to the client as a enum
     *  @param buffer data to send to client
     *  @param repeat number of times the packet will be send
     * 
     */
    void send(const client &c, uint8_t type, const std::string &buffer = "", unsigned short repeat = 1) {
        size_t size = buffer.size();
        auto p = reinterpret_cast<header *>(::operator new (sizeof(header) + size));
        p->magicValue = 0x42dead42;
        p->type = type;
        p->size = buffer.size();
        std::memcpy(reinterpret_cast<uint8_t *>(p) + sizeof(header), buffer.data(), size);
        try {
            while (repeat != 0) {
                m_socket.send_to(asio::buffer(reinterpret_cast<const uint8_t *>(p), sizeof(header) + size), c.get_endpoint());
                repeat--;
            }
        } catch (...) {}
        delete p;
    }
    /**
     *  this function is used to receive pending packets from clients
     * 
     */
    void receive() {
        if (std::chrono::system_clock::now() > m_check_alive + std::chrono::seconds(10)) {
            m_check_alive = std::chrono::system_clock::now();
            for (size_t i = 0; i < m_clients.size(); i++) {
                if (m_clients[i].alive() == false) {
                    std::cout << "[USER] " << m_clients[i].get_ip() << " : DISCONNECTED" << std::endl;
                    m_clients.erase(m_clients.begin() + i);
                    i--;
                } else {
                    m_clients[i].alive() = false;
                }
            }
        }
        while (true) {
            asio::ip::udp::endpoint endpoint;
            uint8_t recv_str[4800];
            asio::error_code error;
            auto len = m_socket.receive_from(asio::buffer(recv_str, 4800), endpoint, 0, error);
            if (error == asio::error::would_block)
                return;
            std::string data{};
            auto ret = reinterpret_cast<const header *>(recv_str);
            if (ret->magicValue != 0x42dead42 or len != sizeof(header) + (ret->size))
                return;
            for (size_t i = 0; i < ret->size; i++)
                data.push_back(((reinterpret_cast<const uint8_t *>(ret) + sizeof(header)))[i]);
            for (size_t i = 0; i < m_clients.size(); i++) {
                if (m_clients[i].get_ip() == endpoint.address().to_string() and m_clients[i].get_port() == endpoint.port()) {
                    if (ret->type == m_alive_handshake) {
                        m_clients[i].alive() = true;
                        continue;
                    } else if (ret->type == m_logout_handshake) {
                        std::cout << "[USER] " << m_clients[i].get_ip() << " : DISCONNECTED" << std::endl;
                        m_clients.erase(m_clients.begin() + i);
                        return;
                    }
                    m_clients[i].get_packets().push_back({ret->type, std::move(data)});
                    return;
                }
            }
            if (ret->type == m_login_handshake and m_clients.size() < m_max_clients and m_allow_new_clients == true) {
                m_clients.push_back(std::move(endpoint));
                std::cout << "[USER] " << m_clients.back().get_ip() << " : CONNECTED" << std::endl;
                m_check_alive = std::chrono::system_clock::now();
                send(m_clients.back(), m_login_handshake, "", 10);
            }
        }
    }
    /**
     *  this function is used to get the clients
     *
     *  @return client list
     * 
     */
    std::vector<client> &get_clients() noexcept {
        return m_clients;
    }
    /**
     *  this function is used to lock or unlock new connections
     * 
     *  @param value choice
     */
    void allow_new_connections(bool value) {m_allow_new_clients = value;}
};
}