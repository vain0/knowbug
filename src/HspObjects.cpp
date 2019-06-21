#include "hpiutil/hpiutil.hpp"
#include "HspDebugApi.h"
#include "HspObjects.h"
#include "HspStaticVars.h"

static auto const GLOBAL_MODULE_ID = std::size_t{ 0 };

static auto const GLOBAL_MODULE_NAME = "@";

static auto var_name_to_scope_resolution(char const* var_name) -> char const* {
	return std::strchr(var_name, '@');
}

// 変数をモジュールごとに分類する。
static auto group_vars_by_module(std::vector<HspString> const& var_names) -> std::vector<HspObjects::Module> {
	auto modules = std::vector<HspObjects::Module>{};

	// モジュール名、変数名、変数IDの組
	auto tuples = std::vector<std::tuple<HspStringView, HspStringView, std::size_t>>{};

	{
		for (auto vi = std::size_t{}; vi < var_names.size(); vi++) {
			auto&& var_name = var_names[vi];

			auto p = var_name_to_scope_resolution(var_name.data());
			auto module_name = HspStringView{ (p ? p : GLOBAL_MODULE_NAME) };

			tuples.emplace_back(module_name, var_name.as_ref(), vi);
		}
	}

	std::sort(std::begin(tuples), std::end(tuples));

	// モジュールと変数の関係を構築する。
	{
		modules.emplace_back(HspStringView{ GLOBAL_MODULE_NAME }.to_owned());

		for (auto&& t : tuples) {
			auto module_name_ref = std::get<0>(t);
			auto var_name_ref = std::get<1>(t);
			auto vi = std::get<2>(t);

			{
				auto module_id = modules.size() - 1;
				if (!(modules[module_id].name() == module_name_ref)) {
					modules.emplace_back(module_name_ref.to_owned());
				}
			}

			{
				auto module_id = modules.size() - 1;
				assert(modules[module_id].name() == module_name_ref);
				modules[module_id].add_var(vi);
			}
		}
	}

	// 事後条件
	assert(modules[GLOBAL_MODULE_ID].name() == HspStringView{ "@" });
	return modules;
}

static auto create_type_datas() -> std::vector<HspObjects::TypeData> {
	auto types = std::vector<HspObjects::TypeData>{};
	types.emplace_back(HspStringView{ "unknown" }.to_owned());
	types.emplace_back(HspStringView{ "label" }.to_owned());
	types.emplace_back(HspStringView{ "str" }.to_owned());
	types.emplace_back(HspStringView{ "double" }.to_owned());
	types.emplace_back(HspStringView{ "int" }.to_owned());
	types.emplace_back(HspStringView{ "struct" }.to_owned());
	types.emplace_back(HspStringView{ "comobj" }.to_owned());
	return types;
}

static auto path_to_pval(HspObjectPath const& path, HspDebugApi& api) -> std::optional<PVal*> {
	switch (path.kind()) {
	case HspObjectKind::StaticVar:
		{
			auto static_var_id = path.as_static_var().static_var_id();
			return std::make_optional(api.static_var_to_pval(static_var_id));
		}
	default:
		return std::nullopt;
	}
}

static auto path_to_data(HspObjectPath const& path, HspDebugApi& api) -> std::optional<HspData> {
	// FIXME: 静的変数も値を提供できる

	switch (path.kind()) {
	case HspObjectKind::Element:
		{
			auto&& pval = path_to_pval(*path.parent(), api);
			if (!pval) {
				assert(false && u8"配列要素の親は変数であるはず");
				return std::nullopt;
			}

			auto aptr = api.var_element_to_aptr(*pval, path.as_element().indexes());
			return std::make_optional( api.var_element_to_data(*pval, aptr));
		}
	default:
		return std::nullopt;
	}
}

static auto path_to_int(HspObjectPath::Int const& path, HspDebugApi& api) -> std::optional<HspInt> {
	auto&& data = path_to_data(*path.parent(), api);
	if (!data) {
		assert(false && u8"int の親は data を生成できるはず");
		return std::nullopt;
	}

	if (data->type() != HspType::Int) {
		return std::nullopt;
	}

	return std::make_optional(api.data_to_int(*data));
}

// -----------------------------------------------
// HspObjects
// -----------------------------------------------

HspObjects::HspObjects(HspDebugApi& api, HspStaticVars& static_vars)
	: api_(api)
	, static_vars_(static_vars)
	, modules_(group_vars_by_module(static_vars.get_all_names()))
	, types_(create_type_datas())
{
}

auto HspObjects::type_name(HspType type) const->HspStringView {
	auto type_id = (std::size_t)type;
	if (!(1 <= type_id && type_id < types_.size())) {
		return types_[0].name();
	}
	return types_[type_id].name();
}

auto HspObjects::module_global_id() const->std::size_t {
	return GLOBAL_MODULE_ID;
}

auto HspObjects::module_count() const->std::size_t {
	return modules_.size();
}

auto HspObjects::module_name(std::size_t module_id) const -> HspStringView {
	return modules_.at(module_id).name();
}

auto HspObjects::module_var_count(std::size_t module_id) const->std::size_t {
	return modules_.at(module_id).var_ids().size();
}

auto HspObjects::module_var_at(std::size_t module_id, std::size_t index) const->std::size_t {
	return modules_.at(module_id).var_ids().at(index);
}

auto HspObjects::static_var_name(std::size_t static_var_id)->std::string {
	return *api_.static_var_find_name(static_var_id);
}

bool HspObjects::static_var_is_array(std::size_t static_var_id) {
	return hpiutil::PVal_isStandardArray(&hpiutil::staticVars()[static_var_id]);
}

auto HspObjects::static_var_to_pval(std::size_t static_var_id)->PVal* {
	return api_.static_var_to_pval(static_var_id);
}

auto HspObjects::static_var_to_type(std::size_t static_var_id)->HspType {
	return api_.var_to_type(api_.static_var_to_pval(static_var_id));
}

auto HspObjects::static_var_child_count(HspObjectPath::StaticVar const& path) const->std::size_t {
	auto static_var_id = path.static_var_id();
	auto pval = api_.static_var_to_pval(static_var_id);
	return api_.var_element_count(pval);
}

auto HspObjects::static_var_child_at(HspObjectPath::StaticVar const& path, std::size_t child_index) const->std::shared_ptr<HspObjectPath const> {
	auto aptr = (APTR)child_index;
	auto static_var_id = path.static_var_id();
	auto pval = api_.static_var_to_pval(static_var_id);
	auto&& indexes = api_.var_element_to_indexes(pval, aptr);
	return path.new_element(indexes);
}

auto HspObjects::path_to_int(HspObjectPath::Int const& path) const -> HspInt {
	return (::path_to_int(path, api_)).value_or(HspInt{});
}

// -----------------------------------------------
// HspObjects::Module
// -----------------------------------------------

HspObjects::Module::Module(HspString&& name)
	: name_(std::move(name))
	, var_ids_()
{
}

void HspObjects::Module::add_var(std::size_t static_var_id) {
	var_ids_.emplace_back(static_var_id);
}

// -----------------------------------------------
// HspObjects::TypeData
// -----------------------------------------------

HspObjects::TypeData::TypeData(HspString&& name)
	: name_(std::move(name))
{
}
