#include <sycl/sycl.hpp>
#include <iostream>
#include <chrono>

using namespace sycl;
using namespace std;
using namespace chrono;

void test_kernel(queue& q) {
    q.submit([&](handler& h) {
        stream out(1024, 256, h); // buffer size and max statement size
        h.parallel_for(range<1>(10), [=](id<1> i){
            out << "Hello from core " << i[0] << "\n";
        });
    }).wait();

    cout << "Kernel executed successfully.\n";
}

int main() {
    // constexpr size_t N = 10000;

    queue q;
    try {
        q = queue{gpu_selector_v};
    } catch (const sycl::exception& e) {
        cout << "GPU not available. Falling back to CPU.\n";
        q = queue{cpu_selector_v};
    }

    cout << "Running on device: "
         << q.get_device().get_info<info::device::name>() << "\n";

    test_kernel(q);

    cout << "Successfully completed on device.\n";
    return 0;
}