#include "Route.h"

Route::Route() {
    nodes.push_back(0);
    nodes.push_back(0);
}

std::vector<int>& Route::getNodes() {
    return nodes;
}

const std::vector<int>& Route::getNodes() const {
    return nodes;
}

void Route::insertCustomerBeforeDepot(int customerId) {
    nodes.insert(nodes.end() - 1, customerId);
}

void Route::insertCustomerAt(int position, int customerId) {
    nodes.insert(nodes.begin() + position, customerId);
}

void Route::removeCustomerAt(int position) {
    nodes.erase(nodes.begin() + position);
}

int Route::customerCount() const {
    return static_cast<int>(nodes.size()) - 2;
}

bool Route::isEmpty() const {
    return customerCount() == 0;
}
