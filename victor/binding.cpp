#include <time.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <victor/victor.hpp>


using namespace pybind11::literals; 
namespace py = pybind11;

PYBIND11_MODULE(victor, m) {
    m.doc() = "Python bindings for Victor - Lightweight Vector Index Library";

    py::class_<VictorIndex>(m, "VictorIndex")
        .def(py::init<int, int, uint16_t>(), 
             py::arg("type"), py::arg("method"), py::arg("dims"),
             "Create a new VictorIndex.\n\n"
             "Args:\n"
             "    type (int): Index type (e.g., FLAT_INDEX).\n"
             "    method (int): Distance metric (e.g., L2NORM, COSINE).\n"
             "    dims (int): Dimensions of the vectors.")
        
        .def_static("load", &VictorIndex::load,
             py::arg("filename"),
             "Load an index from file.\n\n"
             "Args:\n"
             "    filename (str): Path to saved index file.\n\n"
             "Returns:\n"
             "    VictorIndex instance.")
        
        .def("insert", &VictorIndex::insert,
             py::arg("id"), py::arg("vector"),
             "Insert a vector into the index.\n\n"
             "Args:\n"
             "    id (int): Unique identifier.\n"
             "    vector (List[float]): Vector data.")
        
        .def("search", [](VictorIndex& self, py::array_t<float> query) {
            auto buf = query.request();
            if (buf.ndim != 1)
                throw std::runtime_error("Query vector must be 1D");
            std::vector<float> vec(static_cast<float*>(buf.ptr), static_cast<float*>(buf.ptr) + buf.shape[0]);
            auto res = self.search(vec);
            return py::make_tuple(res.first, res.second);
        }, py::arg("query"),
        "Search for the nearest neighbor.\n\n"
        "Args:\n"
        "    query (np.ndarray): Query vector.\n\n"
        "Returns:\n"
        "    Tuple (id, distance).")

        .def("search_n", [](VictorIndex& self, py::array_t<float> query, int n) {
            auto buf = query.request();
            if (buf.ndim != 1)
                throw std::runtime_error("Query vector must be 1D");
            std::vector<float> vec(static_cast<float*>(buf.ptr), static_cast<float*>(buf.ptr) + buf.shape[0]);
            auto results = self.search_n(vec, n);
            py::list output;
            for (auto& res : results)
                output.append(py::make_tuple(res.first, res.second));
            return output;
        }, py::arg("query"), py::arg("n"),
        "Search for top N nearest neighbors.\n\n"
        "Args:\n"
        "    query (np.ndarray): Query vector.\n"
        "    n (int): Number of neighbors.\n\n"
        "Returns:\n"
        "    List of (id, distance) tuples.")

        .def("remove", &VictorIndex::remove,
             py::arg("id"),
             "Delete a vector by its ID.\n\n"
             "Args:\n"
             "    id (int): Vector ID to delete.")

        .def("contains", &VictorIndex::contains,
             py::arg("id"),
             "Check if a vector ID exists.\n\n"
             "Args:\n"
             "    id (int): ID to check.\n\n"
             "Returns:\n"
             "    bool: True if found, False otherwise.")

        .def("size", &VictorIndex::size,
             "Get the number of vectors stored.\n\n"
             "Returns:\n"
             "    int: Number of vectors.")

        .def("dump", &VictorIndex::dump,
             py::arg("filename"),
             "Dump the index to a file.\n\n"
             "Args:\n"
             "    filename (str): Path to save the index.")

        .def("stats", [](VictorIndex& self) {
            IndexStats s = self.stats();
            return py::dict(
                "insert"_a = py::dict("count"_a=s.insert.count, "total"_a=s.insert.total),
                "remove"_a = py::dict("count"_a=s.remove.count, "total"_a=s.remove.total),
                "search"_a = py::dict("count"_a=s.search.count, "total"_a=s.search.total),
                "search_n"_a = py::dict("count"_a=s.search_n.count, "total"_a=s.search_n.total),
                "dump"_a = py::dict("count"_a=s.dump.count, "total"_a=s.dump.total)
            );
        }, "Get operation timing statistics.");
}
