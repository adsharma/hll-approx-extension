#define DUCKDB_EXTENSION_MAIN

#include "hll_approx_extension.hpp"

#include "approx_functions.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/aggregate_function.hpp"
#include "duckdb/function/scalar_function.hpp"
#include "duckdb/main/extension_util.hpp"

#include <duckdb/parser/parsed_data/create_aggregate_function_info.hpp>
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>

// OpenSSL linked through vcpkg
#include <openssl/opensslv.h>

namespace duckdb {

static void RegisterAggregateFunction(DatabaseInstance &db, const char *name, AggregateFunction fun) {
	// aggregate function
	AggregateFunctionSet result;
	result.AddFunction(fun);
	result.name = name;
	ExtensionUtil::RegisterFunction(db, std::move(result));
}

static void LoadInternal(DatabaseInstance &instance) {
	RegisterAggregateFunction(instance, hll_approx::ApproxCountDistinctFun::Name,
	                          hll_approx::ApproxCountDistinctFun::GetFunction());
	ExtensionUtil::RegisterFunction(instance, hll_approx::ApproxQuantileFun::GetFunctions());
}

void HllApproxExtension::Load(DuckDB &db) {
	LoadInternal(*db.instance);
}
std::string HllApproxExtension::Name() {
	return "hll_approx";
}

std::string HllApproxExtension::Version() const {
#ifdef EXT_VERSION_HLL_APPROX
	return EXT_VERSION_HLL_APPROX;
#else
	return "";
#endif
}

} // namespace duckdb

extern "C" {

DUCKDB_EXTENSION_API void hll_approx_init(duckdb::DatabaseInstance &db) {
	duckdb::DuckDB db_wrapper(db);
	db_wrapper.LoadExtension<duckdb::HllApproxExtension>();
}

DUCKDB_EXTENSION_API const char *hll_approx_version() {
	return duckdb::DuckDB::LibraryVersion();
}
}

#ifndef DUCKDB_EXTENSION_MAIN
#error DUCKDB_EXTENSION_MAIN not defined
#endif
