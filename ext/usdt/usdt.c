#include <ruby.h>
#include "usdt.h"

VALUE USDT;
VALUE USDT_Provider;
VALUE USDT_Probe;
VALUE USDT_Error;

static VALUE provider_create(VALUE self, VALUE name, VALUE mod);
static VALUE provider_probe(int argc, VALUE *argv, VALUE self);
static VALUE provider_enable(VALUE self);
static VALUE probe_enabled(VALUE self);

void Init_usdt() {
  USDT = rb_define_module("USDT");

  USDT_Error = rb_define_class_under(USDT, "Error", rb_eRuntimeError);

  USDT_Provider = rb_define_class_under(USDT, "Provider", rb_cObject);
  rb_define_singleton_method(USDT_Provider, "create", provider_create, 2);
  rb_define_method(USDT_Provider, "probe", provider_probe, -1);
  rb_define_method(USDT_Provider, "enable", provider_enable, 0);

  USDT_Probe = rb_define_class_under(USDT, "Probe", rb_cObject);
  rb_define_method(USDT_Probe, "enabled?", probe_enabled, 0);
}

/**
 * USDT::Provider.create :name, :modname
 */
static VALUE provider_create(VALUE self, VALUE name, VALUE mod) {
  usdt_provider_t* p = usdt_create_provider(
    STR2CSTR(name),
    STR2CSTR(mod)
  );

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
  const char *func = STR2CSTR(argv[0]);
  const char *name = STR2CSTR(argv[1]);
  const char *types[6];
  size_t i, pargc = 0;

  for (i = 0; i < 6; i++) {
    if (i < argc-2 && TYPE(argv[i+2]) == T_STRING) {
      types[i] = STR2CSTR(argv[i+2]);
      pargc++;
    } else {
      types[i] = NULL;
    }
  }

  usdt_provider_t *provider = DATA_PTR(self);

  usdt_probedef_t **probe;
  probe = ALLOC(usdt_probedef_t *);
  *probe = usdt_create_probe(func, name, pargc, types);

  usdt_provider_add_probe(provider, *probe);
  VALUE rbProbe = Data_Wrap_Struct(USDT_Probe, NULL, free, probe);
  return rbProbe;
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
