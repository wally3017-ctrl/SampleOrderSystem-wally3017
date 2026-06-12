#pragma once

struct OrderSummary {
    int reserved  = 0;
    int confirmed = 0;
    int producing = 0;
    int released  = 0;
};
