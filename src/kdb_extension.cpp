#define DUCKDB_EXTENSION_MAIN

#include "kdb_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar_function.hpp"
#include "duckdb/main/extension_util.hpp"
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>

// OpenSSL linked through vcpkg
#include <openssl/opensslv.h>

namespace duckdb {

inline void KdbScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &name_vector = args.data[0];
	UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
		return StringVector::AddString(result, "Kdb " + name.GetString() + " 🐥");
	});
}

inline void KdbOpenSSLVersionScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &name_vector = args.data[0];
	UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
		return StringVector::AddString(result, "Kdb " + name.GetString() + ", my linked OpenSSL version is " +
		                                           OPENSSL_VERSION_TEXT);
	});
}

static void LoadInternal(DatabaseInstance &instance) {
	// Register a scalar function
	auto kdb_scalar_function = ScalarFunction("kdb", {LogicalType::VARCHAR}, LogicalType::VARCHAR, KdbScalarFun);
	ExtensionUtil::RegisterFunction(instance, kdb_scalar_function);

	// Register another scalar function
	auto kdb_openssl_version_scalar_function = ScalarFunction("kdb_openssl_version", {LogicalType::VARCHAR},
	                                                            LogicalType::VARCHAR, KdbOpenSSLVersionScalarFun);
	ExtensionUtil::RegisterFunction(instance, kdb_openssl_version_scalar_function);
}

void KdbExtension::Load(DuckDB &db) {
	LoadInternal(*db.instance);
}
std::string KdbExtension::Name() {
	return "kdb";
}

std::string KdbExtension::Version() const {
#ifdef EXT_VERSION_KDB
	return EXT_VERSION_KDB;
#else
	return "";
#endif
}

} // namespace duckdb

extern "C" {

DUCKDB_EXTENSION_API void kdb_init(duckdb::DatabaseInstance &db) {
	duckdb::DuckDB db_wrapper(db);
	db_wrapper.LoadExtension<duckdb::KdbExtension>();
}

DUCKDB_EXTENSION_API const char *kdb_version() {
	return duckdb::DuckDB::LibraryVersion();
}
}

#ifndef DUCKDB_EXTENSION_MAIN
#error DUCKDB_EXTENSION_MAIN not defined
#endif
