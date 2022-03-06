#ifndef PYTHON_MACROS_H__INCLUDED
#define PYTHON_MACROS_H__INCLUDED

#  if PY_VERSION_HEX >= 0x03000000
#define PY_INIT_RET PyObject*
#define PY_INIT_NAME(name) PyInit_##name
#else
#define PY_INIT_RET void
#define PY_INIT_NAME(name) init##name
#endif

#define PN_INIT_PYTHON_MODULE(name) \
	if(PyImport_AppendInittab(#name, PY_INIT_NAME(name)) == -1) \
	throw std::runtime_error("Failed to add " #name " module to builtins");

#endif // #ifndef PYTHON_MACROS_H__INCLUDED