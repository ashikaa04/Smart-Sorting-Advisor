#pragma once
#include <string>

std::string predictAlgorithm(double size, double sortedness, double range) {
    if (range <= 3348.500000) {
        if (range <= 2865.500000) {
            if (range <= 1860.000000) {
                return "0";
            } else {
                if (range <= 1874.000000) {
                    if (sortedness <= 0.481728) {
                        return "0";
                    } else {
                        return "1";
                    }
                } else {
                    return "0";
                }
            }
        } else {
            if (range <= 3035.000000) {
                if (range <= 2951.500000) {
                    return "1";
                } else {
                    if (sortedness <= 0.503268) {
                        return "0";
                    } else {
                        return "1";
                    }
                }
            } else {
                return "0";
            }
        }
    } else {
        if (size <= 751.000000) {
            if (sortedness <= 0.520338) {
                if (size <= 744.000000) {
                    if (sortedness <= 0.519338) {
                        return "1";
                    } else {
                        return "2";
                    }
                } else {
                    return "2";
                }
            } else {
                return "0";
            }
        } else {
            if (sortedness <= 0.487019) {
                if (size <= 915.000000) {
                    if (sortedness <= 0.486701) {
                        return "1";
                    } else {
                        return "2";
                    }
                } else {
                    return "2";
                }
            } else {
                return "1";
            }
        }
    }
}
