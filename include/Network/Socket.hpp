#pragma once

#include <asio.hpp>
#include <thread>
#include <memory>
#include <vector>
#include <chrono>
#include <mutex>
#include <exception>

#include "Header.hpp"
#include "Packet.hpp"

namespace network {
class socket {
private:
    asio::io_context m_io_context{};
    asio::ip::udp::socket m_socket;
    std::unique_ptr<asio::ip::udp::endpoint> m_server{};
    std::unique_ptr<std::thread> m_receiver{};
    std::mutex m_mutex{};
    std::vector<packet> m_buffer{};
    uint8_t m_login_handshake;
    uint8_t m_logout_handshake;
    uint8_t m_alive_handshake;
    bool m_connected{};
public:
    socket(unsigned short port, uint8_t login_handshake, uint8_t logout_handshake, uint8_t alive_handshake)
        : m_socket(m_io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)),
        m_login_handshake(login_handshake), m_logout_handshake(logout_handshake), m_alive_handshake(alive_handshake) {
        m_socket.non_blocking(true);
    }
    ~socket() {disconnect();}
    socket(socket &) = delete;
    socket &operator=(socket &) = delete;
    socket(socket &&) = delete;
    socket &operator=(socket &&) = delete;

    /**
     *  this function is used to send a request to the server to connect
     *  @param ip the host ip
     *  @param port is the port for the ip
     *  @param login_timeout login timeout in sec, the function will throw after that delay
     *  
     */
    void connect(std::string ip, unsigned short port, long login_timeout = 3) {
        disconnect();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        try {
            std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now() + std::chrono::seconds(login_timeout);
            m_server = std::make_unique<asio::ip::udp::endpoint>(asio::ip::make_address(std::move(ip)), port);
            m_connected = true;
            m_receiver = std::make_unique<std::thread>(receive, std::ref(*this));
            send(m_login_handshake, "", 10);
            while (std::chrono::system_clock::now() < deadline) {
                auto packets = pop_packets();
                for (auto &p : packets) {
                    if (p.type == m_login_handshake)
                        return;
                }
                std::this_thread::yield();
            }
            throw std::exception();
        } catch (...) {
            disconnect();
            throw std::runtime_error("network::client::connect : could not connect to " + ip + ":" + std::to_string(port));
        }
    }

    /**
     *  this function is used to disconnect from the host and clear the buffer
     * 
     */
    void disconnect() noexcept {
        if (m_receiver) {
            m_connected = false;
            m_receiver->join();
        }
        try {
            send(m_logout_handshake, "", 10);
        } catch (...) {}
        m_server = nullptr;
        m_receiver = nullptr;
        m_buffer.clear();
    }

    /**
     *  this function is used to send packets to the server
     * 
     *  @param type type of the packet
     *  @param buffer data to send to the server
     *  @param repeat number of times to send the packets
     * 
     */
    void send(uint8_t type, const std::string &buffer = "", unsigned short repeat = 1) {
        if (!m_server)
            throw std::runtime_error("network::client::send : not connected to any host");
        size_t size = buffer.size();
        auto p = reinterpret_cast<header *>(::operator new (sizeof(header) + size));
        p->magicValue = 0x42dead42;
        p->type = type;
        p->size = buffer.size();
        std::memcpy(reinterpret_cast<uint8_t *>(p) + sizeof(header), buffer.data(), size);
        try {
            while (repeat != 0) {
                m_socket.send_to(asio::buffer(reinterpret_cast<const uint8_t *>(p), sizeof(header) + size), *m_server);
                repeat--;
            }
        } catch (...) {}
        delete p;
    }

    /**
     *  this function is used to get the remote ip
     *
     *  @return the remote address as a std::string
     * 
     */
    std::string get_remote_ip() const {
        if (!m_server)
            return "";
        return m_server->address().to_string();
    }

    /**
     *  this function is used to get the remote port
     *
     *  @return the remote port as a unsigned short
     * 
     */
    unsigned short get_remote_port() const {
        if (!m_server)
            return 0;
        return m_server->port();
    }

    /**
     *  this function is used to get the pending packets
     *
     *  @return the pending packets contained in the socket
     * 
     */
    std::vector<packet> get_packets() noexcept {
        std::lock_guard lock(m_mutex);
        return m_buffer;
    }

    /** 
     *  this function is used to move the pending packets and get them
     *
     *  @return the pending packets contained in the socket and empty them on the socket class
     * 
     */
    std::vector<packet> pop_packets() noexcept {
        std::lock_guard lock(m_mutex);
        return std::move(m_buffer);
    }
private:
    static void receive(socket &self) {
        std::chrono::system_clock::time_point chrono = std::chrono::system_clock::now();

        if (!self.m_server)
            throw std::runtime_error("network::client::receive : not connected to any host");

        while (self.m_connected) {
            if (std::chrono::system_clock::now() > chrono + std::chrono::seconds(1)) {
                chrono = std::chrono::system_clock::now();
                self.send(self.m_alive_handshake, "", 1);
            }
            asio::ip::udp::endpoint endpoint;
            uint8_t recv_str[4800];
            asio::error_code error;
            std::string data{};
            auto len = self.m_socket.receive_from(asio::buffer(recv_str, 4800), endpoint, 0, error);
            if (error == asio::error::would_block)
                continue;
            if (self.get_remote_ip() != endpoint.address().to_string() or self.get_remote_port() != endpoint.port())
                continue;
            auto ret = reinterpret_cast<const header *>(recv_str);
            if (ret->magicValue == 0x42dead42 and len == sizeof(header) + (ret->size)) {
                for (size_t i = 0; i < ret->size; i++)
                    data.push_back(((reinterpret_cast<const uint8_t *>(ret) + sizeof(header)))[i]);
                self.m_mutex.lock();
                self.m_buffer.push_back({ret->type, std::move(data)});
                self.m_mutex.unlock();
            }
            std::this_thread::yield();
        }
    }
};
}