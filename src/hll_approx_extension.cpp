#define DUCKDB_EXTENSION_MAIN

#include "hll_approx_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar_function.hpp"
#include "duckdb/main/extension_util.hpp"
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>

// OpenSSL linked through vcpkg
#include <openssl/opensslv.h>

namespace duckdb {

inline void HllApproxScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
    auto &name_vector = args.data[0];
    UnaryExecutor::Execute<string_t, string_t>(
	    name_vector, result, args.size(),
	    [&](string_t name) {
			return StringVector::AddString(result, "HllApprox "+name.GetString()+" 🐥");;
        });
}

inline void HllApproxOpenSSLVersionScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
    auto &name_vector = args.data[0];
    UnaryExecutor::Execute<string_t, string_t>(
	    name_vector, result, args.size(),
	    [&](string_t name) {
			return StringVector::AddString(result, "HllApprox " + name.GetString() +
                                                     ", my linked OpenSSL version is " +
                                                     OPENSSL_VERSION_TEXT );;
        });
}

static void LoadInternal(DatabaseInstance &instance) {
    // Register a scalar function
    auto hll_approx_scalar_function = ScalarFunction("hll_approx", {LogicalType::VARCHAR}, LogicalType::VARCHAR, HllApproxScalarFun);
    ExtensionUtil::RegisterFunction(instance, hll_approx_scalar_function);

    // Register another scalar function
    auto hll_approx_openssl_version_scalar_function = ScalarFunction("hll_approx_openssl_version", {LogicalType::VARCHAR},
                                                LogicalType::VARCHAR, HllApproxOpenSSLVersionScalarFun);
    ExtensionUtil::RegisterFunction(instance, hll_approx_openssl_version_scalar_function);
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
