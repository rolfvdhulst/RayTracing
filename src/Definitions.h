//
// Created by rolf on 14-8-22.
//

#ifndef RAYTRACING_SRC_DEFINITIONS_H_
#define RAYTRACING_SRC_DEFINITIONS_H_

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;
using Real = double;

#endif //RAYTRACING_SRC_DEFINITIONS_H_
