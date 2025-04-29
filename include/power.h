#ifndef POWER_H
#define POWER_H

#include "dashboard.h"

void power_update(const DashboardData_t *data);
float power_get_instant(void);

#endif