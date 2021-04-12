#include "repl.cpp"

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

typedef struct
{
    PyObject_HEAD
    float win_probability;
    float tie_probability;
    float lose_probability;
    float mean_score;
    float median_score;
    float mean_damage_taken;
    float mean_damage_dealt;
    float expected_hero_health;
    float expected_enemy_hero_health;
    float death_probability;
    float enemy_death_probability;
} BattleResultObject;

static void BattleResult_dealloc(BattleResultObject* self)
{
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject* BattleResult_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    BattleResultObject *self;
    self = (BattleResultObject *) type->tp_alloc(type, 0);
    if (self != NULL) {
        self->win_probability = 0;
        self->tie_probability = 0;
        self->lose_probability = 0;
        self->mean_score = 0;
        self->median_score = 0;
        self->mean_damage_taken = 0;
        self->mean_damage_dealt = 0;
        self->expected_hero_health = 0;
        self->expected_enemy_hero_health = 0;
        self->death_probability = 0;
        self->enemy_death_probability = 0;
    }
    return (PyObject *) self;
}

static int BattleResult_init(BattleResultObject* self, PyObject* args, PyObject* kwds)
{
    static char *kwlist[] = {
        (char*)"win_probability",
        (char*)"tie_probability",
        (char*)"lose_probability",
        (char*)"mean_score",
        (char*)"median_score",
        (char*)"mean_damage_taken",
        (char*)"mean_damage_dealt",
        (char*)"expected_hero_health",
        (char*)"expected_enemy_hero_health",
        (char*)"death_probability",
        (char*)"enemy_death_probability",
        NULL
    };
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|fffffffffff", kwlist,
                                     &self->win_probability,
                                     &self->tie_probability,
                                     &self->lose_probability,
                                     &self->mean_score,
                                     &self->median_score,
                                     &self->mean_damage_taken,
                                     &self->mean_damage_dealt,
                                     &self->expected_hero_health,
                                     &self->expected_enemy_hero_health,
                                     &self->death_probability,
                                     &self->enemy_death_probability))
        return -1;

    return 0;
}

static PyMemberDef BattleResult_members[] = {
    {"win_probability", T_FLOAT, offsetof(BattleResultObject, win_probability), 0,
     "The probability of winning this battle."},
    {"tie_probability", T_FLOAT, offsetof(BattleResultObject, tie_probability), 0,
     "The probability of a tie."},
    {"lose_probability", T_FLOAT, offsetof(BattleResultObject, lose_probability), 0,
     "The probability of losing this battle."},
    {"mean_score", T_FLOAT, offsetof(BattleResultObject, mean_score), 0,
     "The mean score across all simulations of the battle."},
    {"median_score", T_FLOAT, offsetof(BattleResultObject, median_score), 0,
     "The median score across all simulations of the battle."},
    {"mean_damage_taken", T_FLOAT, offsetof(BattleResultObject, mean_damage_taken), 0,
     "The mean damage taken (by the friendly hero) across all simulations of the battle."},
    {"mean_damage_dealt", T_FLOAT, offsetof(BattleResultObject, mean_damage_dealt), 0,
     "The mean damage dealt (to the enemy hero) across all simulations of the battle."},
    {"expected_hero_health", T_FLOAT, offsetof(BattleResultObject, expected_hero_health), 0,
     "The expected health of the hero after this battle."},
    {"expected_enemy_hero_health", T_FLOAT, offsetof(BattleResultObject, expected_enemy_hero_health), 0,
     "The expected health of the enemy hero after this battle."},
    {"death_probability", T_FLOAT, offsetof(BattleResultObject, death_probability), 0,
     "The probability of the hero dying after this battle."},
    {"enemy_death_probability", T_FLOAT, offsetof(BattleResultObject, enemy_death_probability), 0,
     "The probability of the enemy hero dying after this battle."},
    {NULL}  /* Sentinel */
};

