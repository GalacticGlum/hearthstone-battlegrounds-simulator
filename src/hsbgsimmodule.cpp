#include "repl.cpp"

#include <Python.h>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

static PyObject* hsbg_run_simulator(PyObject* self, PyObject* args)
{
    PyObject* commands_list_obj;
    int n_games;
    if (!PyArg_ParseTuple(args, "O!i", &PyList_Type, &commands_list_obj, &n_games))
    {
        PyErr_SetString(PyExc_TypeError, "commands must be a list.");
        return NULL;
    }

    PyObject* item_obj;
    const char* command;
    std::string input_commands;

    Py_ssize_t n = PyList_Size(commands_list_obj);
    for (int i = 0; i < n; ++i)
    {
        item_obj = PyList_GetItem(commands_list_obj, i);
        command = PyUnicode_AsUTF8(item_obj);
        input_commands += std::string(command) + "\n";
    }

    std::istringstream in(input_commands);
    REPL repl(std::cout);
    while (in.good()) {
        std::string line;
        std::getline(in,line);
        repl.parse_line(line);
    }

    std::vector<int> results;
    ScoreSummary stats = simulate(repl.players[0], repl.players[1], n_games, &results);

    PyObject* ret_value = PyTuple_New(2);
    PyTuple_SetItem(ret_value, 0, PyFloat_FromDouble(repl.players[0].health - stats.mean_damage_taken(0)));
    PyTuple_SetItem(ret_value, 1, PyFloat_FromDouble(repl.players[1].health - stats.mean_damage_taken(1)));
    return ret_value;
}

static PyMethodDef HsbgSimMethods[] = {
    {"run_simulator", hsbg_run_simulator, METH_VARARGS,
     "Python interface for the Hearthstone Battlegrounds combat simulator."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef hsbgsimmodule = {
    PyModuleDef_HEAD_INIT,
    "hsbg_sim",
    "Python interface for the Hearthstone Battlegrounds combat simulator.",
    -1,
    HsbgSimMethods
};

PyMODINIT_FUNC PyInit_hsbg_sim(void) {
    return PyModule_Create(&hsbgsimmodule);
}
