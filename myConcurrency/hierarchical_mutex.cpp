//
// Created by xiaoyuyu on 2022/5/27.
//

#include "hierarchical_mutex.h"

thread_local size_t hierarchical_mutex::upper_bound_hierarchy = SIZE_T_MAX;