static PyTypeObject BattleResultType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "hsbg_sim.BattleResult",                     /* tp_name */
    sizeof(BattleResultObject),                  /* tp_basicsize */
    0,                                           /* tp_itemsize */
    (destructor) BattleResult_dealloc,           /* tp_dealloc */
    0,                                           /* tp_print */
    0,                                           /* tp_getattr */
    0,                                           /* tp_setattr */
    0,                                           /* tp_reserved */
    0,                                           /* tp_repr */
    0,                                           /* tp_as_number */
    0,                                           /* tp_as_sequence */
    0,                                           /* tp_as_mapping */
    0,                                           /* tp_hash  */
    0,                                           /* tp_call */
    0,                                           /* tp_str */
    0,                                           /* tp_getattro */
    0,                                           /* tp_setattro */
    0,                                           /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,    /* tp_flags */
    "The result of the combat phase simulator.", /* tp_doc */
    0,                                           /* tp_traverse */
    0,                                           /* tp_clear */
    0,                                           /* tp_richcompare */
    0,                                           /* tp_weaklistoffset */
    0,                                           /* tp_iter */
    0,                                           /* tp_iternext */
    0,                                           /* tp_methods */
    BattleResult_members,                        /* tp_members */
    0,                                           /* tp_getset */
    0,                                           /* tp_base */
    0,                                           /* tp_dict */
    0,                                           /* tp_descr_get */
    0,                                           /* tp_descr_set */
    0,                                           /* tp_dictoffset */
    (initproc) BattleResult_init,                /* tp_init */
    0,                                           /* tp_alloc */
    BattleResult_new,                             /* tp_new */
};

typedef struct
{
    PyObject_HEAD
    PyObject* name;
    int attack;
    int health;
    bool is_golden;
    bool taunt;
    bool divine_shield;
    bool poisonous;
    bool windfury;
    bool reborn;
} MinionObject;

static int Minion_traverse(MinionObject* self, visitproc visit, void* arg)
{
    Py_VISIT(self->name);
    return 0;
}

static int Minion_clear(MinionObject* self)
{
    Py_CLEAR(self->name);
    return 0;
}

static void Minion_dealloc(MinionObject* self)
{
    PyObject_GC_UnTrack(self);
    Minion_clear(self);
    Py_TYPE(self)->tp_free((PyObject*) self);
}

static PyObject* Minion_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    MinionObject *self;
    self = (MinionObject *) type->tp_alloc(type, 0);
    if (self != NULL)
    {
        self->name = PyUnicode_FromString("");
        if (self->name == NULL)
        {
            Py_DECREF(self);
            return NULL;
        }
        self->attack = 0;
        self->health = 0;
        self->is_golden = false;
        self->taunt = false;
        self->divine_shield = false;
        self->poisonous = false;
        self->windfury = false;
        self->reborn = false;
    }
    return (PyObject *) self;
}

static int Minion_init(MinionObject* self, PyObject* args, PyObject* kwds)
{
    static char *kwlist[] = {
        (char*)"name",
        (char*)"attack",
        (char*)"health",
        (char*)"is_golden",
        (char*)"taunt",
        (char*)"divine_shield",
        (char*)"poisonous",
        (char*)"windfury",
        (char*)"reborn",
        NULL
    };
    PyObject *name = NULL, *tmp;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|Uiipppppp", kwlist,
                                     &name,
                                     &self->attack,
                                     &self->health,
                                     &self->is_golden,
                                     &self->taunt,
                                     &self->divine_shield,
                                     &self->poisonous,
                                     &self->windfury,
                                     &self->reborn))
        return -1;

    if (name)
    {
        tmp = self->name;
        Py_INCREF(name);
        self->name = name;
        Py_DECREF(tmp);
    }

    return 0;
}

static PyMemberDef Minion_members[] = {
    {"attack", T_INT, offsetof(MinionObject, attack), 0,
     "The attack of the minion"},
    {"health", T_INT, offsetof(MinionObject, health), 0,
     "The health of the minion"},
    {"is_golden", T_BOOL, offsetof(MinionObject, is_golden), 0,
     "Whether this minion is a golden copy"},
    {"taunt", T_BOOL, offsetof(MinionObject, taunt), 0,
     "Whether this minion has taunt"},
    {"divine_shield", T_BOOL, offsetof(MinionObject, divine_shield), 0,
     "Whether this minion has divine shield"},
    {"poisonous", T_BOOL, offsetof(MinionObject, poisonous), 0,
     "Whether this minion is poisonous"},
    {"windfury", T_BOOL, offsetof(MinionObject, windfury), 0,
     "Whether this minion has windfury"},
    {"reborn", T_BOOL, offsetof(MinionObject, reborn), 0,
     "Whether this minion has reborn"},
    {NULL}  /* Sentinel */
};

