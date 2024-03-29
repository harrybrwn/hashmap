#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

#pragma pack(1)

#define MapValue PyObject *
#include "hashmap.h"

typedef struct
{
	PyObject_HEAD

		Map *_map;
	size_t size;
} HashMap;

static char HashMap_doc[] =
	"Map is a hash table implimentation written in c for python.";

static void HashMap_dealloc(HashMap *self)
{
	map_close_free_keys(self->_map);
	Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *
HashMap_new(PyTypeObject *type, PyObject *args, PyObject *kwgs)
{
	HashMap *self;
	self = (HashMap *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->_map = NULL;
		self->size = 0;
	}

	return (PyObject *)self;
}

static int
HashMap_init(HashMap *self, PyObject *args, PyObject *kwgs)
{
	size_t msize = DEFAULT_MAP_SIZE;
	if (!PyArg_ParseTuple(args, "|l", &msize))
	{
		return -1;
	}

	self->_map = create_map(msize);
	if (self->_map == NULL)
		return -1;
	self->size = self->_map->__size;
	return 0;
}

static char HashMap_put_doc[] =
	"put will stort any python object in association with a string key.\n"
	"  Args:\n"
	"    key: string, name of the object being stored\n"
	"    val: any python object to be stored\n";

static PyObject *
HashMap_put(HashMap *self, PyObject *args, PyObject *kw)
{
	char *key;
	PyObject *val;
	static char *kwlist[] = {"key", "val", NULL};

	if (!PyArg_ParseTupleAndKeywords(args, kw, "sO", kwlist, &key, &val))
		return NULL;

	char *mykey = malloc((strlen(key) + 1) * sizeof(char));
	strcpy(mykey, key);

	Py_INCREF(val);
	map_put(self->_map, mykey, val);

	Py_INCREF(Py_None);
	return Py_None;
}

#define KEY_ERR_IF(expr, KEY)                                          \
	{                                                                  \
		if (expr)                                                      \
		{                                                              \
			PyErr_Format(PyExc_KeyError, "Cannot find key '%s'", KEY); \
			return NULL;                                               \
		}                                                              \
	}

static inline PyObject *get_map_val(Map *m, char *key)
{
	PyObject *val = map_get(m, key);
	KEY_ERR_IF(val == NULL, key);

	Py_INCREF(val);
	return val;
}

static PyObject *
HashMap_get(HashMap *self, PyObject *args, PyObject *kw)
{
	char *key;
	if (!PyArg_ParseTuple(args, "s", &key))
		return NULL;
	return get_map_val(self->_map, key);
}

static PyObject *
HashMap_delete(HashMap *self, PyObject *args, PyObject *kw)
{
	char *key;
	static char *kwlist[] = {"key", NULL};

	if (!PyArg_ParseTupleAndKeywords(args, kw, "s", kwlist, &key))
		return NULL;

	PyObject *val = map_get(self->_map, key);
	KEY_ERR_IF(val == NULL, key);
	Py_DECREF(val);

	map_delete(self->_map, key);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *get_keys(Map *m)
{
	size_t i, count = m->item_count;
	PyObject *str_list = PyList_New(count);

	if (count == 0)
		return str_list;

	char *keys[count];
	map_keys(m, keys);

	for (i = 0; i < count; i++)
	{
		PyList_SET_ITEM(str_list, (Py_ssize_t)i, Py_BuildValue("s", keys[i]));
	}
	return str_list;
}

static PyObject *
HashMap_keys(HashMap *self, PyObject *Py_UNUSED(ignored))
{
	return get_keys(self->_map);
}

static char HashMap_clear_doc[] =
	"Flush all the data from the HashMap.";

static PyObject *
HashMap_clear(HashMap *self, PyObject *Py_UNUSED(ignored))
{
	map_clear_free_keys(self->_map);
	self->size = 0;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
HashMap_resize(HashMap *self, PyObject *args)
{
	size_t size;

	if (!PyArg_ParseTuple(args, "i", &size))
		return NULL;

	if (map_resize(&self->_map, size) == -1)
	{
		PyErr_SetString(PyExc_MemoryError, "Could not allocate that much memory");
		return NULL;
	}

	self->size = size;

	Py_INCREF(Py_None);
	return Py_None;
}

static PyMethodDef HashMap_methods[] = {
	{"put", (PyCFunction)HashMap_put, METH_VARARGS | METH_KEYWORDS, HashMap_put_doc},
	{"get", (PyCFunction)HashMap_get, METH_VARARGS | METH_KEYWORDS, "get data from the HashMap"},
	{"delete", (PyCFunction)HashMap_delete, METH_VARARGS | METH_KEYWORDS, "delete data from the HashMap"},
	{"keys", (PyCFunction)HashMap_keys, METH_NOARGS, "get all keys stored in the Hashmap"},
	{"resize", (PyCFunction)HashMap_resize, METH_VARARGS, "resize the map internals"},
	{"clear", (PyCFunction)HashMap_clear, METH_NOARGS, HashMap_clear_doc},
	{NULL},
};

static PyMemberDef HashMap_members[] = {
	{"size", T_INT, offsetof(HashMap, size), 0, "size of the map"},
	{NULL},
};

static PyObject *HashMap_getsize(HashMap *self, void *closure)
{
	return Py_BuildValue("i", self->size);
}

static int HashMap_setsize(HashMap *self, PyObject *value, void *closure)
{
	if (value == NULL)
	{
		PyErr_SetString(PyExc_TypeError, "Cannot delete the size");
		return -1;
	}
	if (!PyLong_Check(value))
	{
		PyErr_SetString(PyExc_TypeError,
						"The size attribute value must be an int");
		return -1;
	}

	self->size = PyLong_AsLong(value);
	Py_INCREF(value);
	map_resize(&self->_map, self->size);
	return 0;
}

static PyGetSetDef HashMap_getsetters[] = {
	{"size", (getter)HashMap_getsize, (setter)HashMap_setsize, "size of the HashMap", NULL},
	{NULL},
};

static PyObject *HashMap__iter__(HashMap *self)
{
	return PyObject_GetIter(get_keys(self->_map));
}

#define STR_CONV(OBJ, STR)                                                         \
	{                                                                              \
		PyObject *_temp_bytes = PyUnicode_AsEncodedString(OBJ, "UTF-8", "strict"); \
		if (_temp_bytes != NULL)                                                   \
		{                                                                          \
			STR = PyBytes_AS_STRING(_temp_bytes);                                  \
			Py_DECREF(_temp_bytes);                                                \
		}                                                                          \
	}

static PyObject *HashMap__getitem__(HashMap *self, PyObject *key)
{
	char *key_str = NULL;
	if (!PyUnicode_CheckExact(key))
	{
		PyErr_SetString(PyExc_TypeError,
						"HashMap key must be a string");
		return NULL;
	}
	STR_CONV(key, key_str);
	return get_map_val(self->_map, key_str);
}

static int HashMap__setitem__(HashMap *self, PyObject *key, PyObject *val)
{
	char *key_str = NULL;
	if (!PyUnicode_CheckExact(key))
	{
		PyErr_SetString(PyExc_TypeError,
						"HashMap key must be a string");
		return 1;
	}
	STR_CONV(key, key_str);

	char *mykey = malloc(strlen(key_str));
	strcpy(mykey, key_str);

	map_put(self->_map, mykey, val);
	Py_INCREF(val);
	return 0;
}

static Py_ssize_t HashMap__len__(HashMap *self)
{
	return self->_map->item_count;
}

int HashMap__contains__(HashMap *self, PyObject *value)
{
	char *key = NULL;
	if (!PyUnicode_CheckExact(value))
	{
		PyErr_SetString(PyExc_TypeError,
						"HashMap key must be a string");
		return -1;
	}
	STR_CONV(value, key);

	PyObject *res = map_get(self->_map, key);
	if (res == NULL)
		return 0;
	return 1;
}

static PyMappingMethods HashMap_as_mapping = {
	(lenfunc)HashMap__len__,		   /*mp_length*/
	(binaryfunc)HashMap__getitem__,	/*mp_subscript*/
	(objobjargproc)HashMap__setitem__, /*mp_ass_subscript*/
};

static PySequenceMethods HashMap_as_sequence = {
	0,								 /* sq_length */
	0,								 /* sq_concat */
	0,								 /* sq_repeat */
	0,								 /* sq_item */
	0,								 /* sq_slice */
	0,								 /* sq_ass_item */
	0,								 /* sq_ass_slice */
	(objobjproc)HashMap__contains__, /* sq_contains */
	0,								 /* sq_inplace_concat */
	0,								 /* sq_inplace_repeat */
};

static PyTypeObject HashMap_Type = {
	PyVarObject_HEAD_INIT(NULL, 0) "hashmap.HashMap", /* tp_name */
	sizeof(PyDictObject),							  /* tp_basicsize */
	0,												  /* tp_itemsize */
	(destructor)HashMap_dealloc,					  /* tp_dealloc */
	0,												  /* tp_print */
	0,												  /* tp_getattr */
	0,												  /* tp_setattr */
	0,												  /* tp_as_async */
	0,												  /* tp_repr */
	0,												  /* tp_as_number */
	&HashMap_as_sequence,							  /* tp_as_sequence */
	&HashMap_as_mapping,							  /* tp_as_mapping */
	PyObject_HashNotImplemented,					  /* tp_hash */
	0,												  /* tp_call */
	0,												  /* tp_str */
	0,												  /* tp_getattro */
	0,												  /* tp_setattro */
	0,												  /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,		  /* tp_flags */
	HashMap_doc,									  /* tp_doc */
	0,												  /* tp_traverse */
	0,												  /* tp_clear */
	0,												  /* tp_richcompare */
	0,												  /* tp_weaklistoffset */
	(getiterfunc)HashMap__iter__,					  /* tp_iter */
	0,												  /* tp_iternext */
	HashMap_methods,								  /* tp_methods */
	HashMap_members,								  /* tp_members */
	HashMap_getsetters,								  /* tp_getset */
	0,												  /* tp_base */
	0,												  /* tp_dict */
	0,												  /* tp_descr_get */
	0,												  /* tp_descr_set */
	0,												  /* tp_dictoffset */
	(initproc)HashMap_init,							  /* tp_init */
	0,												  /* tp_alloc */
	HashMap_new,									  /* tp_new */
	0,												  /* tp_free */
	0,												  /* tp_is_gc */
	0,												  /* tp_bases */
	0,												  /* tp_mro */
	0,												  /* tp_cache */
	0,												  /* tp_subclasses */
	0,												  /* tp_weaklist */
	0,												  /* tp_del */
	0,												  /* tp_version_tag */
	0												  /* tp_finalize */
};

PyMethodDef hashmap_module_methods[] = {
	{NULL, NULL, 0, NULL}};

static PyModuleDef hashmap = {
	PyModuleDef_HEAD_INIT,
	.m_name = "hashmap",
	.m_doc = "a library for a hashmap",
	.m_size = -1, // the module state is global
	.m_methods = hashmap_module_methods,
};

PyMODINIT_FUNC PyInit_hashmap(void)
{
	if (PyType_Ready(&HashMap_Type) < 0)
		return NULL;

	PyObject *mod = PyModule_Create(&hashmap);
	if (mod == NULL)
		return NULL;

	PyModule_AddIntMacro(mod, DEFAULT_MAP_SIZE);

	Py_INCREF(&HashMap_Type);
	PyModule_AddObject(mod, "HashMap", (PyObject *)&HashMap_Type);
	return mod;
}