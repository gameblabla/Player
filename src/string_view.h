/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EP_STRING_VIEW_H
#define EP_STRING_VIEW_H

#include <lcf/string_view.h>
#include <lcf/dbstring.h>
#include <format>
using StringView = lcf::StringView;
using U32StringView = lcf::U32StringView;

using lcf::ToString;
using lcf::ToStringView;

template<>
struct std::formatter<lcf::StringView> : formatter<string_view> {
	auto format(const lcf::StringView& s, format_context& ctx) const -> decltype(ctx.out());
};

template<>
struct std::formatter<lcf::DBString> : formatter<string_view> {
	auto format(const lcf::DBString& s, format_context& ctx) const -> decltype(ctx.out());
};

#endif
