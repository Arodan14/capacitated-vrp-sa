#ifndef ROUTE_H
#define ROUTE_H

#include <vector>

class Route {
private:
    std::vector<int> nodes;

public:
    Route();

    std::vector<int>& getNodes();
    const std::vector<int>& getNodes() const;

    void insertCustomerBeforeDepot(int customerId);
    void insertCustomerAt(int position, int customerId);
    void removeCustomerAt(int position);

    int customerCount() const;
    bool isEmpty() const;
};

#endif