static PyObject* Minion_getname(MinionObject* self, void* closure)
{
    Py_INCREF(self->name);
    return self->name;
}

static int Minion_setname(MinionObject* self, PyObject* value, void* closure)
{
    if (value == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the name attribute");
        return -1;
    }

    if (!PyUnicode_Check(value))
    {
        PyErr_SetString(PyExc_TypeError, "The name attribute value must be a string");
        return -1;
    }

    Py_INCREF(value);
    Py_CLEAR(self->name);
    self->name = value;
    return 0;
}

static PyGetSetDef Minion_getsetters[] = {
    {"name", (getter) Minion_getname, (setter) Minion_setname,
     "The name of the minion.", NULL},
    {NULL}  /* Sentinel */
};

static PyTypeObject MinionType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "hsbg_sim.Minion",                         /* tp_name */
    sizeof(MinionObject),                      /* tp_basicsize */
    0,                                         /* tp_itemsize */
    (destructor) Minion_dealloc,               /* tp_dealloc */
    0,                                         /* tp_print */
    0,                                         /* tp_getattr */
    0,                                         /* tp_setattr */
    0,                                         /* tp_reserved */
    0,                                         /* tp_repr */
    0,                                         /* tp_as_number */
    0,                                         /* tp_as_sequence */
    0,                                         /* tp_as_mapping */
    0,                                         /* tp_hash  */
    0,                                         /* tp_call */
    0,                                         /* tp_str */
    0,                                         /* tp_getattro */
    0,                                         /* tp_setattro */
    0,                                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE
        | Py_TPFLAGS_HAVE_GC,                  /* tp_flags */
    "A minion.",                                /* tp_doc */
    (traverseproc) Minion_traverse,             /* tp_traverse */
    (inquiry) Minion_clear,                     /* tp_clear */
    0,                                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                          /* tp_iter */
    0,                                          /* tp_iternext */
    0,                                          /* tp_methods */
    Minion_members,                             /* tp_members */
    Minion_getsetters,                          /* tp_getset */
    0,                                          /* tp_base */
    0,                                          /* tp_dict */
    0,                                          /* tp_descr_get */
    0,                                          /* tp_descr_set */
    0,                                          /* tp_dictoffset */
    (initproc) Minion_init,                     /* tp_init */
    0,                                          /* tp_alloc */
    Minion_new,                                 /* tp_new */
};

// typedef struct
// {
//     PyObject_HEAD
//     int tavern_tier;
//     int hero_health;
//     PyObject* minions;
// } BoardObject;

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

    BattleResultObject* result;
    result = (BattleResultObject*)BattleResultType.tp_new((PyTypeObject*)&BattleResultType, NULL, NULL);
    if (result != NULL) {
        result->win_probability = static_cast<float>(stats.win_rate(0));
        result->tie_probability = static_cast<float>(stats.draw_rate());
        result->lose_probability = static_cast<float>(stats.win_rate(1));
        result->mean_score = static_cast<float>(stats.mean_score());
        result->median_score = static_cast<float>(results[results.size()/2]);
        result->mean_damage_taken = static_cast<float>(stats.mean_damage_taken(0));
        result->mean_damage_dealt = static_cast<float>(stats.mean_damage_taken(1));
        result->expected_hero_health = static_cast<float>(repl.players[0].health - result->mean_damage_taken);
        result->expected_enemy_hero_health = static_cast<float>(repl.players[1].health - result->mean_damage_dealt);
        result->death_probability = static_cast<float>(stats.death_rate(0));
        result->enemy_death_probability = static_cast<float>(stats.death_rate(1));
    }
    return (PyObject*) result;
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

PyMODINIT_FUNC
PyInit_hsbg_sim(void) {
    PyObject *m;
    if (PyType_Ready(&BattleResultType) < 0)
        return NULL;

    if (PyType_Ready(&MinionType) < 0)
        return NULL;

    m = PyModule_Create(&hsbgsimmodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&BattleResultType);
    if (PyModule_AddObject(m, "BattleResult", (PyObject *) &BattleResultType) < 0) {
        Py_DECREF(&BattleResultType);
        Py_DECREF(m);
        return NULL;
    }

    Py_INCREF(&MinionType);
    if (PyModule_AddObject(m, "Minion", (PyObject *) &MinionType) < 0) {
        Py_DECREF(&MinionType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
