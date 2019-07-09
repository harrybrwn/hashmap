#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

#define MapValue PyObject*
#include "../hashmap.h"

typedef struct {
	PyObject_HEAD

	Map* _map;
} HashMap;

static void HashMap_dealloc(HashMap* self)
{
	Map_close(self->_map);
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject*
HashMap_new(PyTypeObject* type, PyObject *args, PyObject* kwgs)
{
	HashMap* self;
	self = (HashMap*) type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->_map = New_Map();
	}

	return (PyObject*) self;
}

static int
HashMap_init(HashMap *self, PyObject *args, PyObject *kwgs)
{
	self->_map = New_Map();
	if (self->_map == NULL)
		return -1;
	return 0;
}

static PyMemberDef HashMap_members[] = {
	{NULL}
};

static PyObject*
HashMap_put(HashMap* self, PyObject *args, PyObject* kw)
{
	char* key;
	PyObject* val;
	static char* kwlist[] = {"key", "val", NULL};

	if (!PyArg_ParseTupleAndKeywords(args, kw, "sO", kwlist, &key, &val))
        return NULL;
	Map_put(self->_map, key, val);
	return Py_None;
}

static PyObject*
HashMap_get(HashMap* self, PyObject *args, PyObject* kw)
{
	char* key;
	static char* kwlist[] = {"key", NULL};

	if (!PyArg_ParseTupleAndKeywords(args, kw, "s", kwlist, &key))
        return NULL;
	return Map_get(self->_map, key);
}

static PyObject*
HashMap_delete(HashMap* self, PyObject *args, PyObject* kw)
{
	char* key;
	static char* kwlist[] = {"key", NULL};

	if (!PyArg_ParseTupleAndKeywords(args, kw, "s", kwlist, &key))
        return NULL;
	Map_delete(self->_map, key);
	return Py_None;
}

static PyObject*
HashMap_keys(HashMap* self, PyObject *args)
{
	/* yup, dynamic stack allocation in c89 */
	char **keys = alloca(sizeof(char*) * self->_map->item_count);
	Map_keys(self->_map, keys);
	
	PyObject* str_list = PyList_New(self->_map->item_count);
	size_t i;
	for (i = 0; i < self->_map->item_count; i++)
		PyList_SET_ITEM(str_list, (Py_ssize_t)i, Py_BuildValue("s", keys[i]));
	
	free(keys);
	return str_list;
}

static PyObject*
HashMap_resize(HashMap* self, PyObject *args)
{
	int size;
	if (!PyArg_ParseTuple(args, "i", &size))
		return NULL;
	Map_resize(&self->_map, (size_t)size);
	return Py_None;
}

static PyMethodDef HashMap_methods[] = {
	{"put",    (PyCFunction) HashMap_put,    METH_VARARGS | METH_KEYWORDS, "put data into the HashMap"},
	{"get",    (PyCFunction) HashMap_get,    METH_VARARGS | METH_KEYWORDS, "get data from the HashMap"},
	{"delete", (PyCFunction) HashMap_delete, METH_VARARGS | METH_KEYWORDS, "delete data from the HashMap"},
	{"keys",   (PyCFunction) HashMap_keys,   METH_NOARGS,                  "get all keys stored in the Hashmap"},
	{"resize", (PyCFunction) HashMap_resize, METH_VARARGS,                 "resize the map internals"},
	{NULL},
};

static PyTypeObject HashMapType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "hashmap.HashMap",
	.tp_doc = "Map is a hashmap.",
	.tp_basicsize = sizeof(HashMap),
	.tp_itemsize  = 0,
	.tp_flags   = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
	.tp_new     = HashMap_new,
	.tp_init    = (initproc) HashMap_init,
	.tp_dealloc = (destructor) HashMap_dealloc,
	.tp_members = HashMap_members,
	.tp_methods = HashMap_methods,
};

PyMethodDef hashmap_methods[] = {
	{NULL, NULL, 0, NULL}
};

static PyModuleDef hashmap = {
	PyModuleDef_HEAD_INIT,
	.m_name = "hashmap",
	.m_doc = "a library for a hashmap",
	.m_size = -1, // the module state is global
	.m_methods = hashmap_methods,
};

PyMODINIT_FUNC PyInit_hashmap(void)
{
	if (PyType_Ready(&HashMapType) < 0)
		return NULL;

	PyObject* mod = PyModule_Create(&hashmap);
	if (mod == NULL)
		return NULL;

	Py_INCREF(&HashMapType);
	PyModule_AddObject(mod, "HashMap", (PyObject*) &HashMapType);
	return mod;
}