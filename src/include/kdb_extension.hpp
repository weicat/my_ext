#pragma once

#include "duckdb.hpp"

namespace duckdb {

class KdbExtension : public Extension {
public:
	void Load(DuckDB &db) override;
	std::string Name() override;
	std::string Version() const override;
};

} // namespace duckdb
