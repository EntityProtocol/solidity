/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <libyul/optimiser/ASTWalker.h>

namespace yul
{

/**
 * Structural simplifier. Performs the following simplification steps:
 * - remove if with empty body and movable condition
 * - replace if with empty body and non-movable condition with pop(condition)
 * - remove if with true condition (replace with body)
 * - remove if with false condition
 * - turn switch with single case into if
 * - replace switch with only default case with pop(expression) and body
 * - remove for with false condition
 *
 * Prerequisites: Disambiguator
 */
class StructuralSimplifier: public ASTModifier
{
public:
	using ASTModifier::operator();
	void operator()(Block& _block) override;
private:
	static bool isTrue(Literal const& _literal);
	static bool isFalse(Literal const& _literal);
};

}
