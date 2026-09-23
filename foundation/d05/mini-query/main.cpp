/*
 * 本文件只证明：内存表上固定数据按 scan→predicate→hash join→aggregate→having→sort/limit 走一遍。
 * 不证明：通用 SQL、优化器、磁盘存储、空结果集；数据全写死，不用 random。
 */
#include "../../common/check.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Customer {
    int id;
    const char *name;
};

struct Order {
    int id;
    int customer_id;
    int amount;
};

struct Joined {
    const char *name;
    int amount;
};

struct Agg {
    std::string name;
    int total;
};

int main()
{
    const Customer customers[] = {
        {1, "Alice"}, {2, "Bob"}, {3, "Carol"}, {4, "Dave"}, {5, "Eve"},
    };
    const Order orders[] = {
        {10, 1, 40}, {11, 1, 60}, {12, 2, 120}, {13, 2, 80},
        {14, 3, 200}, {15, 3, 10}, {16, 4, 15}, {17, 5, 50},
    };
    const int order_n = (int)(sizeof(orders) / sizeof(orders[0]));
    const int customer_n = (int)(sizeof(customers) / sizeof(customers[0]));

    /* scan */
    std::vector<Order> scanned(orders, orders + order_n);
    std::cout << "stage scan: " << scanned.size() << "\n";
    FD_CHECK(scanned.size() == 8, "scan cardinality 8");

    /* predicate: amount >= 20 → drop amounts 10 and 15 (order ids 15 and 16) */
    std::vector<Order> filtered;
    for (const Order &o : scanned) {
        if (o.amount >= 20) {
            filtered.push_back(o);
        }
    }
    std::cout << "stage predicate: " << filtered.size() << "\n";
    FD_CHECK(filtered.size() == 6, "predicate cardinality 6");

    /* hash join orders.customer_id = customers.id */
    std::unordered_map<int, const char *> by_id;
    for (int i = 0; i < customer_n; i++) {
        by_id.emplace(customers[i].id, customers[i].name);
    }
    std::vector<Joined> joined;
    for (const Order &o : filtered) {
        auto it = by_id.find(o.customer_id);
        if (it != by_id.end()) {
            joined.push_back(Joined{it->second, o.amount});
        }
    }
    std::cout << "stage hash_join: " << joined.size() << "\n";
    FD_CHECK(joined.size() == 6, "join cardinality 6");

    /* aggregate: SUM(amount) GROUP BY name */
    std::unordered_map<std::string, int> totals;
    for (const Joined &j : joined) {
        totals[j.name] += j.amount;
    }
    std::vector<Agg> grouped;
    grouped.reserve(totals.size());
    for (const auto &kv : totals) {
        grouped.push_back(Agg{kv.first, kv.second});
    }
    std::cout << "stage aggregate: " << grouped.size() << "\n";
    FD_CHECK(grouped.size() == 4, "aggregate cardinality 4");

    /* having: total >= 100 → drop Eve(50) */
    std::vector<Agg> having;
    for (const Agg &g : grouped) {
        if (g.total >= 100) {
            having.push_back(g);
        }
    }
    std::cout << "stage having: " << having.size() << "\n";
    FD_CHECK(having.size() == 3, "having cardinality 3");

    /* sort total DESC, name ASC; limit 2 */
    std::sort(having.begin(), having.end(), [](const Agg &a, const Agg &b) {
        if (a.total != b.total) {
            return a.total > b.total;
        }
        return a.name < b.name;
    });
    if (having.size() > 2) {
        having.resize(2);
    }
    std::cout << "stage sort_limit: " << having.size() << "\n";
    FD_CHECK(having.size() == 2, "sort/limit cardinality 2");

    std::cout << "result:\n";
    for (const Agg &row : having) {
        std::cout << row.name << " " << row.total << "\n";
    }
    FD_CHECK(having[0].name == "Bob" && having[0].total == 200, "first row Bob 200");
    FD_CHECK(having[1].name == "Carol" && having[1].total == 200, "second row Carol 200");
    return 0;
}
