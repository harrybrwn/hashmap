#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

#pragma pack(1)

#define MapValue PyObject*
#include "hashmap.h"


typedef struct {
	PyObject_HEAD

	size_t size;
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
		self->_map = NULL;
		self->size = 0;
	}

	return (PyObject*) self;
}


static int
HashMap_init(HashMap *self, PyObject *args, PyObject *kwgs)
{
	self->_map = New_Map();
	if (self->_map == NULL)
		return -1;
	self->size = self->_map->__size;
	return 0;
}


static PyObject*
HashMap_put(HashMap* self, PyObject *args, PyObject* kw)
{
	char* key;
	PyObject* val;
	static char* kwlist[] = {"key", "val", NULL};

	if (!PyArg_ParseTupleAndKeywords(args, kw, "sO", kwlist, &key, &val))
		return NULL;

	Py_INCREF(val);
	Map_put(self->_map, key, val);
	Py_INCREF(Py_None);
	return Py_None;
}


#define KEY_ERR_IF(expr) {if(expr){PyErr_SetString(PyExc_KeyError, "Unknown key"); return NULL;}}


static PyObject*
HashMap_get(HashMap* self, PyObject *args, PyObject* kw)
{
	char* key;
	if (!PyArg_ParseTuple(args, "s", &key))
        return NULL;

	PyObject* val = Map_get(self->_map, key);
	KEY_ERR_IF(val == NULL);

	return val;
}


static PyObject*
HashMap_delete(HashMap* self, PyObject *args, PyObject* kw)
{
	char* key;
	static char* kwlist[] = {"key", NULL};

	if (!PyArg_ParseTupleAndKeywords(args, kw, "s", kwlist, &key))
        return NULL;
	
	PyObject* val = Map_get(self->_map, key);
	KEY_ERR_IF(val == NULL);

	Map_delete(self->_map, key);
	Py_INCREF(Py_None);
	return Py_None;
}


static PyObject*
HashMap_keys(HashMap* self, PyObject *Py_UNUSED(ignored))
{
	size_t i;
	char **keys = malloc(sizeof(char*) * self->_map->item_count);
	
	PyObject* str_list = PyList_New(self->_map->item_count);
	Map_keys(self->_map, keys);

	for (i = 0; i < self->_map->item_count; i++)
		PyList_SET_ITEM(str_list, (Py_ssize_t)i, Py_BuildValue("s", keys[i]));

	free(keys);
	return str_list;
}


static PyObject*
HashMap_resize(HashMap* self, PyObject *args)
{
	size_t size;
	
	if (!PyArg_ParseTuple(args, "i", &size))
		return NULL;

	Map_resize(&self->_map, size);
	self->size = size;
	
	Py_INCREF(Py_None);
	return Py_None;
}


static PyMethodDef HashMap_methods[] = {
	// {"__getitem__", (PyCFunction) HashMap_getitem__, METH_O | METH_COEXIST,        __getitem__doc},
	// {"__setitem__", (PyCFunction) HashMap_setitem__, METH_O | METH_COEXIST,        __setitem__doc},
	{"put",         (PyCFunction) HashMap_put,       METH_VARARGS | METH_KEYWORDS, "put data into the HashMap"},
	{"get",         (PyCFunction) HashMap_get,       METH_VARARGS | METH_KEYWORDS, "get data from the HashMap"},
	{"delete",      (PyCFunction) HashMap_delete,    METH_VARARGS | METH_KEYWORDS, "delete data from the HashMap"},
	{"keys",        (PyCFunction) HashMap_keys,      METH_NOARGS,                  "get all keys stored in the Hashmap"},
	{"resize",      (PyCFunction) HashMap_resize,    METH_VARARGS,                 "resize the map internals"},
	{NULL},
};


static PyMemberDef HashMap_members[] = {
	{"size", T_INT, offsetof(HashMap, size), 0, "size of the map"},
	{NULL},
};


static PyObject* HashMap_getsize(HashMap* self, void *closure)
{
	return Py_BuildValue("i", self->size);
}


static int HashMap_setsize(HashMap* self, PyObject* value, void *closure)
{
	if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the size");
        return -1;
    }
    if (!PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError,
                        "The size attribute value must be an int");
        return -1;
    }

	self->size = PyLong_AsLong(value);
	Py_INCREF(value);
	Map_resize(&self->_map, self->size);
	return 0;
}


static PyGetSetDef HashMap_getsetters[] = {
	{"size", (getter) HashMap_getsize, (setter) HashMap_setsize, "size of the HashMap", NULL},
	{NULL},
};


static PyObject* HashMap_getitem__(HashMap* self, PyObject *key)
{
	char* key_str = NULL;
	if (!PyUnicode_CheckExact(key)) {
		PyErr_SetString(PyExc_TypeError,
                        "HashMap key must be a string");
		return NULL;
	}

	PyObject *temp_bytes = PyUnicode_AsEncodedString(key, "UTF-8", "strict"); // Owned reference
	if (temp_bytes != NULL)
	{
		key_str = PyBytes_AS_STRING(temp_bytes);
		Py_DECREF(temp_bytes);
	}
	else
	{
		return NULL;
	}

	return Map_get(self->_map, key_str);
}


static int HashMap_setitem__(HashMap* self, PyObject *key, PyObject *val)
{
	char* key_str = NULL;
	if (!PyUnicode_CheckExact(key)) {
		PyErr_SetString(PyExc_TypeError,
                        "HashMap key must be a string");
		return 1;
	}

	PyObject *temp_bytes = PyUnicode_AsEncodedString(key, "UTF-8", "strict"); // Owned reference
	if (temp_bytes != NULL)
	{
		key_str = PyBytes_AS_STRING(temp_bytes);
		Py_DECREF(temp_bytes);
	}
	else
	{
		return 1;
	}

	Map_put(self->_map, key_str, val);
	return 0;
}

static Py_ssize_t HashMap_len__(HashMap *self)
{
	return self->_map->item_count;
}


static PyMappingMethods HashMap_as_mappings = {
	(lenfunc) HashMap_len__,           /*mp_length*/
	(binaryfunc) HashMap_getitem__,    /*mp_subscript*/
	(objobjargproc) HashMap_setitem__, /*mp_ass_subscript*/
};


static PyTypeObject HashMapType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "hashmap.HashMap",
	.tp_doc = "Map is a hashmap.",
	.tp_basicsize  = sizeof(HashMap),
	.tp_itemsize   = 0,
	.tp_flags      = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
	.tp_new        = HashMap_new,
	.tp_init       = (initproc)   HashMap_init,
	.tp_dealloc    = (destructor) HashMap_dealloc,
	.tp_as_mapping = &HashMap_as_mappings,
	.tp_members    = HashMap_members,
	.tp_methods    = HashMap_methods,
	.tp_getset     = HashMap_getsetters,
};


PyMethodDef hashmap_module_methods[] = {
	{NULL, NULL, 0, NULL}
};


static PyModuleDef hashmap = {
	PyModuleDef_HEAD_INIT,
	.m_name = "hashmap",
	.m_doc = "a library for a hashmap",
	.m_size = -1, // the module state is global
	.m_methods = hashmap_module_methods,
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