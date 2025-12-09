#include "include/BindLevelInfo.h"

void bindLevelInfo(py::module &m) {
  py::class_<LevelInfo>(m, "LevelInfo")
      .def(py::init<>())
      .def(py::init<Price, Quantity>(), py::arg("price"), py::arg("quantity"))
      .def_readonly("price", &LevelInfo::price, "Level Price")
      .def_readonly("quantity", &LevelInfo::quantity, "Level quantity")
      .def("__repr__", [](const LevelInfo &info) {
        return "LevelInfo (price=" + std::to_string(info.price) +
               ", quantity=" + std::to_string(info.quantity) + ")";
      });
}
