//
// Created by Maximilian Denninger on 13.08.18.
//

#include "BoundingBox.h"

std::ostream& operator<<(std::ostream& os, const BoundingBox& box){
os << "min: " << box.min() << ", max: " << box.max() << std::endl;
return os;
}
