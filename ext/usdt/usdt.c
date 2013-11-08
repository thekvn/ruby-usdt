#include <ruby.h>
#include "usdt.h"

VALUE USDT;
VALUE USDT_Provider;
VALUE USDT_Probe;
VALUE USDT_Error;

static VALUE provider_create(VALUE self, VALUE name, VALUE mod);
static VALUE provider_probe(int argc, VALUE *argv, VALUE self);
static VALUE provider_remove_probe(VALUE self, VALUE probe);
static VALUE provider_enable(VALUE self);
static VALUE provider_disable(VALUE self);
static VALUE probe_enabled(VALUE self);
static VALUE probe_fire(int argc, VALUE *argv, VALUE self);
static VALUE probe_function(VALUE self);
static VALUE probe_name(VALUE self);
static VALUE probe_args(VALUE self);

void Init_usdt() {
  USDT = rb_define_module("USDT");

  USDT_Error = rb_define_class_under(USDT, "Error", rb_eRuntimeError);

  USDT_Provider = rb_define_class_under(USDT, "Provider", rb_cObject);
  rb_define_singleton_method(USDT_Provider, "create", provider_create, 2);
  rb_define_method(USDT_Provider, "probe", provider_probe, -1);
  rb_define_method(USDT_Provider, "remove_probe", provider_remove_probe, 1);
  rb_define_method(USDT_Provider, "enable", provider_enable, 0);
  rb_define_method(USDT_Provider, "disable", provider_disable, 0);

  USDT_Probe = rb_define_class_under(USDT, "Probe", rb_cObject);
  rb_define_method(USDT_Probe, "enabled?", probe_enabled, 0);
  rb_define_method(USDT_Probe, "fire", probe_fire, -1);
  rb_define_method(USDT_Probe, "function", probe_function, 0);
  rb_define_method(USDT_Probe, "name", probe_name, 0);
  rb_define_method(USDT_Probe, "arguments", probe_args, 0);
}

/**
 * USDT::Provider.create :name, :modname
 */
static VALUE provider_create(VALUE self, VALUE name, VALUE mod) {
  Check_Type(name, T_SYMBOL);
  Check_Type(mod, T_SYMBOL);

  const char *namestr = rb_id2name(rb_to_id(name));
  const char *modstr = rb_id2name(rb_to_id(mod));

  usdt_provider_t* p = usdt_create_provider(namestr, modstr);

  VALUE rbProvider = Data_Wrap_Struct(USDT_Provider, NULL, free, p);

  if (rb_block_given_p()) {
    rb_yield(rbProvider);
  }

  return rbProvider;
}

/**
 * USDT::Provider#probe(func, name, pargs*)
 */
static VALUE provider_probe(int argc, VALUE *argv, VALUE self) {
  const char *func = rb_id2name(rb_to_id(argv[0]));
  const char *name = rb_id2name(rb_to_id(argv[1]));
  const char *types[USDT_ARG_MAX];
  size_t i, pargc = 0;
  size_t t_int = rb_intern("integer");
  size_t t_str = rb_intern("string");

  for (i = 0; i < USDT_ARG_MAX; i++) {
    if (i < argc - 2) {
      Check_Type(argv[i+2], T_SYMBOL);
      if (t_int == rb_to_id(argv[i+2])) {
        types[i] = "int";
        pargc++;
      } else if (t_str == rb_to_id(argv[i+2])) {
        types[i] = "char *";
        pargc++;
      } else {
        types[i] = NULL;
      }
    } else {
      types[i] = NULL;
    }
  }

  usdt_provider_t *provider = DATA_PTR(self);

  usdt_probedef_t **probe;
  probe = ALLOC(usdt_probedef_t *);
  *probe = usdt_create_probe(func, name, pargc, types);

  if ((usdt_provider_add_probe(provider, *probe) == 0)) {
    VALUE rbProbe = Data_Wrap_Struct(USDT_Probe, NULL, free, probe);
    return rbProbe;
  }
  else {
    rb_raise(USDT_Error, "%s", usdt_errstr(provider));
  }
}

