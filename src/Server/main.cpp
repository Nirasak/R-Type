#include "ServerCore.hpp"
#include "ThreadPool.hpp"

int main()
{
    srand(time(NULL));
    try {
        thread_pool pool;
        for (size_t i = 0; i < 8; i++) {
            pool.add([i]() {
                while (true) {
                    try {
                        server_core core(6000 + i);
                        core.run();
                    } catch (const ecs::exception &e) {
                        std::cout << e.what() << " on port " << std::to_string(6000 + i) << std::endl;
                    }
                }
            });
        }
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        return (84);
    }
    return (0);
}