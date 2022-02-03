#include <vector>
#include <memory>

#include "../sort.hh"

int main() {
    auto v = std::make_unique<std::vector<int>>();
    print_vec(*v);
    *v = {1, 2, 3};
    print_vec(*v);
    std::cout << (*v).size() << std::endl;
    std::cout << v->size() << std::endl;
}