/**
 * USDT::Provider#remove_probe(probe)
 */
static VALUE provider_remove_probe(VALUE self, VALUE probe) {
  usdt_provider_t *provider = DATA_PTR(self);
  usdt_probedef_t **p = DATA_PTR(probe);
  usdt_probedef_t *probedef = *p;

  usdt_provider_remove_probe(provider, probedef);

  return Qtrue;
}

/**
 * USDT::Provider#enable
 */
static VALUE provider_enable(VALUE self) {
  usdt_provider_t *provider = DATA_PTR(self);
  int status = usdt_provider_enable(provider);
  if (status == 0) {
    return Qtrue;
  } else {
    rb_raise(USDT_Error, "%s", usdt_errstr(provider));
  }
}

/**
 * USDT::Provider#disable
 */
static VALUE provider_disable(VALUE self) {
  usdt_provider_t *provider = DATA_PTR(self);
  int status = usdt_provider_disable(provider);
  if (status == 0) {
    return Qtrue;
  } else {
    rb_raise(USDT_Error, "%s", usdt_errstr(provider));
  }
}

/**
 * USDT::Probe#enabled?
 */
static VALUE probe_enabled(VALUE self) {
  usdt_probedef_t **p = DATA_PTR(self);
  usdt_probedef_t *pd = *p;

  if (usdt_is_enabled(pd->probe) == 0) {
    return Qfalse;
  } else {
    return Qtrue;
  }
}

/**
 * USDT::Probe#fire *args
 */
static VALUE probe_fire(int argc, VALUE *argv, VALUE self) {
  if (probe_enabled(self) == Qfalse) {
    return Qfalse;
  }

  usdt_probedef_t **p = DATA_PTR(self);
  usdt_probedef_t *probedef = *p;

  void *pargs[USDT_ARG_MAX];
  size_t i;

  for (i = 0; i < probedef->argc; i++) {
    if (probedef->types[i] == USDT_ARGTYPE_STRING) {
      Check_Type(argv[i], T_STRING);
      pargs[i] = (void *) RSTRING_PTR(argv[i]);
    } else if (probedef->types[i] == USDT_ARGTYPE_INTEGER) {
      Check_Type(argv[i], T_FIXNUM);
      pargs[i] = (void *) FIX2INT(argv[i]);
    } else {
      pargs[i] = NULL;
    }
  }

  usdt_fire_probe(probedef->probe, probedef->argc, pargs);
  return Qtrue;
}

/**
 * USDT::Probe#function
 */
static VALUE probe_function(VALUE self) {
  usdt_probedef_t **p = DATA_PTR(self);
  usdt_probedef_t *probedef = *p;
  return ID2SYM(rb_intern(probedef->function));
}

/**
 * USDT::Probe#name
 */
static VALUE probe_name(VALUE self) {
  usdt_probedef_t **p = DATA_PTR(self);
  usdt_probedef_t *probedef = *p;
  return ID2SYM(rb_intern(probedef->name));
}

/**
 * USDT::Probe#arguments
 */
static VALUE probe_args(VALUE self) {
  usdt_probedef_t **p = DATA_PTR(self);
  usdt_probedef_t *probedef = *p;

  VALUE args = rb_ary_new();
  size_t i;

  for (i = 0; i < probedef->argc; i++) {
    if (probedef->types[i] == USDT_ARGTYPE_STRING) {
      rb_ary_push(args, ID2SYM(rb_intern("string")));
    } else if (probedef->types[i] == USDT_ARGTYPE_INTEGER) {
      rb_ary_push(args, ID2SYM(rb_intern("integer")));
    } else {
      rb_ary_push(args, Qnil);
    }
  }
  return args;
}
