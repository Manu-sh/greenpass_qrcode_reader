#pragma once

#define USE_CBOR_CONTEXT
#include <cn-cbor/cn-cbor.h>

extern cn_cbor_context *context;
#define CBOR_CONTEXT_PARAM , context
#define CBOR_CONTEXT_PARAM_COMMA context,
