#include <string>
#include <vector>
#include <iostream>

struct Folder {
    std::string name;
    std::vector<Folder> children;

    Folder(std::string name): name(name) {};

    void add_child(Folder child) {
        children.push_back(child);
    }

    void print_folder(std::ostream & out, int floor) const {
        out << std::string(floor * 2, ' ') << name << "\n";
        for (const Folder & child : children) {
            int current_floor = floor + 1;
            child.print_folder(out, current_floor);
        };
    }
};

// class TreeIterator {
//     TreeIterator operator++();
//     Cos operator*();
// };

// class TreeWalker {
//     TreeIterator begin();
//     TreeIterator end();
// };

std::ostream & operator<<(std::ostream & out, const Folder & folder) {
    folder.print_folder(out, 0);
    return out;
}

int main() {
    Folder folder("duży_kot");
    Folder podfolder1("mały_kotek1");
    Folder podfolder2("maly_kotek2");
    Folder podfolder3("jeszcze_mniejszy_kotek");
    podfolder1.add_child(podfolder3);
    folder.add_child(podfolder1);
    folder.add_child(podfolder2);
    std::cout << folder << std::endl;
    
    // for (const Folder & folder : folder.tree()) {
    //     std::cout << folder << std::endl;
    // }
}
