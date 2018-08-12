#ifndef SIMPLE_PATH_PLANNER_H_
#define SIMPLE_PATH_PLANNER_H_

#include <vector>
#include "../../main.hpp"

class PathPlanner {
public:
    static std::vector<std::pair<double, double>> path_vec;

    void emplace_path(double Forward, double Rotation);
    bool check_path();
    std::vector<std::pair<double, double>>::const_iterator get_first_path();
    void path_erase_first();
};

extern void SimplePathPlanner_init();
extern int SimplePathPlanner_main();

#endif /* SIMPLE_PATH_PLANNER_H_ */