#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include "Network/Socket.hpp"
#include "Network/Server.hpp"

class OSRedirector {
    private:
        std::ostringstream _oss{};
        std::streambuf *_backup{};
        std::ostream &_c;

    public:
        OSRedirector(OSRedirector &) = delete;
        OSRedirector &operator=(OSRedirector &) = delete;

        OSRedirector(std::ostream &c) : _c(c) {
            _backup = _c.rdbuf();
            _c.rdbuf(_oss.rdbuf());
        }

        ~OSRedirector() {
            _c.rdbuf(_backup);
        }

        const std::string getContent() {
            _oss << std::flush;
            return _oss.str();
        }
};

using namespace std::chrono_literals;

void server_thread(bool &alive)
{
    OSRedirector oss(std::cout);
    try {
        network::server serv(6000, 10, 0, 1, 2);
        alive = true;
        while (alive) {
            serv.receive();
            for (auto &cli : serv.get_clients()) {
                auto data = cli.pop_packets();
                for (auto &packet : data) {
                    if (packet.type == 3) {
                        serv.send(cli, 3, "messageOK");
                    }
                }
            }
            std::this_thread::yield();
        }
    } catch (const std::exception &e) {
        EXPECT_EQ(true, false);
        std::terminate();
    }
}

TEST(network, basic_test)
{
    bool alive = false;
    std::thread th(server_thread, std::ref(alive));
    while (alive == false)
        std::this_thread::sleep_for(10ms);
    network::socket sock(5000, 0, 1, 2);
    sock.connect("127.0.0.1", 6000);
    sock.send(3, "message?");

    std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now() + std::chrono::seconds(5);
    while (std::chrono::system_clock::now() < deadline) {
        auto data = sock.pop_packets();
        for (auto &packet : data) {
            if (packet.type == 3 and packet.data == "messageOK") {
                alive = false;
                th.join();
                EXPECT_EQ(true, true);
                return;
            }
        }
        std::this_thread::yield();
    }
    alive = false;
    th.join();
    EXPECT_EQ(true, false);
}

TEST(network, multiple_clients)
{
    bool alive = false;
    size_t ok = 0;
    std::thread th(server_thread, std::ref(alive));
    while (alive == false)
        std::this_thread::sleep_for(10ms);
    std::array<network::socket, 3> socks{{network::socket(5000, 0, 1, 2), network::socket(5001, 0, 1, 2), network::socket(5002, 0, 1, 2)}};
    for (auto &sock : socks) {
        sock.connect("127.0.0.1", 6000);
        sock.send(3, "message?");
    }
    std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now() + std::chrono::seconds(5);
    while (std::chrono::system_clock::now() < deadline and ok != socks.size()) {
        for (auto &sock : socks) {
            auto data = sock.pop_packets();
            for (auto &packet : data) {
                if (packet.type == 3 and packet.data == "messageOK") {
                    ok++;
                }
            }
        }
        std::this_thread::yield();
    }
    alive = false;
    th.join();
    if (ok == socks.size())
        EXPECT_EQ(true, true);
    else
        EXPECT_EQ(true, false);
}

TEST(network, connect_disconnect)
{
    bool alive = false;
    bool received = false;
    std::thread th(server_thread, std::ref(alive));
    while (alive == false)
        std::this_thread::sleep_for(10ms);
    network::socket sock(5000, 0, 1, 2);
    sock.connect("127.0.0.1", 6000);
    sock.send(3, "message?");

    std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now() + std::chrono::seconds(5);
    while (std::chrono::system_clock::now() < deadline) {
        auto data = sock.pop_packets();
        for (auto &packet : data) {
            if (packet.type == 3 and packet.data == "messageOK") {
                received = true;
            }
        }
        std::this_thread::yield();
    }
    EXPECT_EQ(received, true);
    received = false;
    sock.disconnect();
    sock.connect("127.0.0.1", 6000);
    sock.send(3, "message?");
    deadline = std::chrono::system_clock::now() + std::chrono::seconds(5);
    while (std::chrono::system_clock::now() < deadline) {
        auto data = sock.pop_packets();
        for (auto &packet : data) {
            if (packet.type == 3 and packet.data == "messageOK") {
                received = true;
            }
        }
        std::this_thread::yield();
    }
    EXPECT_EQ(received, true);
    alive = false;
    th.join();
}