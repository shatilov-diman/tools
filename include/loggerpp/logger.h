// Project loggerpp
// Copyright (C) 2018 Dmitry Shatilov
//
// This file is a part of the project loggerpp.
// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Additional restriction according to GPLv3 pt 7:
// b) required preservation author attributions;
// c) required preservation links to original sources
//
// Original sources:
//   https://github.com/shatilov-diman/loggerpp/
//   https://bitbucket.org/charivariltd/loggerpp/
//
// Author contacts:
//   Dmitry Shatilov (e-mail: shatilov.diman@gmail.com; site: https://www.linkedin.com/in/shatilov)
//
//

#pragma once

#include "log_base.h"
#include "log_formatter.h"

#include "../utils/utils.h"

#include <iostream>

namespace charivari_ltd
{
namespace loggerpp
{
	struct default_log_traits
	{
		using formatter_t = default_formatter;
		using key_t = std::variant<std::string, std::wstring>;
		using value_t = std::variant<nullptr_t, bool, char, std::int64_t, std::uint64_t, double, std::string, std::wstring, level, std::chrono::system_clock::time_point>;

		struct item_t
		{
			key_t key;
			value_t value;
		};

		using tags_t = std::deque<std::pair<key_t, value_t>>;

		static inline tags_t extend_back(tags_t&& tags, tags_t&& t)
		{
			for (auto&& item : t)
				tags.push_back(std::move(item));
			return tags;
		}

		static inline tags_t extend_front(tags_t&& tags, tags_t&& t)
		{
			for (auto&& item : t)
				tags.push_front(std::move(item));
			return tags;
		}
	};


	template<typename tags_t>
	inline void check_guarantee_size(const tags_t& tags)
	{
		if (tags.size() < constants::index_guaratee_size)
			throw std::runtime_error("check_guarantee_size: size(tags) < constants::log_guaratee_size");
	}

	template<typename tags_t>
	inline std::chrono::system_clock::time_point get_time(const tags_t& tags)
	{
		check_guarantee_size(tags);
		return std::get<std::chrono::system_clock::time_point>(tags[constants::index_time].second);
	}

	template<typename tags_t>
	inline level get_level(const tags_t& tags)
	{
		check_guarantee_size(tags);
		return std::get<level>(tags[constants::index_level].second);
	}

	template<typename tags_t>
	inline std::string get_message(const tags_t& tags)
	{
		check_guarantee_size(tags);
		return std::get<std::string>(tags[constants::index_message].second);
	}

	template<typename tags_t>
	inline std::wstring get_wmessage(const tags_t& tags)
	{
		check_guarantee_size(tags);
		return std::get<std::wstring>(tags[constants::index_message].second);
	}

	template<typename tags_t, typename key_t, typename value_t>
	inline std::optional<value_t> get_tag(const tags_t& tags, const key_t& key)
	{
		for (const auto& t : tags)
			if (t.key == key)
				return t.value;
		return {};
	}

	template<typename type_t, typename tags_t,  typename key_t>
	inline std::optional<type_t> get_tag(const tags_t& tags, const key_t& key)
	{
		if (auto opt = get_tag(tags, key))
			return { std::get<type_t>(*opt) };
		return {};
	}

	inline std::string to_string(const default_log_traits::key_t& key)
	{
		return std::visit([] (const auto& value) {
			return utils::to_string(value);
		}, key);
	}
	inline std::wstring to_wstring(const default_log_traits::key_t& key)
	{
		return std::visit([] (const auto& value) {
			return utils::to_wstring(value);
		}, key);
	}
	inline std::string to_string(const default_log_traits::value_t& value)
	{
		return std::visit([] (const auto& value) {
			return utils::to_string(value);
		}, value);
	}
	inline std::wstring to_wstring(const default_log_traits::value_t& value)
	{
		return std::visit([] (const auto& value) {
			return utils::to_wstring(value);
		}, value);
	}

	inline void default_consumer(const default_log_traits::tags_t& tags)
	{
		std::cout
			<< utils::to_string(get_time(tags)) << '\t'
			<< utils::to_string(get_level(tags)) << '\t'
			<< get_message(tags) << std::endl;
	}
} //namespace loggerpp

	using logger = loggerpp::logger_base<loggerpp::default_log_traits>;
} //namespace charivari_ltd
